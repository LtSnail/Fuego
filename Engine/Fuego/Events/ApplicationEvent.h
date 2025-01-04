#pragma once

#include "Event.h"

namespace Fuego
{
class FUEGO_API WindowResizeEvent : public Event
{
public:
    WindowResizeEvent(uint32_t width, uint32_t height)
        : Event(EVENT_NAME(WindowResizeEvent))
        , _width(width)
        , _height(height)
    {
    }

    inline uint32_t GetWidth() const
    {
        return _width;
    }
    inline uint32_t GetHeight() const
    {
        return _height;
    }

    inline std::string ToString() const override
    {
        std::stringstream ss;
        ss << _name << " w: " << _width << ", h: " << _height;
        return ss.str();
    }

private:
    uint32_t _width, _height;
};

class FUEGO_API WindowStartResizeEvent : public Event
{
public:
    WindowStartResizeEvent()
        : Event(EVENT_NAME(WindowStartResizeEvent))
    {
    }

    inline std::string ToString() const override
    {
        std::stringstream ss;
        ss << _name;
        return ss.str();
    }
};

class FUEGO_API WindowEndResizeEvent : public Event
{
public:
    WindowEndResizeEvent()
        : Event(EVENT_NAME(WindowEndResizeEvent))
    {
    }

    inline std::string ToString() const override
    {
        std::stringstream ss;
        ss << _name;
        return ss.str();
    }
};

class FUEGO_API WindowValidateEvent : public Event
{
public:
    WindowValidateEvent()
        : Event(EVENT_NAME(WindowValidateEvent))
    {
    }
};

class FUEGO_API WindowCloseEvent : public Event
{
public:
    WindowCloseEvent()
        : Event(EVENT_NAME(WindowCloseEvent))
    {
    }
};

class FUEGO_API AppTickEvent : public Event
{
public:
    AppTickEvent()
        : Event(EVENT_NAME(AppTickEvent))
    {
    }
};

class FUEGO_API AppUpdateEvent : public Event
{
public:
    AppUpdateEvent()
        : Event(EVENT_NAME(AppUpdateEvent))
    {
    }
};

class FUEGO_API AppRenderEvent : public Event
{
public:
    AppRenderEvent()
        : Event(EVENT_NAME(AppRenderEvent))
    {
    }
};
}  // namespace Fuego
