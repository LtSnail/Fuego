#pragma once

#include "Core.h"

#define EVENT_NAME(type) (#type)

namespace Fuego
{
class FUEGO_API Event
{
public:
    Event() = delete;

    virtual std::string ToString() const
    {
        return _name;
    }

    virtual bool GetHandled() const
    {
        return _handled;
    }
    virtual void SetHandled()
    {
        _handled = true;
    }

    virtual ~Event() = default;

protected:
    explicit Event(const char* name)
        : _name(name)
    {
    }

    bool _handled = false;
    const char* _name;
};

inline std::ostream& operator<<(std::ostream& os, const Event& ev)
{
    return os << ev.ToString();
}
}  // namespace Fuego
