#include "WinEventQueue.h"

namespace Fuego
{
    void WinEventQueue::Update()
    {

    }

    std::shared_ptr<const Event> WinEventQueue::Front()
    {
        return nullptr;
    }

    void WinEventQueue::Pop()
    {
        m_Queue.pop();
    }

    bool WinEventQueue::Empty()
    {
        return m_Queue.empty();
    }

    std::unique_ptr<EventQueue> EventQueue::CreateEventQueue()
    {
        return std::make_unique<WinEventQueue>();
    }
}
