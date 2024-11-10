#include "EventQueueWin.h"

namespace Fuego
{
void EventQueueWin::Update() {}

const Event* EventQueueWin::Front() { return m_Queue.front().get(); }

void EventQueueWin::Pop() { m_Queue.pop(); }

bool EventQueueWin::Empty() { return m_Queue.empty(); }

void EventQueueWin::PushEvent(std::unique_ptr<Event> e)
{
    m_Queue.push(std::move(e));
}

std::unique_ptr<EventQueue> EventQueue::CreateEventQueue()
{
    return std::make_unique<EventQueueWin>();
}
}  // namespace Fuego
