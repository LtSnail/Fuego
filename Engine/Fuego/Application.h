#pragma once

#include "chpch.h"

#include "Core.h"
#include "EventQueue.h"
#include "Window.h"

namespace Fuego
{
    class Application
    {
    public:
       FUEGO_API Application();
       FUEGO_API virtual ~Application();

       FUEGO_API void Run();

    private:
        std::unique_ptr<Window> m_Window;
        std::unique_ptr<EventQueue> m_EventQueue;
    };

    // Should be defined in a client.
    Application* CreateApplication();
}
