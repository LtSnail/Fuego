#pragma once

#include "EventQueue.h"
#include "Events/ApplicationEvent.h"
#include "Layer.h"
#include "LayerStack.h"
#include "Window.h"

namespace Fuego
{
class FUEGO_API Application
{
    FUEGO_INTERFACE(Application);

   public:
    Application();
    virtual ~Application();
    FUEGO_NON_COPYABLE_NON_MOVABLE(Application)

    void Run();

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);

    void OnEvent(Event& event);
    bool OnWindowClose(WindowCloseEvent& event);
    bool OnWindowResize(WindowResizeEvent& event);
};

// Should be defined in a client.
Application* CreateApplication();
}  // namespace Fuego
