#include "EventQueueWin.h"

namespace Fuego
{
    void EventQueueWin::Update()
    {

    }

    std::shared_ptr<const Event> EventQueueWin::Front()
    {
        return nullptr;
    }

    void EventQueueWin::Pop()
    {
        m_Queue.pop();
    }

    bool EventQueueWin::Empty()
    {
        return m_Queue.empty();
    }

    std::unique_ptr<EventQueue> EventQueue::CreateEventQueue()
    {
        return std::make_unique<EventQueueWin>();
    }
}
