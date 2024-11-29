#pragma once

#include "Core.h"


namespace Fuego
{
#define EVENT_CLASS_TYPE(type)           \
    const char* GetName() const override \
    {                                    \
        return #type;                    \
    }

class FUEGO_API Event
{
public:
    Event() = default;
    virtual const char* GetName() const
    {
        return nullptr;
    }
    virtual std::string ToString() const
    {
        return GetName();
    }
    virtual bool GetHandled() const
    {
        return m_handled;
    }
    virtual void SetHandled()
    {
        m_handled = true;
    }

    virtual ~Event() = default;

protected:
    bool m_handled = false;
};

inline std::ostream& operator<<(std::ostream& os, const Event& ev)
{
    return os << ev.ToString();
}
}  // namespace Fuego
