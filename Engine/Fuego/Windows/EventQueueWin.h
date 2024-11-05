#pragma once

#include "EventQueue.h"

namespace Fuego
{
    class EventQueueWin : public EventQueue
    {
    public:
        virtual void Update() override;
        virtual std::shared_ptr<const Event> Front() override;
        virtual void Pop() override;
        virtual bool Empty() override;

    private:
        std::queue<Event> m_Queue;
    };
}
