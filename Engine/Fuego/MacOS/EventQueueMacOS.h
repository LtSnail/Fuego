#include "EventQueue.h"
#include "fupch.h"

namespace Fuego
{
class FUEGO_API EventQueueMacOS : public EventQueue
{
   public:
    EventQueueMacOS();
    ~EventQueueMacOS();

    virtual void Update() override;
    virtual std::shared_ptr<const Event> Front() override;
    virtual void Pop() override;
    virtual bool Empty() override;

   private:
    friend void PushEvent(EventQueueMacOS* eq, std::shared_ptr<const Event> ev);
    std::queue<std::shared_ptr<const Event>> m_Queue;

    // WindowEventsObserver*
    void* m_WindowEventsObserver;
};
}  // namespace Fuego
