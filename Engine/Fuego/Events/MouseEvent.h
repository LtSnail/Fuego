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

    inline float GetX() const
    {
        return m_MouseX;
    }
    inline float GetY() const
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
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

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

    inline float GetXOffset() const
    {
        return m_xOffset;
    }
    inline float GetYOffset() const
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
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

private:
    float m_xOffset, m_yOffset;
};

class FUEGO_API MouseButtonEvent : public Event
{
public:
    inline int GetMouseButton() const
    {
        return m_Button;
    }

    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

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
