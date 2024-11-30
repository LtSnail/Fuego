#include "EventQueue.h"

namespace Fuego
{
class FUEGO_API EventQueueMacOS : public EventQueue
{
public:
    EventQueueMacOS();
    ~EventQueueMacOS();

    virtual void Update() override;
    virtual std::shared_ptr<EventVariant> Front() override;
    virtual void Pop() override;
    virtual bool Empty() override;

private:
    friend void PushEvent(EventQueueMacOS* eq, std::shared_ptr<EventVariant> ev);
    std::queue<std::shared_ptr<EventVariant>> _queue;

    // WindowEventsObserver*
    void* _windowEventsObserver;
};
}  // namespace Fuego
