#include "Application.h"

#include "Log.h"

namespace Fuego
{
class Application::ApplicationImpl
{
    friend class Application;

    std::unique_ptr<Window> m_Window;
    std::unique_ptr<EventQueue> m_EventQueue;
    bool m_Running;
    LayerStack m_LayerStack;
};

Application::Application() : d(new ApplicationImpl())
{
    d->m_EventQueue = EventQueue::CreateEventQueue();
    d->m_Window = Window::CreateAppWindow(WindowProps(), *d->m_EventQueue);
    d->m_Running = true;
}

Application::~Application() { delete d; }

void Application::PushLayer(Layer* layer) { d->m_LayerStack.PushLayer(layer); }

void Application::PushOverlay(Layer* overlay)
{
    d->m_LayerStack.PushOverlay(overlay);
}

void Application::OnEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowCloseEvent>(
        std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
    dispatcher.Dispatch<WindowResizeEvent>(
        std::bind(&Application::OnWindowResize, this, std::placeholders::_1));

    FU_CORE_TRACE("{0}", event.ToString());

    for (auto it = d->m_LayerStack.end(); it != d->m_LayerStack.begin();)
    {
        (*--it)->OnEvent(event);
        if (event.Handled())
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
