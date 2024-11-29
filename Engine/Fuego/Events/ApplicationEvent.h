#pragma once

#include "Event.h"

namespace Fuego
{
class FUEGO_API WindowResizeEvent : public Event
{
public:
    WindowResizeEvent(unsigned int width, unsigned int height)
        : m_Width(width)
        , m_Height(height)
    {
    }

    unsigned int GetWidth() const
    {
        return m_Width;
    }
    unsigned int GetHeight() const
    {
        return m_Height;
    }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << GetName() << " w: " << m_Width << ", h: " << m_Height;
        return ss.str();
    }

    EVENT_CLASS_TYPE(WindowResize)

private:
    unsigned int m_Width, m_Height;
};

class FUEGO_API WindowCloseEvent : public Event
{
public:
    WindowCloseEvent() = default;

    EVENT_CLASS_TYPE(WindowClose)
};

class FUEGO_API AppTickEvent : public Event
{
public:
    AppTickEvent() = default;

    EVENT_CLASS_TYPE(AppTick)
};

class FUEGO_API AppUpdateEvent : public Event
{
public:
    AppUpdateEvent() = default;

    EVENT_CLASS_TYPE(AppUpdate)
};

class FUEGO_API AppRenderEvent : public Event
{
public:
    AppRenderEvent() = default;

    EVENT_CLASS_TYPE(AppRender)
};
}  // namespace Fuego
