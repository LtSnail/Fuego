#pragma once

#include <cstdint>

#include "Event.h"
#include "MouseCodes.h"

namespace Fuego
{
class FUEGO_API MouseMovedEvent final : public Event
{
public:
    MouseMovedEvent(float x, float y)
        : Event(EVENT_NAME(MouseMovedEvent))
        , _mouseX(x)
        , _mouseY(y)
    {
    }

    inline float GetX() const
    {
        return _mouseX;
    }
    inline float GetY() const
    {
        return _mouseY;
    }

    inline std::string ToString() const override
    {
        std::stringstream ss;
        ss << _name << ": " << _mouseX << ", " << _mouseY;
        return ss.str();
    }

private:
    float _mouseX, _mouseY;
};

class FUEGO_API MouseScrolledEvent final : public Event
{
public:
    MouseScrolledEvent(float xOffset, float yOffset)
        : Event(EVENT_NAME(MouseScrolledEvent))
        , _offsetX(xOffset)
        , _offsetY(yOffset)
    {
    }

    inline float GetXOffset() const
    {
        return _offsetX;
    }
    inline float GetYOffset() const
    {
        return _offsetY;
    }

    inline std::string ToString() const override
    {
        std::stringstream ss;
        ss << _name << ": " << _offsetX << ", " << _offsetY;
        return ss.str();
    }

private:
    float _offsetX, _offsetY;
};

class FUEGO_API MouseButtonEvent
{
public:
    inline int GetMouseButton() const
    {
        return _button;
    }

protected:
    explicit MouseButtonEvent(MouseCode button)
        : _button(button)
    {
    }

    uint16_t _button;
};

class FUEGO_API MouseButtonPressedEvent final : public Event, MouseButtonEvent
{
public:
    MouseButtonPressedEvent(MouseCode button)
        : Event(EVENT_NAME(MouseButtonPressedEvent))
        , MouseButtonEvent(button)
    {
    }

    inline std::string ToString() const override
    {
        std::stringstream ss;
        ss << _name << ": " << Mouse::GetMouseButtonName(_button);
        return ss.str();
    }
};

class FUEGO_API MouseButtonReleasedEvent final : public Event, MouseButtonEvent
{
public:
    MouseButtonReleasedEvent(MouseCode button)
        : Event(EVENT_NAME(MouseButtonReleasedEvent))
        , MouseButtonEvent(button)
    {
    }

    inline std::string ToString() const override
    {
        std::stringstream ss;
        ss << _name << ": " << Mouse::GetMouseButtonName(_button);
        return ss.str();
    }
};
}  // namespace Fuego
