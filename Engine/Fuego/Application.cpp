#include "Application.h"

#include "Events/EventVisitor.h"
#include "FileSystem/FileSystem.h"
#include "FuTime.h"
#include "KeyCodes.h"
#include "LayerStack.h"
#include "Renderer.h"
#include "Scene.h"
#include "ThreadPool.h"

namespace Fuego
{

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
    std::unique_ptr<Fuego::Time> _time_manager;

    RendererType renderer;
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
    ServiceLocator::instance().GetService<Fuego::Graphics::Renderer>()->ChangeViewport(event.GetX(), event.GetY(), event.GetWidth(), event.GetHeight());
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
    ServiceLocator::instance().GetService<Fuego::Graphics::Renderer>()->ValidateWindow();
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
        ServiceLocator::instance().GetService<Fuego::Graphics::Renderer>()->ToggleWireFrame();
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
    auto renderer = ServiceLocator::instance().GetService<Fuego::Graphics::Renderer>();
    auto assets_manager = ServiceLocator::instance().GetService<Fuego::AssetsManager>();
    renderer->ShowWireFrame();
    // TODO: As for now we use just one opaque shader, but we must think about different passes
    // using different shaders with blending and probably using pre-passes
    renderer->SetShaderObject(renderer->opaque_shader.get());
    renderer->CurrentShaderObject()->Use();

    auto model_1 = assets_manager->Get<Fuego::Graphics::Model>("Shotgun");
    auto locked_model_1 = model_1.lock();
    if (locked_model_1)
        renderer->DrawModel(locked_model_1.get(), glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.f)));

    auto model_2 = assets_manager->Get<Fuego::Graphics::Model>("WaterCooler");
    auto locked_model_2 = model_2.lock();
    if (locked_model_2)
        renderer->DrawModel(locked_model_2.get(), glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 10.f)));

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

void Application::Init(ApplicationBootSettings& settings)
{
    d->renderer = settings.renderer;
    d->m_EventQueue = EventQueue::CreateEventQueue();
    d->m_Window = Window::CreateAppWindow(settings.window_props, *d->m_EventQueue);
    d->_time_manager = Time::CreateTimeManager(settings.fixed_dt);

    auto fs = ServiceLocator::instance().Register<Fuego::FS::FileSystem>();
    fs.value()->Init();

    auto renderer = ServiceLocator::instance().Register<Fuego::Graphics::Renderer>();
    renderer.value()->Init();
    SetVSync(settings.vsync);

    auto thread_pool = ServiceLocator::instance().Register<Fuego::ThreadPool>();
    thread_pool.value()->Init();

    auto assets_manager = ServiceLocator::instance().Register<Fuego::AssetsManager>();
    auto fallback_img = assets_manager.value()->Load<Fuego::Graphics::Image2D>("fallback.png", Fuego::Graphics::ImageFormat::RGB);
    renderer.value()->CreateTexture(*fallback_img);
    std::chrono::steady_clock::time_point timer = std::chrono::steady_clock::now();
    assets_manager.value()->LoadAsync<Fuego::Graphics::Model>("Shotgun/Shotgun.obj");
    assets_manager.value()->LoadAsync<Fuego::Graphics::Model>("WaterCooler/WaterCooler.obj");
    auto now = std::chrono::steady_clock::now();
    auto res = now - timer;
    FU_CORE_INFO("time: {0} ms", std::chrono::duration_cast<std::chrono::milliseconds>(res).count());

    // assets_manager.value()->Unload<Fuego::Graphics::Model>("WaterCooler/WaterCooler.obj");

    d->initialized = true;
    d->m_Running = true;
}

void Application::SetVSync(bool active) const
{
    auto renderer = ServiceLocator::instance().Register<Fuego::Graphics::Renderer>();
    renderer.value()->SetVSync(active);
}

bool Application::IsVSync() const
{
    auto renderer = ServiceLocator::instance().Register<Fuego::Graphics::Renderer>();
    return renderer.value()->IsVSync();
}

void Application::Run()
{
    if (!d->initialized)
    {
        Application::ApplicationBootSettings settings{};
        Init(settings);
    }

    while (d->m_Running)
    {
        auto renderer = ServiceLocator::instance().GetService<Fuego::Graphics::Renderer>();

        d->_time_manager->Tick();

        char buffer[32];
        sprintf(buffer, "%d", d->_time_manager->FPS());
        d->m_Window->SetTitle(buffer);

        float dtTime = d->_time_manager->DeltaTime();


        renderer->Clear();
        d->m_EventQueue->OnUpdate(dtTime);
        d->m_Window->OnUpdate(dtTime);
        Fuego::Graphics::Camera::GetActiveCamera()->OnUpdate(dtTime);

        for (auto layer : d->m_LayerStack)
        {
            layer->OnUpdate(dtTime);
        }

        while (!d->m_EventQueue->Empty())
        {
            auto ev = d->m_EventQueue->Front();
            OnEvent(*ev);
            d->m_EventQueue->Pop();
        }

        renderer->OnUpdate(dtTime);
        renderer->Present();
    }
}

}  // namespace Fuego
