#pragma once

#include <type_traits>

#include "Core.h"
#include "fupch.h"

namespace Fuego
{
// Blocking events, refactor to an event bus later on.

enum class EventType
{
    None = 0,

    // Window
    EventTypeWindowClose,
    EventTypeWindowResize,
    EventTypeWindowFocus,
    EventTypeWindowLostFocus,
    EventTypeWindowMoved,

    // Application
    EventTypeAppTick,
    EventTypeAppUpdate,
    EventTypeAppRender,

    // Input
    EventTypeKeyPressed,
    EventTypeKeyReleased,
    EventTypeMouseButtonPressed,
    EventTypeMouseButtonReleased,
    EventTypeMouseMoved,
    EventTypeMouseScrolled
};

enum EventCategory
{
    None = 0,
    EventCategoryApplication = BIT(0),
    EventCategoryInput = BIT(1),
    EventCategoryKeyboard = BIT(2),
    EventCategoryMouse = BIT(3),
    EventCategoryMouseButton = BIT(4)
};

#define EVENT_CLASS_TYPE(type)                                              \
    static EventType GetStaticType() { return EventType::EventType##type; } \
    EventType GetEventType() const override { return GetStaticType(); }     \
    const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) \
    int GetCategoryFlags() const override { return category; }

class FUEGO_API Event
{
    friend class EventDispatcher;

   public:
    Event() = default;
    virtual EventType GetEventType() const = 0;
    virtual const char* GetName() const = 0;
    virtual int GetCategoryFlags() const = 0;
    virtual std::string ToString() const { return GetName(); }
    virtual bool Handled() const { return m_handled; }

    inline bool IsInCategory(EventCategory category) const
    {
        return GetCategoryFlags() & category;
    }

    virtual ~Event() = default;

   protected:
    bool m_handled = false;
};

template <typename T>
concept DerivedFromEvent = std::is_base_of_v<Event, T>;

class EventDispatcher
{
    template <typename T>
    using EventFn = std::function<bool(T&)>;

   public:
    EventDispatcher(Event& ev) : m_Event(ev) {}

    template <DerivedFromEvent T>
    bool Dispatch(EventFn<T> func)
    {
        if (m_Event.GetEventType() == T::GetStaticType())
        {
            m_Event.m_handled = func(static_cast<T&>(m_Event));
            return true;
        }

        return false;
    }

   private:
    Event& m_Event;
};

inline std::ostream& operator<<(std::ostream& os, const Event& ev)
{
    return os << ev.ToString();
}
}  // namespace Fuego
