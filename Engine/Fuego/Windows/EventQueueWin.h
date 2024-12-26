#pragma once

#include <mutex>

#include "EventQueue.h"
#include "Window.h"

namespace Fuego
{
class EventQueueWin : public EventQueue
{
    friend class WindowWin;

public:
    virtual void Update() override;
    virtual std::shared_ptr<EventVariant> Front() override;
    virtual void Pop() override;
    virtual bool Empty() override;

private:
    virtual void PushEvent(std::shared_ptr<EventVariant>&& e);

    std::queue<std::shared_ptr<EventVariant>> _queue;
    std::mutex _mutex;
};
}  // namespace Fuego
