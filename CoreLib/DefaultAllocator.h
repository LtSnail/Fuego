#pragma once

#include <cstdint>

namespace Fuego
{
class DefaultAllocator
{
public:
    uint8_t* allocate(size_t size) noexcept;
    void deallocate(uint8_t* ptr, size_t size) noexcept;
};
}  // namespace Fuego
