#include "DefaultAllocator.h"

#include <Core.h>

uint8_t* Fuego::DefaultAllocator::allocate(size_t size) noexcept
{
    return static_cast<uint8_t*>(::operator new[](size, std::nothrow));
}

void Fuego::DefaultAllocator::deallocate(uint8_t* ptr, size_t size) noexcept
{
    UNUSED(size);
    ::operator delete[](ptr, std::nothrow);
}
