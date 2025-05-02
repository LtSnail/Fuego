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
template <>
Application& singleton<Application>::instance()
{
    static Application inst;
    return inst;
}

class Application::ApplicationImpl
{
    friend class Application;
    std::unique_ptr<Window> m_Window;
    std::unique_ptr<EventQueue> m_EventQueue;
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
{  // clang-format off
    auto ApplicationEventVisitor = EventVisitor{[this](WindowResizeEvent&   ev) { OnWindowResize(ev); }, 
                                                [this](WindowStartResizeEvent&   ev) { OnStartResizeWindow(ev); },
                                                [this](WindowEndResizeEvent&    ev) {OnEndResizeWindow(ev); },
                                                [this](WindowValidateEvent&    ev) {OnValidateWindow(ev); },
                                                [this](WindowCloseEvent&    ev) {OnWindowClose(ev); },
                                                [this](AppRenderEvent&    ev) {OnRenderEvent(ev); },
                                                [this](KeyPressedEvent&    ev) {OnKeyPressEvent(ev); },
                                                [](auto&) {}
        };
    // clang-format on

    std::visit(ApplicationEventVisitor, event);

    for (auto it = d->m_LayerStack.end(); it != d->m_LayerStack.begin();)
    {
        (*--it)->OnEvent(event);

        if (std::visit([](auto&& e) { return e.GetHandled(); }, event))
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
    ServiceLocator::instance().GetService<Fuego::Renderer::Renderer>()->ChangeViewport(event.GetX(), event.GetY(), event.GetWidth(), event.GetHeight());
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
    ServiceLocator::instance().GetService<Fuego::Renderer::Renderer>()->ValidateWindow();
    d->m_Window->SetPainted();
    event.SetHandled();
    return true;
}
bool Application::OnKeyPressEvent(KeyPressedEvent& event)
{
    KeyCode crossplatform_key = event.GetKeyCode();

    switch (crossplatform_key)
    {
    case Key::D1:
        ServiceLocator::instance().GetService<Fuego::Renderer::Renderer>()->ToggleWireFrame();
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
    auto renderer = ServiceLocator::instance().GetService<Fuego::Renderer::Renderer>();
    renderer->ShowWireFrame();
    // TODO: As for now we use just one opaque shader, but we must think about different passes
    // using different shaders with blending and probably using pre-passes
    ServiceLocator::instance().GetService<Fuego::Renderer::Renderer>()->SetShaderObject(renderer->opaque_shader.get());
    renderer->CurrentShaderObject()->Use();
    float counter = 1.f;
    for (auto it = d->_models.begin(); it != d->_models.end(); ++it)
    {
        Fuego::Renderer::Model* model_ptr = it->get();
        renderer->DrawModel(model_ptr, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, counter)));
        counter += 10.f;
    }
    UNUSED(event);
    return true;
}
bool Application::OnMouseMoveEvent(MouseMovedEvent& event)
{
    UNUSED(event);
    return true;
}

Window& Application::GetWindow()
{
    return *d->m_Window;
}

void Application::Init()
{
    d->_fs = std::make_unique<FS::FileSystem>();
    d->m_EventQueue = EventQueue::CreateEventQueue();
    d->m_Window = Window::CreateAppWindow(WindowProps(), *d->m_EventQueue);

    auto fs = ServiceLocator::instance().AddService<Fuego::FS::FileSystem, CheckFileSystem>();
    if (fs)
        fs.value()->FUCreateFile("test", "test");
    ServiceLocator::instance().AddService<Fuego::Renderer::Renderer, CheckRenderer>();

    d->_models.reserve(10);

    AddTexture("fallback.png");
    LoadModel("Shotgun/Shotgun.obj");
    LoadModel("WaterCooler/WaterCooler.obj");

    d->initialized = true;
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
        d->_textures.emplace(std::string(path.data()),
                             std::move(ServiceLocator::instance().GetService<Fuego::Renderer::Renderer>()->CreateTexture(data, width, height)));
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
        ServiceLocator::instance().GetService<Fuego::Renderer::Renderer>()->Clear();
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
        ServiceLocator::instance().GetService<Fuego::Renderer::Renderer>()->Present();
    }
}

}  // namespace Fuego
