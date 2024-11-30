#include "Application.h"

#include "Events/EventVisitor.h"
#include "Input.h"
#include "Renderer/Buffer.h"

namespace Fuego
{
class Application::ApplicationImpl
{
    friend class Application;

    std::unique_ptr<Window> m_Window;
    std::unique_ptr<EventQueue> m_EventQueue;
    bool m_Running;
    LayerStack m_LayerStack;
    static Application* m_Instance;
};
Application* Application::ApplicationImpl::m_Instance = nullptr;

Application::Application()
    : d(new ApplicationImpl())
{
    ApplicationImpl::m_Instance = this;
    d->m_EventQueue = EventQueue::CreateEventQueue();
    d->m_Window = Window::CreateAppWindow(WindowProps(), *d->m_EventQueue);
    d->m_Running = true;
}

Application::~Application()
{
    delete d;
}

void Application::PushLayer(Layer* layer)
{
    d->m_LayerStack.PushLayer(layer);
}

void Application::PushOverlay(Layer* overlay)
{
    d->m_LayerStack.PushOverlay(overlay);
}

void Application::OnEvent(EventVariant& event)
{
    auto ApplicationEventVisitor =
        EventVisitor{[this](WindowCloseEvent& ev) { OnWindowClose(ev); }, [this](WindowResizeEvent& ev) { OnWindowResize(ev); }, [](Event&) {}};

    std::visit(ApplicationEventVisitor, event);

    for (auto it = d->m_LayerStack.end(); it != d->m_LayerStack.begin();)
    {
        (*--it)->OnEvent(event);

        auto HandledEventVisitor = EventVisitor{[](const Event& ev) -> bool
                                                {
                                                    FU_CORE_TRACE("{0}", ev.ToString());
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
    UNUSED(event);

    d->m_Running = false;
    return true;
}

bool Application::OnWindowResize(WindowResizeEvent& event)
{
    FU_CORE_TRACE("{0}", event.ToString());
    return true;
}

Application& Application::Get()
{
    return *Application::ApplicationImpl::m_Instance;
}

Window& Application::GetWindow()
{
    return *d->m_Window;
}

void Application::Run()
{
    while (d->m_Running)
    {
        d->m_EventQueue->Update();
        d->m_Window->Update();

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
    }
}
}  // namespace Fuego
