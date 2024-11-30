#pragma once

#include <Core.h>

#include <exception>

#include "CoreLibConcepts.h"
#include "DefaultAllocator.h"

namespace Fuego
{

template <class T, FuegoAllocator Allocator = DefaultAllocator>
class FUEGO_API Uptr
{
public:
    FUEGO_NON_COPYABLE(Uptr);

    Uptr() noexcept = default;
    explicit Uptr(std::remove_extent_t<T>* ptr) noexcept;
    Uptr(Uptr<T, Allocator>&& other) noexcept;
    Uptr<T, Allocator>& operator=(Uptr<T, Allocator>&& other) noexcept;
    ~Uptr();

    std::remove_extent_t<T> const* Get() const noexcept;
    std::remove_extent_t<T>* Get() noexcept;

    std::remove_extent_t<T> const* operator->() const noexcept;
    std::remove_extent_t<T>* operator->() noexcept;

    std::remove_extent_t<T> const& operator*() const noexcept;
    std::remove_extent_t<T>& operator*() noexcept;

    void Reset(std::remove_extent_t<T>* ptr = nullptr) noexcept;
    std::remove_extent_t<T>* Release() noexcept;

    void Swap(Uptr<T, Allocator>& other);

    explicit operator bool() const noexcept;

    Allocator& GetAllocator() noexcept;

    template <class... Args>
    friend Uptr<T, Allocator> MakeUnique(Args&&... args);

private:
    std::remove_extent_t<T>* _ptr = nullptr;
    Allocator _alloc{};
};

template <class T, FuegoAllocator Allocator>
inline Uptr<T, Allocator>::Uptr(std::remove_extent_t<T>* ptr) noexcept
    : _ptr(ptr)
{
}

template <class T, FuegoAllocator Allocator>
inline Uptr<T, Allocator>::Uptr(Uptr<T, Allocator>&& other) noexcept
    : _ptr(other._ptr)
    , _alloc(std::move_if_noexcept(other._alloc))
{
    other._ptr = nullptr;
}

template <class T, FuegoAllocator Allocator>
inline Uptr<T, Allocator>& Uptr<T, Allocator>::operator=(Uptr<T, Allocator>&& other) noexcept
{
    if (this == &other)
        return *this;

    Reset(other.Release());
    _alloc = std::move_if_noexcept(other._alloc);

    other._ptr = nullptr;

    return *this;
}

template <class T, FuegoAllocator Allocator>
inline Uptr<T, Allocator>::~Uptr()
{
    Reset();
}

template <class T, FuegoAllocator Allocator>
inline std::remove_extent_t<T> const* Uptr<T, Allocator>::Get() const noexcept
{
    return _ptr;
}

template <class T, FuegoAllocator Allocator>
inline std::remove_extent_t<T>* Uptr<T, Allocator>::Get() noexcept
{
    return _ptr;
}

template <class T, FuegoAllocator Allocator>
inline std::remove_extent_t<T> const* Uptr<T, Allocator>::operator->() const noexcept
{
    FU_CORE_ASSERT(_ptr, "Nullptr dereferencing");
    return _ptr;
}

template <class T, FuegoAllocator Allocator>
inline std::remove_extent_t<T>* Uptr<T, Allocator>::operator->() noexcept
{
    FU_CORE_ASSERT(_ptr, "Nullptr dereferencing");
    return _ptr;
}

template <class T, FuegoAllocator Allocator>
inline std::remove_extent_t<T> const& Uptr<T, Allocator>::operator*() const noexcept
{
    FU_CORE_ASSERT(_ptr, "Nullptr dereferencing");
    return *_ptr;
}

template <class T, FuegoAllocator Allocator>
inline std::remove_extent_t<T>& Uptr<T, Allocator>::operator*() noexcept
{
    FU_CORE_ASSERT(_ptr, "Nullptr dereferencing");
    return *_ptr;
}

template <class T, FuegoAllocator Allocator>
inline void Uptr<T, Allocator>::Reset(std::remove_extent_t<T>* ptr) noexcept
{
    auto* OldPtr = _ptr;
    _ptr = ptr;

    if (OldPtr)
        _alloc.deallocate(reinterpret_cast<uint8_t*>(OldPtr), sizeof(sizeof(std::remove_extent_t<T>)));
}

template <class T, FuegoAllocator Allocator>
inline std::remove_extent_t<T>* Uptr<T, Allocator>::Release() noexcept
{
    auto* OldPtr = _ptr;
    _ptr = nullptr;

    return OldPtr;
}

template <class T, FuegoAllocator Allocator>
inline void Uptr<T, Allocator>::Swap(Uptr<T, Allocator>& other)
{
    std::swap(_ptr, other._ptr);
    std::swap(_alloc, other._alloc);
}

template <class T, FuegoAllocator Allocator>
inline Uptr<T, Allocator>::operator bool() const noexcept
{
    return _ptr != nullptr;
}

template <class T, FuegoAllocator Allocator>
inline Allocator& Uptr<T, Allocator>::GetAllocator() noexcept
{
    return _alloc;
}

template <class T, FuegoAllocator AllocatorT, class U, FuegoAllocator AllocatorU>
auto operator<=>(const Uptr<T, AllocatorT>& lhs, const Uptr<U, AllocatorU>& rhs) noexcept
{
    return std::compare_three_way{}(lhs.Get(), rhs.Get());
}

template <class T, FuegoAllocator AllocatorT, class U, FuegoAllocator AllocatorU>
bool operator==(const Uptr<T, AllocatorT>& lhs, const Uptr<U, AllocatorU>& rhs) noexcept
{
    return lhs.Get() == rhs.Get();
}

template <class T, FuegoAllocator AllocatorT, class U, FuegoAllocator AllocatorU>
bool operator!=(const Uptr<T, AllocatorT>& lhs, const Uptr<U, AllocatorU>& rhs) noexcept
{
    return lhs.Get() != rhs.Get();
}

template <class T, FuegoAllocator Allocator>
bool operator==(const Uptr<T, Allocator>& lhs, std::nullptr_t) noexcept
{
    return lhs.Get() == nullptr;
}

template <class T, FuegoAllocator Allocator>
bool operator!=(const Uptr<T, Allocator>& lhs, std::nullptr_t) noexcept
{
    return lhs.Get() != nullptr;
}

template <class T, FuegoAllocator Allocator>
bool operator==(std::nullptr_t, const Uptr<T, Allocator>& rhs) noexcept
{
    return rhs.Get() == nullptr;
}

template <class T, FuegoAllocator Allocator>
bool operator!=(std::nullptr_t, const Uptr<T, Allocator>& rhs) noexcept
{
    return rhs.Get() != nullptr;
}

template <class T, FuegoAllocator Allocator, class U>
auto operator<=>(const Uptr<T, Allocator>& lhs, U const* rhs) noexcept
{
    return std::compare_three_way{}(lhs.Get(), rhs);
}

template <class U, class T, FuegoAllocator Allocator>
auto operator<=>(U const* lhs, const Uptr<T, Allocator>& rhs) noexcept
{
    return std::compare_three_way{}(lhs, rhs.Get());
}

template <class T, FuegoAllocator Allocator>
auto operator<=>(std::nullptr_t, const Uptr<T, Allocator>& rhs) noexcept
{
    return std::compare_three_way{}(nullptr, rhs.Get());
}

template <class T, FuegoAllocator Allocator>
auto operator<=>(const Uptr<T, Allocator>& lhs, std::nullptr_t) noexcept
{
    return std::compare_three_way{}(lhs.Get(), nullptr);
}

template <class T, FuegoAllocator Allocator, class U>
bool operator==(const Uptr<T, Allocator>& lhs, U const* rhs) noexcept
{
    return lhs.Get() == rhs;
}

template <class U, class T, FuegoAllocator Allocator>
bool operator==(U const* rhs, const Uptr<T, Allocator>& lhs) noexcept
{
    return rhs == lhs.Get();
}

template <class T, FuegoAllocator Allocator, class U>
bool operator!=(const Uptr<T, Allocator>& lhs, U const* rhs) noexcept
{
    return lhs.Get() != rhs;
}

template <class U, class T, FuegoAllocator Allocator>
bool operator!=(U const* lhs, const Uptr<T, Allocator>& rhs) noexcept
{
    return lhs != rhs.Get();
}

template <class T, FuegoAllocator AllocatorT, class U, FuegoAllocator AllocatorU>
ptrdiff_t operator-(Uptr<T, AllocatorT>& lhs, Uptr<U, AllocatorU>& rhs)
{
    return lhs.Get() - rhs.Get();
}

template <class T, FuegoAllocator Allocator>
std::remove_extent_t<T>* operator+(Uptr<T, Allocator>& ptr, ptrdiff_t n)
{
    return ptr.Get() + n;
}

template <class T, FuegoAllocator Allocator>
std::remove_extent_t<T>* operator-(Uptr<T, Allocator>& ptr, ptrdiff_t n)
{
    return ptr.Get() - n;
}

template <class T, FuegoAllocator Allocator = DefaultAllocator, class... Args>
Uptr<T, Allocator> MakeUnique(Args&&... args)
{
    Allocator alloc{};
    uint8_t* MemBlock = alloc.allocate(sizeof(std::remove_extent_t<T>));

    std::remove_extent_t<T>* Ptr = nullptr;
    try
    {
        Ptr = std::construct_at(reinterpret_cast<std::remove_extent_t<T>*>(MemBlock), std::forward<Args>(args)...);
    }
    catch (...)
    {
        alloc.deallocate(MemBlock, sizeof(std::remove_extent_t<T>));
        throw;
    }

    return Uptr<T, Allocator>(Ptr);
}

template <ArrayType T, FuegoAllocator Allocator>
class FUEGO_API Uptr<T, Allocator>
{
public:
    FUEGO_NON_COPYABLE(Uptr);

    Uptr() noexcept = default;
    explicit Uptr(std::remove_extent_t<T>* ptr, size_t size) noexcept;
    Uptr(Uptr<T, Allocator>&& other) noexcept;
    Uptr<T, Allocator>& operator=(Uptr<T, Allocator>&& other) noexcept;
    ~Uptr();

    std::remove_extent_t<T> const* Get() const noexcept;
    std::remove_extent_t<T>* Get() noexcept;

    std::remove_extent_t<T> const& operator[](size_t Idx) const noexcept;
    std::remove_extent_t<T>& operator[](size_t Idx) noexcept;

    void Reset(std::remove_extent_t<T>* ptr = nullptr, size_t size = 0) noexcept;
    std::remove_extent_t<T>* Release() noexcept;

    void Swap(Uptr<T, Allocator>& other);

    explicit operator bool() const noexcept;

    Allocator& GetAllocator() noexcept;
    size_t Size() const noexcept;

private:
    std::remove_extent_t<T>* _ptr = nullptr;
    Allocator _alloc{};
    size_t _size = 0;
};

template <ArrayType T, FuegoAllocator Allocator>
inline Fuego::Uptr<T, Allocator>::Uptr(std::remove_extent_t<T>* ptr, size_t size) noexcept
    : _ptr(ptr)
    , _size(size)
{
}

template <ArrayType T, FuegoAllocator Allocator>
inline Uptr<T, Allocator>::Uptr(Uptr<T, Allocator>&& other) noexcept
    : _ptr(other._ptr)
    , _alloc(std::move_if_noexcept(other._alloc))
    , _size(other._size)
{
    other._ptr = nullptr;
    other._size = 0;
}

template <ArrayType T, FuegoAllocator Allocator>
inline Uptr<T, Allocator>& Uptr<T, Allocator>::operator=(Uptr<T, Allocator>&& other) noexcept
{
    if (this == &other)
        return *this;

    Reset(other.Release(), other._size);
    _alloc = std::move_if_noexcept(other._alloc);
    _size = other._size;

    other._ptr = nullptr;
    other._size = 0;

    return *this;
}

template <ArrayType T, FuegoAllocator Allocator>
inline Uptr<T, Allocator>::~Uptr()
{
    Reset();
}

template <ArrayType T, FuegoAllocator Allocator>
inline std::remove_extent_t<T> const* Uptr<T, Allocator>::Get() const noexcept
{
    return _ptr;
}

template <ArrayType T, FuegoAllocator Allocator>
inline std::remove_extent_t<T>* Uptr<T, Allocator>::Get() noexcept
{
    return _ptr;
}

template <ArrayType T, FuegoAllocator Allocator>
inline std::remove_extent_t<T> const& Uptr<T, Allocator>::operator[](size_t Idx) const noexcept
{
    FU_CORE_ASSERT(_ptr, "Nullptr dereferencing");
    FU_CORE_ASSERT(Idx < _size, "Index out of bounds");
    return _ptr[Idx];
}

template <ArrayType T, FuegoAllocator Allocator>
inline std::remove_extent_t<T>& Uptr<T, Allocator>::operator[](size_t Idx) noexcept
{
    FU_CORE_ASSERT(_ptr, "Nullptr dereferencing");
    FU_CORE_ASSERT(Idx < _size, "Index out of bounds");
    return _ptr[Idx];
}

template <ArrayType T, FuegoAllocator Allocator>
inline void Uptr<T, Allocator>::Reset(std::remove_extent_t<T>* ptr, size_t size) noexcept
{
    auto* OldPtr = _ptr;
    _ptr = ptr;

    if (OldPtr)
        _alloc.deallocate(reinterpret_cast<uint8_t*>(OldPtr), _size * sizeof(std::remove_extent_t<T>));

    _size = size;
}

template <ArrayType T, FuegoAllocator Allocator>
inline size_t Uptr<T, Allocator>::Size() const noexcept
{
    return _size;
}

template <ArrayType T, FuegoAllocator Allocator>
inline std::remove_extent_t<T>* Uptr<T, Allocator>::Release() noexcept
{
    auto* OldPtr = _ptr;
    _ptr = nullptr;
    _size = 0;

    return OldPtr;
}

template <ArrayType T, FuegoAllocator Allocator>
inline void Uptr<T, Allocator>::Swap(Uptr<T, Allocator>& other)
{
    std::swap(_ptr, other._ptr);
    std::swap(_alloc, other._alloc);
    std::swap(_size, other._size);
}

template <ArrayType T, FuegoAllocator Allocator>
inline Uptr<T, Allocator>::operator bool() const noexcept
{
    return _ptr != nullptr;
}

template <ArrayType T, FuegoAllocator Allocator>
inline Allocator& Uptr<T, Allocator>::GetAllocator() noexcept
{
    return _alloc;
}

template <ArrayType T, FuegoAllocator AllocatorT, ArrayType U, FuegoAllocator AllocatorU>
auto operator<=>(const Uptr<T, AllocatorT>& lhs, const Uptr<U, AllocatorU>& rhs) noexcept
{
    return std::compare_three_way{}(lhs.Get(), rhs.Get());
}

template <ArrayType T, FuegoAllocator AllocatorT, ArrayType U, FuegoAllocator AllocatorU>
bool operator==(const Uptr<T, AllocatorT>& lhs, const Uptr<U, AllocatorU>& rhs) noexcept
{
    return lhs.Get() == rhs.Get();
}

template <ArrayType T, FuegoAllocator AllocatorT, ArrayType U, FuegoAllocator AllocatorU>
bool operator!=(const Uptr<T, AllocatorT>& lhs, const Uptr<U, AllocatorU>& rhs) noexcept
{
    return lhs.Get() != rhs.Get();
}

template <ArrayType T, FuegoAllocator Allocator>
bool operator==(const Uptr<T, Allocator>& lhs, std::nullptr_t) noexcept
{
    return lhs.Get() == nullptr;
}

template <ArrayType T, FuegoAllocator Allocator>
bool operator!=(const Uptr<T, Allocator>& lhs, std::nullptr_t) noexcept
{
    return lhs.Get() != nullptr;
}

template <ArrayType T, FuegoAllocator Allocator>
bool operator==(std::nullptr_t, const Uptr<T, Allocator>& rhs) noexcept
{
    return rhs.Get() == nullptr;
}

template <ArrayType T, FuegoAllocator Allocator>
bool operator!=(std::nullptr_t, const Uptr<T, Allocator>& rhs) noexcept
{
    return rhs.Get() != nullptr;
}

template <ArrayType T, FuegoAllocator Allocator, class U>
auto operator<=>(const Uptr<T, Allocator>& lhs, U const* rhs) noexcept
{
    return std::compare_three_way{}(lhs.Get(), rhs);
}

template <class U, ArrayType T, FuegoAllocator Allocator>
auto operator<=>(U const* lhs, const Uptr<T, Allocator>& rhs) noexcept
{
    return std::compare_three_way{}(lhs, rhs.Get());
}

template <ArrayType T, FuegoAllocator Allocator>
auto operator<=>(std::nullptr_t, const Uptr<T, Allocator>& rhs) noexcept
{
    return std::compare_three_way{}(nullptr, rhs.Get());
}

template <ArrayType T, FuegoAllocator Allocator>
auto operator<=>(const Uptr<T, Allocator>& lhs, std::nullptr_t) noexcept
{
    return std::compare_three_way{}(lhs.Get(), nullptr);
}

template <ArrayType T, FuegoAllocator Allocator, class U>
bool operator==(const Uptr<T, Allocator>& lhs, U const* rhs) noexcept
{
    return lhs.Get() == rhs;
}

template <class U, ArrayType T, FuegoAllocator Allocator>
bool operator==(U const* rhs, const Uptr<T, Allocator>& lhs) noexcept
{
    return rhs == lhs.Get();
}

template <ArrayType T, FuegoAllocator Allocator, class U>
bool operator!=(const Uptr<T, Allocator>& lhs, U const* rhs) noexcept
{
    return lhs.Get() != rhs;
}

template <class U, ArrayType T, FuegoAllocator Allocator>
bool operator!=(U const* lhs, Uptr<T, Allocator>& rhs) noexcept
{
    return lhs != rhs.Get();
}

template <ArrayType T, FuegoAllocator AllocatorT, class U, FuegoAllocator AllocatorU>
ptrdiff_t operator-(Uptr<T, AllocatorT>& lhs, Uptr<U, AllocatorU>& rhs)
{
    return lhs.Get() - rhs.Get();
}

template <ArrayType T, FuegoAllocator Allocator>
std::remove_extent_t<T>* operator+(Uptr<T, Allocator>& ptr, ptrdiff_t n)
{
    return ptr.Get() + n;
}

template <ArrayType T, FuegoAllocator Allocator>
std::remove_extent_t<T>* operator-(Uptr<T, Allocator>& ptr, ptrdiff_t n)
{
    return ptr.Get() - n;
}

template <ArrayType T, FuegoAllocator Allocator = DefaultAllocator, class... Args>
Uptr<T, Allocator> MakeUnique(size_t size, Args&&... args)
{
    Allocator alloc{};

    uint8_t* MemBlock = alloc.allocate(size * sizeof(std::remove_extent_t<T>));

    std::remove_extent_t<T>* Instance = nullptr;
    size_t Idx = 0;
    try
    {
        Instance = reinterpret_cast<std::remove_extent_t<T>*>(MemBlock);
        for (; Idx < size; ++Idx) std::construct_at(Instance + Idx, std::forward<Args>(args)...);
    }
    catch (...)
    {
        for (; Idx > 0; --Idx) std::destroy_at(Instance + (Idx - 1));

        alloc.deallocate(MemBlock, size * sizeof(std::remove_extent_t<T>));
        throw;
    }

    return Uptr<T, Allocator>(Instance, size);
}

}  // namespace Fuego
