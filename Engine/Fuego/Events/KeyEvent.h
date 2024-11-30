#pragma once

#include "Event.h"

namespace Fuego
{
class FUEGO_API KeyEvent
{
public:
    inline int GetKeyCode() const
    {
        return _keyCode;
    }

protected:
    explicit KeyEvent(int keycode)
        : _keyCode(keycode)
    {
    }

    int _keyCode;
};

class FUEGO_API KeyPressedEvent final : public Event, KeyEvent
{
public:
    KeyPressedEvent(int keycode, int repeatCount)
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
    KeyReleasedEvent(int keycode)
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
