#pragma once

#include "EventQueue.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Window.h"

namespace Fuego
{
class EventQueueWin : public EventQueue
{
    friend class WindowWin;

   public:
    virtual void Update() override;
    virtual std::shared_ptr<Event> Front() override;
    virtual void Pop() override;
    virtual bool Empty() override;

   private:
    virtual void PushEvent(std::shared_ptr<Event>&& e);

    std::queue<std::shared_ptr<Event>> m_Queue;
};
}  // namespace Fuego
