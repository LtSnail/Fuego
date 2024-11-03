#include "Application.h"

#include "Log.h"

namespace Fuego
{
    Application::Application()
    {
        m_EventQueue = EventQueue::CreateEventQueue();
        m_Window = Window::CreateAppWindow(WindowProps(), *m_EventQueue);
    }

    void Application::Run()
    {
        bool isRunning = true;

        while (isRunning)
        {
            m_EventQueue->Update();
            m_Window->Update();
            
            while (!m_EventQueue->Empty())
            {
                auto ev = m_EventQueue->Front();
                
                switch (ev->GetEventType()) {
                    case EventType::EventTypeWindowClose:
                        isRunning = false;
                        break;
                    case EventType::EventTypeWindowResize:
                        FU_CORE_TRACE("{0}", ev->ToString());
                        break;
                    case EventType::EventTypeWindowFocus:
                        //
                        break;
                    case EventType::EventTypeWindowLostFocus:
                        //
                        break;
                    case EventType::EventTypeWindowMoved:
                        //
                        break;
                    case EventType::EventTypeAppTick:
                        //
                        break;
                    case EventType::EventTypeAppUpdate:
                        //
                        break;
                    case EventType::EventTypeAppRender:
                        //
                        break;
                    case EventType::EventTypeKeyPressed:
                        //
                        break;
                    case EventType::EventTypeKeyReleased:
                        //
                        break;
                    case EventType::EventTypeMouseButtonPressed:
                        //
                        break;
                    case EventType::EventTypeMouseButtonReleased:
                        //
                        break;
                    case EventType::EventTypeMouseMoved:
                        //
                        break;
                    case EventType::EventTypeMouseScrolled:
                        //
                        break;
                    case EventType::None:
                        break;
                }

                m_EventQueue->Pop();
            }
        }
    }
}
