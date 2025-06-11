#include "Application.h"

#include "ApplicationPipeline.hpp"
#include "Events/EventVisitor.h"
#include "FileSystem/FileSystem.h"
#include "KeyCodes.h"
#include "Renderer.h"
#include "Scene.h"
#include "ThreadPool.h"

namespace Fuego
{
using Texture = Fuego::Graphics::Texture;
using Image2D = Fuego::Graphics::Image2D;
using Model = Fuego::Graphics::Model;
using Renderer = Fuego::Graphics::Renderer;

template <>
Application& singleton<Application>::instance()
{
    static Application inst;
    return inst;
}

Application::Application() : initialized(false), m_Running(false) {}

Application::~Application() {}

void Application::PushLayer(Layer* layer)
{
    m_LayerStack.PushLayer(layer);
    layer->OnAttach();
}
void Application::PushOverlay(Layer* overlay)
{
    m_LayerStack.PushOverlay(overlay);
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

    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
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
    m_Running = false;
    event.SetHandled();
    return true;
}
bool Application::OnWindowResize(WindowResizeEvent& event)
{
    ServiceLocator::instance().GetService<Renderer>()->ChangeViewport(event.GetX(), event.GetY(), event.GetWidth(),
                                                                      event.GetHeight());
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
    ServiceLocator::instance().GetService<Renderer>()->ValidateWindow();
    m_Window->SetPainted();
    event.SetHandled();
    return true;
}
bool Application::OnKeyPressEvent(KeyPressedEvent& event)
{
    KeyCode crossplatform_key = event.GetKeyCode();

    switch (crossplatform_key)
    {
        case Key::D1:
            ServiceLocator::instance().GetService<Renderer>()->ToggleWireFrame();
            break;
        case Key::D2:
            m_Window->SwitchInteractionMode();
            break;
    }
    event.SetHandled();
    return true;
}

bool Application::OnRenderEvent(AppRenderEvent& event)
{
    auto renderer = ServiceLocator::instance().GetService<Renderer>();
    auto assets_manager = ServiceLocator::instance().GetService<Fuego::AssetsManager>();
    renderer->ShowWireFrame();
    // TODO: As for now we use just one opaque shader, but we must think about different passes
    // using different shaders with blending and probably using pre-passes
    renderer->SetShaderObject(renderer->opaque_shader.get());
    renderer->CurrentShaderObject()->Use();

    auto model_3 = assets_manager->Get<Model>("Sponza");
    // auto model_3 = assets_manager->Get<Model>("WaterCooler");
    auto locked_model_3 = model_3.lock();
    if (locked_model_3)
        renderer->DrawModel(locked_model_3.get(), glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 10.f)));

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
    return *m_Window;
}

void Application::Init(ApplicationBootSettings& settings)
{
    m_EventQueue = EventQueue::CreateEventQueue();
    m_Window = Window::CreateAppWindow(settings.window_props, *m_EventQueue);
    _time_manager = Time::CreateTimeManager(settings.fixed_dt);

    auto fs = ServiceLocator::instance().Register<Fuego::FS::FileSystem>();
    fs.value()->Init();

    Fuego::Pipeline::Toolchain toolchain{};
    toolchain._renderer.load_texture = Fuego::Pipeline::PostLoadPipeline::load_texture;
    toolchain._renderer.update = Fuego::Pipeline::PostLoadPipeline::update;
    Fuego::Pipeline::PostLoadPipeline::pairs_ptr = &Fuego::Pipeline::Toolchain::renderer::pairs;

    auto renderer = ServiceLocator::instance().Register<Renderer>(settings.renderer, toolchain._renderer);
    renderer.value()->Init();
    renderer.value()->SetVSync(settings.vsync);

    auto thread_pool = ServiceLocator::instance().Register<Fuego::ThreadPool>();
    thread_pool.value()->Init();

    auto assets_manager = ServiceLocator::instance().Register<Fuego::AssetsManager>(toolchain._assets_manager);

    auto resource = renderer.value()->CreateGraphicsResource<Texture>(
        assets_manager.value()->Load<Image2D>("fallback.png")->Resource());

    assets_manager.value()->Load<Model>("Sponza/Sponza.glb");
    // assets_manager.value()->Load<Model>("WaterCooler/WaterCooler.obj");

    initialized = true;
    m_Running = true;
}

void Application::SetVSync(bool active) const
{
    auto renderer = ServiceLocator::instance().GetService<Renderer>();
    renderer->SetVSync(active);
}

bool Application::IsVSync() const
{
    auto renderer = ServiceLocator::instance().GetService<Renderer>();
    return renderer->IsVSync();
}

void Application::Run()
{
    if (!initialized)
    {
        Application::ApplicationBootSettings settings{};
        Init(settings);
    }

    while (m_Running)
    {
        auto renderer = ServiceLocator::instance().GetService<Renderer>();
        auto assets_manager = ServiceLocator::instance().GetService<Fuego::AssetsManager>();

        _time_manager->Tick();

        char buffer[32];
        sprintf(buffer, "%d", _time_manager->FPS());
        m_Window->SetTitle(buffer);

        float dtTime = _time_manager->DeltaTime();

        renderer->Clear();
        m_EventQueue->OnUpdate(dtTime);
        m_Window->OnUpdate(dtTime);
        Fuego::Graphics::Camera::GetActiveCamera()->OnUpdate(dtTime);

        for (auto layer : m_LayerStack)
        {
            layer->OnUpdate(dtTime);
        }

        while (!m_EventQueue->Empty())
        {
            auto ev = m_EventQueue->Front();
            OnEvent(*ev);
            m_EventQueue->Pop();
        }
        renderer->OnUpdate(dtTime);
        renderer->Present();
    }
}

}  // namespace Fuego
