#pragma once

#include <mutex>

#include "EventQueue.h"

namespace Fuego
{
class EventQueueWin final : public EventQueue
{
    friend class WindowWin;

public:
    virtual void OnUpdate(float dlTime) override;
    virtual void OnPostUpdate(float dlTime) override;
    virtual void OnFixedUpdate() override;

    virtual std::shared_ptr<EventVariant> Front() override;
    virtual void Pop() override;
    virtual bool Empty() override;

private:
    virtual void PushEvent(std::shared_ptr<EventVariant>&& e);

    std::queue<std::shared_ptr<EventVariant>> _queue;
    std::mutex _mutex;
};
}  // namespace Fuego
