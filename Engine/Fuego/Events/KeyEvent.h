#pragma once

#include "Event.h"
#include "KeyCodes.h"

namespace Fuego
{
class FUEGO_API KeyEvent
{
public:
    inline KeyCode GetKeyCode() const
    {
        return _keyCode;
    }

protected:
    explicit KeyEvent(KeyCode keycode)
        : _keyCode(keycode)
    {
    }

    KeyCode _keyCode;
};

class FUEGO_API KeyPressedEvent final : public Event, KeyEvent
{
public:
    KeyPressedEvent(KeyCode keycode, int repeatCount)
        : Event(EVENT_NAME(KeyPressedEvent))
        , KeyEvent(keycode)
        , _repeatCount(repeatCount)
    {
        _name = EVENT_NAME(KeyPressedEvent);
    }

    inline int GetRepeatCount() const
    {
        return _repeatCount;
    }

    inline std::string ToString() const override
    {
        std::stringstream ss;
        ss << _name << ": " << _keyCode << " (" << _repeatCount << " repeats)";
        return ss.str();
    }

private:
    int _repeatCount;
};

class FUEGO_API KeyReleasedEvent final : public Event, KeyEvent
{
public:
    KeyReleasedEvent(KeyCode keycode)
        : Event(EVENT_NAME(KeyReleasedEvent))
        , KeyEvent(keycode)
    {
    }

    inline std::string ToString() const override
    {
        std::stringstream ss;
        ss << _name << ": " << _keyCode;
        return ss.str();
    }
};
}  // namespace Fuego
