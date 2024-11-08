#pragma once

#include "Core.h"
#include "EventQueue.h"
#include "Window.h"
#include "fupch.h"

namespace Fuego
{
class Application
{
   public:
    FUEGO_API Application();
    FUEGO_API virtual ~Application() = default;
    FUEGO_NON_COPYABLE_NON_MOVABLE(Application)

    FUEGO_API void Run();

   private:
    std::unique_ptr<Window> m_Window;
    std::unique_ptr<EventQueue> m_EventQueue;
};

// Should be defined in a client.
Application* CreateApplication();
}  // namespace Fuego
