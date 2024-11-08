#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace Fuego
{
class FUEGO_API EventQueue
{
   public:
    EventQueue() = default;
    FUEGO_NON_COPYABLE_NON_MOVABLE(EventQueue)

    virtual void Update() = 0;
    virtual std::shared_ptr<const Event> Front() = 0;
    virtual void Pop() = 0;
    virtual bool Empty() = 0;

    virtual ~EventQueue() = default;

    static std::unique_ptr<EventQueue> CreateEventQueue();
};
}  // namespace Fuego
