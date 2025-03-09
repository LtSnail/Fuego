#pragma once

#include <memory>

namespace Fuego
{
template <typename T>
class singleton
{
public:
    static T& instance()
    {
        static std::unique_ptr<T> instance = std::make_unique<T>();
        return *instance;
    }

    singleton(const singleton&) = delete;
    singleton& operator=(const singleton&) = delete;
    singleton(singleton&&) = delete;
    singleton& operator=(singleton&&) = delete;

protected:
    singleton() = default;
    virtual ~singleton() = default;
};
}  // namespace Fuego
