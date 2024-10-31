#include "WinEventQueue.h"

namespace Fuego
{
    void WindowsEventQueue::Update()
    {

    }

    std::shared_ptr<const Event> WindowsEventQueue::Front()
    {
        return nullptr;
    }

    void WindowsEventQueue::Pop()
    {
        m_Queue.pop();
    }

    bool WindowsEventQueue::Empty()
    {
        return m_Queue.empty();
    }

    std::unique_ptr<EventQueue> EventQueue::CreateEventQueue()
    {
        return std::make_unique<WindowsEventQueue>();
    }
}
