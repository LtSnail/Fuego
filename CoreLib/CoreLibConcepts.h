#pragma once

#include <concepts>

namespace Fuego
{
template <class T>
concept ArrayType = std::is_array_v<T>;

template <class T>
concept NotArrayType = !ArrayType<T>;

template <class T>
concept DefaultConstructible = std::is_default_constructible_v<T>;

template <class T>
concept ByteAllocator = requires(T alloc, uint8_t* ptr, size_t size_in_bytes) {
    { alloc.allocate(size_in_bytes) } -> std::convertible_to<uint8_t*>;
    { alloc.deallocate(ptr, size_in_bytes) };
};

template <class T>
concept FuegoAllocator = DefaultConstructible<T> && ByteAllocator<T>;
}  // namespace Fuego
