#include "Application.h"

#include "Events/EventVisitor.h"
#include "FileSystem/FileSystem.h"
#include "KeyCodes.h"
#include "LayerStack.h"
#include "Renderer.h"
#include "Scene.h"

// Temporary TODO: remove
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#define ASSIMP_LOAD_FLAGS aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType  //

namespace Fuego
{
class Application::ApplicationImpl
{
    friend class Application;
    std::unique_ptr<Window> m_Window;
    std::unique_ptr<EventQueue> m_EventQueue;
    std::unique_ptr<Renderer::Renderer> _renderer;
    std::unique_ptr<Fuego::FS::FileSystem> _fs;
    std::vector<std::unique_ptr<Fuego::Renderer::Model>> _models;
    std::unordered_map<std::string, std::unique_ptr<Fuego::Renderer::Texture>> _textures;

    bool initialized = false;
    bool m_Running;
    LayerStack m_LayerStack;
};

Application::Application()
    : d(new ApplicationImpl())
{
}

Application::~Application()
{
    delete d;
}

Renderer::Renderer& Application::Renderer()
{
    return *d->_renderer.get();
}

void Application::PushLayer(Layer* layer)
{
    d->m_LayerStack.PushLayer(layer);
    layer->OnAttach();
}
void Application::PushOverlay(Layer* overlay)
{
    d->m_LayerStack.PushOverlay(overlay);
}

void Application::OnEvent(EventVariant& event)
{
    // clang-format off
    auto ApplicationEventVisitor = EventVisitor{[this](WindowCloseEvent&    ev) { OnWindowClose(ev); },
                                                [this](WindowResizeEvent&   ev) { OnWindowResize(ev); },
                                                [this](AppRenderEvent&      ev) { OnRenderEvent(ev); },
                                                [this](WindowStartResizeEvent&    ev) { OnStartResizeWindow(ev); },
                                                [this](WindowEndResizeEvent&    ev) { OnEndResizeWindow(ev); },
                                                [this](WindowValidateEvent&    ev) { OnValidateWindow(ev); },
                                                [this](MouseMovedEvent&    ev) { OnMouseMoveEvent(ev); },
                                                [this](KeyPressedEvent&    ev) { OnKeyPressEvent(ev); },
                                                [](Event&){}};
    // clang-format on

    std::visit(ApplicationEventVisitor, event);

    for (auto it = d->m_LayerStack.end(); it != d->m_LayerStack.begin();)
    {
        (*--it)->OnEvent(event);

        auto HandledEventVisitor = EventVisitor{[](const Event& ev) -> bool
                                                {
                                                    // FU_CORE_TRACE("{0}", ev.ToString());
                                                    return ev.GetHandled();
                                                }};

        if (std::visit(HandledEventVisitor, event))
        {
            break;
        }
    }
}

bool Application::OnWindowClose(WindowCloseEvent& event)
{
    d->m_Running = false;
    event.SetHandled();
    return true;
}
bool Application::OnWindowResize(WindowResizeEvent& event)
{
    d->_renderer->ChangeViewport(event.GetX(), event.GetY(), event.GetWidth(), event.GetHeight());
    event.SetHandled();
    return true;
}
bool Application::OnStartResizeWindow(WindowStartResizeEvent& event)
{
    event.SetHandled();
    return true;
}
bool Application::OnEndResizeWindow(WindowEndResizeEvent& event)
{
    event.SetHandled();
    return true;
}
bool Application::OnValidateWindow(WindowValidateEvent& event)
{
    d->_renderer->ValidateWindow();
    d->m_Window->SetPainted();
    event.SetHandled();
    return true;
}
bool Application::OnKeyPressEvent(KeyPressedEvent& event)
{
    KeyEvent& e = (KeyEvent&)event;
    KeyCode crossplatform_key = e.GetKeyCode();

    switch (crossplatform_key)
    {
    case Key::D1:
        d->_renderer->ToggleWireFrame();
        break;
    case Key::D2:
        d->m_Window->SwitchInteractionMode();
        break;
    }
    event.SetHandled();
    return true;
}
bool Application::OnRenderEvent(AppRenderEvent& event)
{
    d->_renderer->ShowWireFrame();
    // TODO: As for now we use just one opaque shader, but we must think about different passes
    // using different shaders with blending and probably using pre-passes
    d->_renderer->SetShaderObject(d->_renderer->opaque_shader.get());
    d->_renderer->CurrentShaderObject()->Use();
    float counter = 1.f;
    for (auto it = d->_models.begin(); it != d->_models.end(); ++it)
    {
        Fuego::Renderer::Model* model_ptr = it->get();
        d->_renderer->DrawModel(model_ptr, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, counter)));
        counter += 10.f;
    }

    // event.SetHandled();
    UNUSED(event);
    return true;
}
bool Application::OnMouseMoveEvent(MouseMovedEvent& event)
{
    UNUSED(event);
    return true;
}

void Application::Init()
{
    d->_fs = std::unique_ptr<Fuego::FS::FileSystem>(new Fuego::FS::FileSystem());
    d->m_EventQueue = EventQueue::CreateEventQueue();
    d->m_Window = Window::CreateAppWindow(WindowProps(), *d->m_EventQueue);
    d->_renderer.reset(new Renderer::Renderer());
    d->m_Running = true;
    FS::FileSystem& fs = Application::instance().FileSystem();

    AddTexture("fallback.png");
    LoadModel("Shotgun/Shotgun.obj");
    LoadModel("WaterCooler/WaterCooler.obj");

    d->initialized = true;
}

Fuego::FS::FileSystem& Application::FileSystem()
{
    return *d->_fs.get();
}

Window& Application::GetWindow()
{
    return *d->m_Window;
}

Fuego::Renderer::Model* Application::LoadModel(std::string_view path)
{
    Assimp::Importer importer{};
    const aiScene* scene = importer.ReadFile(d->_fs->GetFullPathToFile(path.data()), ASSIMP_LOAD_FLAGS);
    if (!scene)
        return nullptr;
    d->_models.emplace_back(std::make_unique<Fuego::Renderer::Model>(scene));
    Fuego::Renderer::Model* model = d->_models.back().get();
    return model;
}

bool Application::IsTextureLoaded(std::string_view texture) const
{
    auto it = d->_textures.find(texture.data());
    return it != d->_textures.end();
}

bool Application::AddTexture(std::string_view path)
{
    unsigned char* data = nullptr;
    int width, height, bits;
    bool res = false;

    if (path.data() && path.size() > 0)
        res = d->_fs->Load_Image(path.data(), bits, data, width, height);
    if (res)
        d->_textures.emplace(std::string(path.data()), std::move(d->_renderer->CreateTexture(data, width, height)));
    return res;
}

const Fuego::Renderer::Texture* Application::GetLoadedTexture(std::string_view name) const
{
    if (name.empty())
        return d->_textures.find("fallback.png")->second.get();

    auto it = d->_textures.find(name.data());
    if (it != d->_textures.end() && it->second)
        return it->second.get();
    return nullptr;
}

void Application::Run()
{
    if (!d->initialized)
    {
        Init();
    }

    while (d->m_Running)
    {
        d->_renderer->Clear();
        d->m_EventQueue->Update();
        d->m_Window->Update();
        Fuego::Renderer::Camera::GetActiveCamera()->Update();

        for (auto layer : d->m_LayerStack)
        {
            layer->OnUpdate();
        }

        while (!d->m_EventQueue->Empty())
        {
            auto ev = d->m_EventQueue->Front();
            OnEvent(*ev);
            d->m_EventQueue->Pop();
        }
        d->_renderer->Present();
    }
}

}  // namespace Fuego
