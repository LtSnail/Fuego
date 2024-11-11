#include "EventQueueWin.h"

namespace Fuego
{
void EventQueueWin::Update() {}

std::shared_ptr<const Event> EventQueueWin::Front() { return m_Queue.front(); }

void EventQueueWin::Pop() { m_Queue.pop(); }

bool EventQueueWin::Empty() { return m_Queue.empty(); }

void EventQueueWin::PushEvent(std::shared_ptr<Event>&& e)
{
    m_Queue.push(std::move(e));
}

std::unique_ptr<EventQueue> EventQueue::CreateEventQueue()
{
    return std::make_unique<EventQueueWin>();
}
}  // namespace Fuego
