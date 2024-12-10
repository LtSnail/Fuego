#include "EventQueueMacOS.h"

#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "KeyCodesMacOS.h"
#include "Log.h"
#include "MouseCodes.h"

void Fuego::PushEvent(EventQueueMacOS* eq, std::shared_ptr<EventVariant> ev)
{
    eq->_queue.push(ev);
}

@interface WindowEventsObserver : NSObject
{
}
@end

@implementation WindowEventsObserver
{
    Fuego::EventQueueMacOS* _eq;
}

- (void)_windowWillClose:(NSNotification*)notification
{
    Fuego::PushEvent(_eq, std::make_shared<Fuego::EventVariant>(Fuego::WindowCloseEvent()));
}

- (void)_windowDidResize:(NSNotification*)notification
{
    NSWindow* window = (NSWindow*)[notification object];
    NSRect rect = [window contentRectForFrameRect:[window frame]];
    Fuego::PushEvent(_eq, std::make_shared<Fuego::EventVariant>(Fuego::WindowResizeEvent(rect.size.width, rect.size.height)));
}

- (instancetype)initWithEventQueue:(Fuego::EventQueueMacOS*)eq
{
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(_windowWillClose:) name:NSWindowWillCloseNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(_windowDidResize:) name:NSWindowDidResizeNotification object:nil];

    self = [super init];
    if (self)
    {
        _eq = eq;
    }
    return self;
}

@end

namespace Fuego
{
EventQueueMacOS::EventQueueMacOS()
{
    _windowEventsObserver = (__bridge_retained void*)[[WindowEventsObserver alloc] initWithEventQueue:this];
}

EventQueueMacOS::~EventQueueMacOS()
{
    __unused id obj = (__bridge_transfer WindowEventsObserver*)_windowEventsObserver;
}

void EventQueueMacOS::Update()
{
    @autoreleasepool
    {
        NSEvent* nsEvent = nil;

        do
        {
            nsEvent = [NSApp nextEventMatchingMask:NSEventMaskAny untilDate:nil inMode:NSDefaultRunLoopMode dequeue:YES];

            switch (nsEvent.type)
            {
            case NSEventTypeSystemDefined:
                break;
            case NSEventTypeKeyDown:
                PushEvent(this, std::make_shared<EventVariant>(KeyPressedEvent(nsEvent.keyCode, 0)));
                break;
            case NSEventTypeKeyUp:
                PushEvent(this, std::make_shared<EventVariant>(KeyReleasedEvent(nsEvent.keyCode)));
                break;
            case NSEventTypeLeftMouseDown:
                PushEvent(this, std::make_shared<EventVariant>(MouseButtonPressedEvent(Mouse::ButtonLeft)));
                break;
            case NSEventTypeLeftMouseUp:
                PushEvent(this, std::make_shared<EventVariant>(MouseButtonReleasedEvent(Mouse::ButtonLeft)));
                break;
            case NSEventTypeRightMouseDown:
                PushEvent(this, std::make_shared<EventVariant>(MouseButtonPressedEvent(Mouse::ButtonRight)));
                break;
            case NSEventTypeRightMouseUp:
                PushEvent(this, std::make_shared<EventVariant>(MouseButtonPressedEvent(Mouse::ButtonRight)));
                break;
            case NSEventTypeMouseMoved:
                PushEvent(this, std::make_shared<EventVariant>(MouseMovedEvent(nsEvent.locationInWindow.x, nsEvent.locationInWindow.y)));
                break;
            case NSEventTypeScrollWheel:
                PushEvent(this, std::make_shared<EventVariant>(MouseScrolledEvent(nsEvent.scrollingDeltaX, nsEvent.scrollingDeltaY)));
                break;
            default:
                break;
            }

            [NSApp sendEvent:nsEvent];
        } while (nsEvent);
    }
}

std::shared_ptr<EventVariant> EventQueueMacOS::Front()
{
    return _queue.front();
}

void EventQueueMacOS::Pop()
{
    _queue.pop();
}

bool EventQueueMacOS::Empty()
{
    return _queue.empty();
}

std::unique_ptr<EventQueue> EventQueue::CreateEventQueue()
{
    return std::make_unique<EventQueueMacOS>();
}
}  // namespace Fuego
