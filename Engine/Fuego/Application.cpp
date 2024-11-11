#include "Application.h"

#include "Log.h"

namespace Fuego
{
Application::Application()
{
    m_EventQueue = EventQueue::CreateEventQueue();
    m_Window = Window::CreateAppWindow(WindowProps(), *m_EventQueue);
    m_Running = true;
}

void Application::PushLayer(Layer* layer)
{
    m_LayerStack.PushLayer(layer);
}

void Application::PushOverlay(Layer* overlay)
{
    m_LayerStack.PushOverlay(overlay);
}

void Application::OnEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowCloseEvent>(
        std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
    dispatcher.Dispatch<WindowResizeEvent>(
        std::bind(&Application::OnWindowResize, this, std::placeholders::_1));

    FU_CORE_TRACE("{0}", event.ToString());

    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
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

    m_Running = false;
    return true;
}

bool Application::OnWindowResize(WindowResizeEvent& event)
{
    FU_CORE_TRACE("{0}", event.ToString());
    return true;
}

void Application::Run()
{
    while (m_Running)
    {
        m_EventQueue->Update();
        m_Window->Update();

        for (auto layer : m_LayerStack)
        {
            layer->OnUpdate();
        }

        while (!m_EventQueue->Empty())
        {
            auto ev = m_EventQueue->Front();
            OnEvent(*ev);

            m_EventQueue->Pop();
        }
    }
}
}  // namespace Fuego
