#include "EventQueueMacOS.h"

#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"
#include "Log.h"
#include "MouseCodes.h"

void Fuego::PushEvent(EventQueueMacOS* eq, std::shared_ptr<const Event> ev)
{
    eq->m_Queue.push(ev);
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
    Fuego::PushEvent(_eq, std::make_shared<const Fuego::WindowCloseEvent>());
}

- (void)_windowDidResize:(NSNotification*)notification
{
    NSWindow* window = (NSWindow*)[notification object];
    NSRect rect = [window contentRectForFrameRect:[window frame]];
    Fuego::PushEvent(
        _eq, std::make_shared<const Fuego::WindowResizeEvent>(rect.size.width, rect.size.height));
}

- (instancetype)initWithEventQueue:(Fuego::EventQueueMacOS*)eq
{
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(_windowWillClose:)
                                                 name:NSWindowWillCloseNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(_windowDidResize:)
                                                 name:NSWindowDidResizeNotification
                                               object:nil];

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
    WindowEventsObserver* observerObj = [[WindowEventsObserver alloc] initWithEventQueue:this];
    m_WindowEventsObserver = ((__bridge_retained void*)observerObj);
}

EventQueueMacOS::~EventQueueMacOS()
{
    // Return ownership to ARC
    __unused id observerObj = (__bridge_transfer id)m_WindowEventsObserver;
}

void EventQueueMacOS::Update()
{
    @autoreleasepool
    {
        NSEvent* nsEvent = nil;

        do
        {
            nsEvent = [NSApp nextEventMatchingMask:NSEventMaskAny
                                         untilDate:nil
                                            inMode:NSDefaultRunLoopMode
                                           dequeue:YES];

            switch (nsEvent.type)
            {
                case NSEventTypeSystemDefined:
                    break;
                case NSEventTypeKeyDown:
                    break;
                case NSEventTypeKeyUp:
                    break;
                case NSEventTypeLeftMouseDown:
                    PushEvent(this,
                              std::make_shared<const MouseButtonPressedEvent>(Mouse::ButtonLeft));
                    break;
                case NSEventTypeLeftMouseUp:
                    PushEvent(this,
                              std::make_shared<const MouseButtonReleasedEvent>(Mouse::ButtonLeft));
                    break;
                case NSEventTypeRightMouseDown:
                    PushEvent(this,
                              std::make_shared<const MouseButtonPressedEvent>(Mouse::ButtonRight));
                    break;
                case NSEventTypeRightMouseUp:
                    PushEvent(this,
                              std::make_shared<const MouseButtonPressedEvent>(Mouse::ButtonRight));
                    break;
                case NSEventTypeMouseMoved:
                    PushEvent(this,
                              std::make_shared<const MouseMovedEvent>(nsEvent.locationInWindow.x,
                                                                      nsEvent.locationInWindow.y));
                    break;
                case NSEventTypeScrollWheel:
                    PushEvent(this,
                              std::make_shared<const MouseScrolledEvent>(nsEvent.scrollingDeltaX,
                                                                         nsEvent.scrollingDeltaY));
                    break;
                default:
                    break;
            }

            [NSApp sendEvent:nsEvent];
        } while (nsEvent);
    }
}

std::shared_ptr<const Event> EventQueueMacOS::Front() { return m_Queue.front(); }

void EventQueueMacOS::Pop() { m_Queue.pop(); }

bool EventQueueMacOS::Empty() { return m_Queue.empty(); }

std::unique_ptr<EventQueue> EventQueue::CreateEventQueue()
{
    return std::make_unique<EventQueueMacOS>();
}
}
