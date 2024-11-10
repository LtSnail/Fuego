#pragma once

#include "EventQueue.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Window.h"

namespace Fuego
{
class EventQueueWin : public EventQueue
{
   public:
    virtual void Update() override;
    virtual const Event* Front() override;
    virtual void Pop() override;
    virtual bool Empty() override;
    void PushEvent(std::unique_ptr<Event> e);

   private:
    std::queue<std::unique_ptr<Event>> m_Queue;
};
}  // namespace Fuego
