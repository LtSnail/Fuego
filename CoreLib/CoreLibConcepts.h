#pragma once

#include <concepts>
#include <memory>

namespace Fuego
{
template <class T, class Deleter>
concept TypeDeleter = !std::is_array_v<T> && requires(Deleter deleter, std::remove_extent_t<T>* ptr) {
    { deleter(ptr) } -> std::convertible_to<void>;
};

template <class T, class Deleter>
concept UnboundedArrayDeleter = std::is_unbounded_array_v<T> && requires(Deleter deleter, std::remove_extent_t<T>* ptr) {
    { deleter(ptr) } -> std::convertible_to<void>;
};

template <class T, class Deleter>
concept IsDefaultCompatibleDeleter = TypeDeleter<T, Deleter> || UnboundedArrayDeleter<T, Deleter>;
}  // namespace Fuego
