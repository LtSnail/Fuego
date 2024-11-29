#pragma once

#include <cstdint>

#include "Event.h"
#include "MouseCodes.h"

namespace Fuego
{
class FUEGO_API MouseMovedEvent : public Event
{
public:
    MouseMovedEvent(float x, float y)
        : m_MouseX(x)
        , m_MouseY(y)
    {
    }

    float GetX() const
    {
        return m_MouseX;
    }
    float GetY() const
    {
        return m_MouseY;
    }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << GetName() << ": " << m_MouseX << ", " << m_MouseY;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseMoved)

private:
    float m_MouseX, m_MouseY;
};

class FUEGO_API MouseScrolledEvent : public Event
{
public:
    MouseScrolledEvent(float xOffset, float yOffset)
        : m_xOffset(xOffset)
        , m_yOffset(yOffset)
    {
    }

    float GetXOffset() const
    {
        return m_xOffset;
    }
    float GetYOffset() const
    {
        return m_yOffset;
    }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << GetName() << ": " << m_xOffset << ", " << m_yOffset;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseScrolled)

private:
    float m_xOffset, m_yOffset;
};

class FUEGO_API MouseButtonEvent : public Event
{
public:
    int GetMouseButton() const
    {
        return m_Button;
    }

protected:
    MouseButtonEvent(MouseCode button)
        : m_Button(button)
    {
    }

    uint16_t m_Button;
};

class FUEGO_API MouseButtonPressedEvent : public MouseButtonEvent
{
public:
    MouseButtonPressedEvent(MouseCode button)
        : MouseButtonEvent(button)
    {
    }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << GetName() << ": " << Mouse::GetMouseButtonName(m_Button);
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseButtonPressed)
};

class FUEGO_API MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
    MouseButtonReleasedEvent(MouseCode button)
        : MouseButtonEvent(button)
    {
    }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << GetName() << ": " << Mouse::GetMouseButtonName(m_Button);
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseButtonReleased)
};
}  // namespace Fuego
