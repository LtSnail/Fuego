#include "WindowMacOS.h"

@interface FuegoWindow : NSWindow
{
}
@end

@implementation FuegoWindow

@end

@interface FuegoView : NSView
- (BOOL)acceptsFirstResponder;
- (BOOL)isOpaque;

@end

@implementation FuegoView

- (void)_updateContentScale
{
    // TODO
}

- (void)scaleDidChange:(NSNotification*)n
{
    [self _updateContentScale];
}

- (void)viewDidMoveToWindow
{
    // Retina Display support
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(scaleDidChange:)
                                                 name:@"NSWindowDidChangeBackingPropertiesNotification"
                                               object:[self window]];

    // immediately update scale after the view has been added to a window
    [self _updateContentScale];
}

- (void)removeFromSuperview
{
    [super removeFromSuperview];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:@"NSWindowDidChangeBackingPropertiesNotification" object:[self window]];
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}

- (BOOL)isOpaque
{
    return YES;
}

- (void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

@end

namespace Fuego
{
WindowMacOS::WindowMacOS(const WindowProps& props, EventQueue& eventQueue)
{
    UNUSED(eventQueue);

    NSRect rect = NSMakeRect(props.x, props.y, props.Width, props.Height);
    NSWindowStyleMask styleMask = NSWindowStyleMaskTitled;
    if (props.Closable)
    {
        styleMask |= NSWindowStyleMaskClosable;
    }
    if (props.Resizable)
    {
        styleMask |= NSWindowStyleMaskResizable;
    }
    if (props.Minimizable)
    {
        styleMask |= NSWindowStyleMaskMiniaturizable;
    }
    if (!props.Frame)
    {
        styleMask |= NSWindowStyleMaskFullSizeContentView;
    }

    // Setup NSWindow
    FuegoWindow* w = [[FuegoWindow alloc] initWithContentRect:rect styleMask:styleMask backing:NSBackingStoreBuffered defer:NO];

    NSString* title = [NSString stringWithCString:props.Title.c_str() encoding:[NSString defaultCStringEncoding]];

    if (!props.Title.empty())
    {
        [w setTitle:(NSString*)title];
    }

    if (props.Centered)
    {
        [w center];
    }
    else
    {
        NSPoint point = NSMakePoint(props.x, props.y);
        point = [w convertPointToScreen:point];
        [w setFrameOrigin:point];
    }

    [w setHasShadow:props.HasShadow];
    [w setTitlebarAppearsTransparent:!props.Frame];

    // Setup NSView
    rect = [w backingAlignedRect:rect options:NSAlignAllEdgesOutward];
    FuegoView* v = [[FuegoView alloc] initWithFrame:rect];

    [v setHidden:NO];
    [v setNeedsDisplay:YES];
    [v setWantsLayer:YES];

    [w setContentView:v];
    [w makeKeyAndOrderFront:NSApp];

    _props = props;
    _window = (__bridge_retained void*)w;
    _view = (__bridge_retained void*)v;
}

WindowMacOS::~WindowMacOS()
{
    Shutdown();
}

void WindowMacOS::Update()
{
    @autoreleasepool
    {
        [NSApp updateWindows];
    }
}

const void* WindowMacOS::GetNativeHandle() const
{
    return _window;
}

void WindowMacOS::Shutdown()
{
    __unused id windowObj = (__bridge_transfer id)_window;
    __unused id viewObj = (__bridge_transfer id)_view;
}

std::unique_ptr<Window> Window::CreateAppWindow(const WindowProps& props, EventQueue& eventQueue)
{
    return std::make_unique<WindowMacOS>(props, eventQueue);
}
}  // namespace Fuego
