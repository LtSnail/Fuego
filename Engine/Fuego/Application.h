#pragma once

#include "EventQueue.h"
#include "Layer.h"
#include "LayerStack.h"
#include "Window.h"
#include "Events/ApplicationEvent.h"


namespace Fuego
{
class Application
{
   public:
    FUEGO_API Application();
    FUEGO_API virtual ~Application() = default;
    FUEGO_NON_COPYABLE_NON_MOVABLE(Application)

    FUEGO_API void Run();

    FUEGO_API void PushLayer(Layer* layer);
    FUEGO_API void PushOverlay(Layer* overlay);

    void OnEvent(Event& event);
    bool OnWindowClose(WindowCloseEvent& event);
    bool OnWindowResize(WindowResizeEvent& event);

   private:
    std::unique_ptr<Window> m_Window;
    std::unique_ptr<EventQueue> m_EventQueue;
    LayerStack m_LayerStack;

    bool m_Running;
};

// Should be defined in a client.
Application* CreateApplication();
}  // namespace Fuego
