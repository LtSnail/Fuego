#include "EventQueueWin.h"

namespace Fuego
{
void EventQueueWin::OnUpdate(float dtTime)
{
    // TODO
}

void EventQueueWin::OnPostUpdate(float dlTime)
{
    // TODO
}

void EventQueueWin::OnFixedUpdate()
{
    // TODO
}

std::shared_ptr<EventVariant> EventQueueWin::Front()
{
    std::lock_guard lock(_mutex);
    return _queue.front();
}

void EventQueueWin::Pop()
{
    std::lock_guard lock(_mutex);
    _queue.pop();
}

bool EventQueueWin::Empty()
{
    return _queue.empty();
}

void EventQueueWin::PushEvent(std::shared_ptr<EventVariant>&& e)
{
    std::lock_guard lock(_mutex);
    _queue.push(std::move(e));
}

std::unique_ptr<EventQueue> EventQueue::CreateEventQueue()
{
    return std::make_unique<EventQueueWin>();
}
}  // namespace Fuego
