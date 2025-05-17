#pragma once

#include <type_traits>

#include "Core.h"
#include "Services/ServiceInterfaces.hpp"

#define EVENT_NAME(type) (#type)

namespace Fuego
{
template <typename Derived>
struct EventBase
{
    std::string ToString() const
    {
        std::string derived_string(static_cast<const Derived*>(this)->ToStringImpl());
        std::stringstream ss;
        ss << event_name << derived_string;
        return ss.str();
    }
    bool GetHandled() const
    {
        return handled;
    }
    void SetHandled()
    {
        handled = true;
    }

protected:
    explicit EventBase(const char* name) noexcept
        : event_name(name)
        , handled(false)
    {
    }
    const char* event_name;
    bool handled;
};

}  // namespace Fuego
