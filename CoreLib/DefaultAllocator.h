#pragma once

#include <cstdint>

namespace Fuego
{

class DefaultAllocator
{
public:
    inline uint8_t* allocate(size_t size) noexcept
    {
        return static_cast<uint8_t*>(::operator new[](size, std::nothrow));
    }
    inline void deallocate(uint8_t* ptr, size_t size) noexcept
    {
        UNUSED(size);
        ::operator delete[](ptr, std::nothrow);
    }
};

}  // namespace Fuego
