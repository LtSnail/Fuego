#include "EventQueueWin.h"

namespace Fuego
{
void EventQueueWin::Update()
{
}

std::shared_ptr<EventVariant> EventQueueWin::Front()
{
    return _queue.front();
}

void EventQueueWin::Pop()
{
    _queue.pop();
}

bool EventQueueWin::Empty()
{
    return _queue.empty();
}

void EventQueueWin::PushEvent(std::shared_ptr<EventVariant>&& e)
{
    _queue.push(std::move(e));
}

std::unique_ptr<EventQueue> EventQueue::CreateEventQueue()
{
    return std::make_unique<EventQueueWin>();
}
}  // namespace Fuego
