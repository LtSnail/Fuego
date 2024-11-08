#pragma once

#include "Event.h"

namespace Fuego
{
class FUEGO_API KeyEvent : public Event
{
   public:
    inline int GetKeyCode() const { return m_KeyCode; }

    EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)
   protected:
    KeyEvent(int keycode) : Event(), m_KeyCode(keycode) {}

    int m_KeyCode;
};

class FUEGO_API KeyPressedEvent : public KeyEvent
{
   public:
    KeyPressedEvent(int keycode, int repeatCount)
        : KeyEvent(keycode), m_RepeatCount(repeatCount)
    {
    }

    inline int GetRepeatCount() const { return m_RepeatCount; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << GetName() << ": " << m_KeyCode << " (" << m_RepeatCount
           << " repeats)";
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyPressed)
   private:
    int m_RepeatCount;
};

class FUEGO_API KeyReleasedEvent : public KeyEvent
{
   public:
    KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << GetName() << ": " << m_KeyCode;
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyReleased)
};
}  // namespace Fuego
