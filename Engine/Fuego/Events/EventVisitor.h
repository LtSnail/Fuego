#pragma once

#include <variant>

#include "Events/ApplicationEvent.h"
#include "Events/Event.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"


namespace Fuego
{
using EventVariant = std::variant<Event, WindowResizeEvent, WindowCloseEvent, AppTickEvent, AppUpdateEvent, AppRenderEvent, KeyPressedEvent, KeyReleasedEvent,
                                  MouseMovedEvent, MouseScrolledEvent, MouseButtonPressedEvent, MouseButtonReleasedEvent>;

template <class... Ts>
struct EventVisitor : Ts...
{
    using Ts::operator()...;
};

// Deduction guide
template <class... Ts>
EventVisitor(Ts...) -> EventVisitor<Ts...>;

}  // namespace Fuego
