#pragma once

#include <Core.h>

#include <cstdint>

#include "CoreLibConcepts.h"
#include "DefaultAllocator.h"


namespace Fuego
{

template <class T, FuegoAllocator Allocator>
class Wptr;

template <class T, FuegoAllocator Allocator = DefaultAllocator>
class FUEGO_API Sptr
{
public:
    template <class U, FuegoAllocator AllocatorU>
    friend class Wptr;

    explicit Sptr(std::remove_extent_t<T>* ptr = nullptr) noexcept;

    Sptr(const Sptr<T, Allocator>& other) noexcept;
    Sptr(Sptr<T, Allocator>&& other) noexcept;

    Sptr<T, Allocator>& operator=(const Sptr<T, Allocator>& other) noexcept;
    Sptr<T, Allocator>& operator=(Sptr<T, Allocator>&& other) noexcept;

    ~Sptr();

    std::remove_extent_t<T> const* Get() const noexcept;
    std::remove_extent_t<T>* Get() noexcept;

    std::remove_extent_t<T> const& operator*() const noexcept;
    std::remove_extent_t<T>& operator*() noexcept;

    std::remove_extent_t<T> const* operator->() const noexcept;
    std::remove_extent_t<T>* operator->() noexcept;

    uint32_t UseCount() const noexcept;
    explicit operator bool() const noexcept;

    /// <summary>
    /// Cleans up after the old pointer and set up on the new one.
    /// </summary>
    /// <param name="ptr">New pointer to be tracked. Will be managed by the previous allocator (the same allocator as previous poiner).</param>
    void Reset(std::remove_extent_t<T>* ptr = nullptr);

    void Swap(Sptr<T, Allocator>& other) noexcept;

    template <NotArrayType U, FuegoAllocator AllocatorU, class... Args>
    friend Sptr<U, AllocatorU> MakeShared(Args&&... args);

private:
    struct _ControlBlock
    {
        std::remove_extent_t<T>* _ptr = nullptr;
        uint32_t _strong = 0;
        uint32_t _weak = 0;
    };

    Allocator _alloc{};
    _ControlBlock* _cb = nullptr;

    explicit Sptr(Sptr<T, Allocator>::_ControlBlock* cb) noexcept;

    void Release();
};

template <class T, FuegoAllocator Allocator>
inline Sptr<T, Allocator>::Sptr(std::remove_extent_t<T>* ptr) noexcept
{
    _cb = reinterpret_cast<_ControlBlock*>(_alloc.allocate(sizeof(_ControlBlock)));
    std::construct_at(_cb);

    _cb->_ptr = ptr;

    if (ptr)
        ++(_cb->_strong);
}

template <class T, FuegoAllocator Allocator>
inline Sptr<T, Allocator>::Sptr(const Sptr<T, Allocator>& other) noexcept
    : _cb(other._cb)
{
    if (_cb)
        ++(_cb->_strong);
}

template <class T, FuegoAllocator Allocator>
inline Sptr<T, Allocator>::Sptr(Sptr<T, Allocator>&& other) noexcept
    : _cb(std::exchange(other._cb, nullptr))
{
}

template <class T, FuegoAllocator Allocator>
inline Sptr<T, Allocator>& Sptr<T, Allocator>::operator=(const Sptr<T, Allocator>& other) noexcept
{
    if (this == &other)
        return *this;

    Release();

    _cb = other._cb;

    if (_cb)
        ++(_cb->_strong);

    return *this;
}

template <class T, FuegoAllocator Allocator>
inline Sptr<T, Allocator>& Sptr<T, Allocator>::operator=(Sptr<T, Allocator>&& other) noexcept
{
    if (this == &other)
        return *this;

    Release();

    _cb = std::exchange(other._cb, nullptr);

    return *this;
}

template <class T, FuegoAllocator Allocator>
inline Sptr<T, Allocator>::~Sptr()
{
    Release();
}

template <class T, FuegoAllocator Allocator>
inline std::remove_extent_t<T> const* Sptr<T, Allocator>::Get() const noexcept
{
    if (!_cb)
        return nullptr;

    return _cb->_ptr;
}

template <class T, FuegoAllocator Allocator>
inline std::remove_extent_t<T>* Sptr<T, Allocator>::Get() noexcept
{
    if (!_cb)
        return nullptr;

    return _cb->_ptr;
}

template <class T, FuegoAllocator Allocator>
inline std::remove_extent_t<T> const& Sptr<T, Allocator>::operator*() const noexcept
{
    FU_CORE_ASSERT(_cb, "Nullptr dereferencing");
    FU_CORE_ASSERT(_cb->_ptr, "Nullptr dereferencing");
    return *_cb->_ptr;
}

template <class T, FuegoAllocator Allocator>
inline std::remove_extent_t<T>& Sptr<T, Allocator>::operator*() noexcept
{
    FU_CORE_ASSERT(_cb, "Nullptr dereferencing");
    FU_CORE_ASSERT(_cb->_ptr, "Nullptr dereferencing");
    return *_cb->_ptr;
}

template <class T, FuegoAllocator Allocator>
inline std::remove_extent_t<T> const* Sptr<T, Allocator>::operator->() const noexcept
{
    FU_CORE_ASSERT(_cb, "Nullptr dereferencing");
    FU_CORE_ASSERT(_cb->_ptr, "Nullptr dereferencing");
    return _cb->_ptr;
}

template <class T, FuegoAllocator Allocator>
inline std::remove_extent_t<T>* Sptr<T, Allocator>::operator->() noexcept
{
    FU_CORE_ASSERT(_cb, "Nullptr dereferencing");
    FU_CORE_ASSERT(_cb->_ptr, "Nullptr dereferencing");
    return _cb->_ptr;
}

template <class T, FuegoAllocator Allocator>
inline uint32_t Sptr<T, Allocator>::UseCount() const noexcept
{
    return _cb ? _cb->_strong : 0;
}

template <class T, FuegoAllocator Allocator>
inline Sptr<T, Allocator>::operator bool() const noexcept
{
    return _cb ? _cb->_ptr != nullptr : false;
}

template <class T, FuegoAllocator Allocator>
inline void Sptr<T, Allocator>::Reset(std::remove_extent_t<T>* ptr)
{
    Release();

    if (ptr)
    {
        _cb = reinterpret_cast<_ControlBlock*>(_alloc.allocate(sizeof(_ControlBlock)));
        std::construct_at(_cb);

        _cb->_ptr = ptr;
        ++(_cb->_strong);
    }
}

template <class T, FuegoAllocator Allocator>
inline void Sptr<T, Allocator>::Release()
{
    if (!_cb)
        return;

    --(_cb->_strong);

    if (_cb->_strong == 0)
    {
        std::destroy_at(_cb->_ptr);
        _alloc.deallocate(reinterpret_cast<uint8_t*>(_cb->_ptr), sizeof(std::remove_extent_t<T>));
        _cb->_ptr = nullptr;

        if (_cb->_weak == 0)
        {
            std::destroy_at(_cb);
            _alloc.deallocate(reinterpret_cast<uint8_t*>(_cb), sizeof(_ControlBlock));
        }
    }

    _cb = nullptr;
}

template <class T, FuegoAllocator Allocator>
inline void Sptr<T, Allocator>::Swap(Sptr<T, Allocator>& other) noexcept
{
    std::swap(_cb, other._cb);
}

template <class T, FuegoAllocator AllocatorT, class U, FuegoAllocator AllocatorU>
auto operator<=>(const Sptr<T, AllocatorT>& lhs, const Sptr<U, AllocatorU>& rhs) noexcept
{
    return std::compare_three_way{}(lhs.Get(), rhs.Get());
}

template <class T, FuegoAllocator AllocatorT, class U, FuegoAllocator AllocatorU>
bool operator==(const Sptr<T, AllocatorT>& lhs, const Sptr<U, AllocatorU>& rhs) noexcept
{
    return lhs.Get() == rhs.Get();
}

template <class T, FuegoAllocator AllocatorT, class U, FuegoAllocator AllocatorU>
bool operator!=(const Sptr<T, AllocatorT>& lhs, const Sptr<U, AllocatorU>& rhs) noexcept
{
    return lhs.Get() != rhs.Get();
}

template <class T, FuegoAllocator Allocator>
bool operator==(const Sptr<T, Allocator>& lhs, std::nullptr_t) noexcept
{
    return lhs.Get() == nullptr;
}

template <class T, FuegoAllocator Allocator>
bool operator!=(const Sptr<T, Allocator>& lhs, std::nullptr_t) noexcept
{
    return lhs.Get() != nullptr;
}

template <class T, FuegoAllocator Allocator>
bool operator==(std::nullptr_t, const Sptr<T, Allocator>& rhs) noexcept
{
    return rhs.Get() == nullptr;
}

template <class T, FuegoAllocator Allocator>
bool operator!=(std::nullptr_t, const Sptr<T, Allocator>& rhs) noexcept
{
    return rhs.Get() != nullptr;
}

template <class T, FuegoAllocator Allocator, class U>
auto operator<=>(const Sptr<T, Allocator>& lhs, U const* rhs) noexcept
{
    return std::compare_three_way{}(lhs.Get(), rhs);
}

template <class U, class T, FuegoAllocator Allocator>
auto operator<=>(U const* lhs, const Sptr<T, Allocator>& rhs) noexcept
{
    return std::compare_three_way{}(lhs, rhs.Get());
}

template <class T, FuegoAllocator Allocator>
auto operator<=>(std::nullptr_t, const Sptr<T, Allocator>& rhs) noexcept
{
    return std::compare_three_way{}(nullptr, rhs.Get());
}

template <class T, FuegoAllocator Allocator>
auto operator<=>(const Sptr<T, Allocator>& lhs, std::nullptr_t) noexcept
{
    return std::compare_three_way{}(lhs.Get(), nullptr);
}

template <class T, FuegoAllocator Allocator, class U>
bool operator==(const Sptr<T, Allocator>& lhs, U const* rhs) noexcept
{
    return lhs.Get() == rhs;
}

template <class U, class T, FuegoAllocator Allocator>
bool operator==(U const* rhs, const Sptr<T, Allocator>& lhs) noexcept
{
    return rhs == lhs.Get();
}

template <class T, FuegoAllocator Allocator, class U>
bool operator!=(const Sptr<T, Allocator>& lhs, U const* rhs) noexcept
{
    return lhs.Get() != rhs;
}

template <class U, class T, FuegoAllocator Allocator>
bool operator!=(U const* lhs, const Sptr<T, Allocator>& rhs) noexcept
{
    return lhs != rhs.Get();
}

template <class T, FuegoAllocator AllocatorT, class U, FuegoAllocator AllocatorU>
ptrdiff_t operator-(Sptr<T, AllocatorT>& lhs, Sptr<U, AllocatorU>& rhs)
{
    return lhs.Get() - rhs.Get();
}

template <class T, FuegoAllocator Allocator>
std::remove_extent_t<T>* operator+(Sptr<T, Allocator>& ptr, ptrdiff_t n)
{
    return ptr.Get() + n;
}

template <class T, FuegoAllocator Allocator>
std::remove_extent_t<T>* operator-(Sptr<T, Allocator>& ptr, ptrdiff_t n)
{
    return ptr.Get() - n;
}

template <class T, FuegoAllocator Allocator>
Sptr<T, Allocator>::Sptr(Sptr<T, Allocator>::_ControlBlock* cb) noexcept
    : _cb(cb)
{
    if (_cb)
        ++(_cb->_strong);
}
template <NotArrayType T, FuegoAllocator Allocator = DefaultAllocator, class... Args>
Sptr<T, Allocator> MakeShared(Args&&... args)
{
    Sptr<T, Allocator> Sp;

    uint8_t* MemBlock = Sp._alloc.allocate(sizeof(std::remove_extent_t<T>));

    std::remove_extent_t<T>* Ptr = nullptr;
    try
    {
        Ptr = std::construct_at(reinterpret_cast<std::remove_extent_t<T>*>(MemBlock), std::forward<Args>(args)...);
    }
    catch (...)
    {
        Sp._alloc.deallocate(MemBlock, sizeof(std::remove_extent_t<T>));
        throw;
    }

    Sp._cb->_ptr = Ptr;
    ++(Sp._cb->_strong);

    return Sp;
}

template <ArrayType T, FuegoAllocator Allocator>
class FUEGO_API Sptr<T, Allocator>
{
public:
    template <class U, FuegoAllocator AllocatorU>
    friend class Wptr;

    explicit Sptr(size_t size, std::remove_extent_t<T>* ptr) noexcept;

    Sptr(const Sptr<T, Allocator>& other) noexcept;
    Sptr(Sptr<T, Allocator>&& other) noexcept;

    Sptr<T, Allocator>& operator=(const Sptr<T, Allocator>& other) noexcept;
    Sptr<T, Allocator>& operator=(Sptr<T, Allocator>&& other) noexcept;

    ~Sptr();

    std::remove_extent_t<T> const* Get() const noexcept;
    std::remove_extent_t<T>* Get() noexcept;

    std::remove_extent_t<T> const& operator[](size_t Idx) const noexcept;
    std::remove_extent_t<T>& operator[](size_t Idx) noexcept;

    uint32_t UseCount() const noexcept;
    explicit operator bool() const noexcept;

    /// <summary>
    /// Cleans up after the old pointer and set up on the new one.
    /// </summary>
    /// <param name="ptr">New pointer to be tracked. Will be managed by the previous allocator (the same allocator as previous poiner).</param>
    void Reset(size_t size = 0, std::remove_extent_t<T>* ptr = nullptr);

    void Swap(Sptr<T, Allocator>& other) noexcept;
    size_t GetSize() const noexcept;

    template <ArrayType U, FuegoAllocator AllocatorU, class... Args>
    friend Sptr<U, AllocatorU> MakeShared(size_t size, Args&&... args);

private:
    struct _ControlBlock
    {
        std::remove_extent_t<T>* _ptr = nullptr;
        uint32_t _strong = 0;
        uint32_t _weak = 0;
    };

    Allocator _alloc{};
    _ControlBlock* _cb = nullptr;
    size_t _size;

    explicit Sptr(Sptr<T, Allocator>::_ControlBlock* cb) noexcept;
    void Release();
};

template <ArrayType T, FuegoAllocator Allocator>
inline Sptr<T, Allocator>::Sptr(size_t size, std::remove_extent_t<T>* ptr) noexcept
    : _size(size)
{
    _cb = reinterpret_cast<_ControlBlock*>(_alloc.allocate(sizeof(_ControlBlock)));
    std::construct_at(_cb);

    _cb->_ptr = ptr;

    if (ptr)
        ++(_cb->_strong);
}

template <ArrayType T, FuegoAllocator Allocator>
inline Sptr<T, Allocator>::Sptr(const Sptr<T, Allocator>& other) noexcept
    : _cb(other._cb)
    , _size(other._size)
{
    if (_cb)
        ++(_cb->_strong);
}

template <ArrayType T, FuegoAllocator Allocator>
inline Sptr<T, Allocator>::Sptr(Sptr<T, Allocator>&& other) noexcept
    : _cb(std::exchange(other._cb, nullptr))
    , _size(other._size)
{
}

template <ArrayType T, FuegoAllocator Allocator>
inline Sptr<T, Allocator>& Sptr<T, Allocator>::operator=(const Sptr<T, Allocator>& other) noexcept
{
    if (this == &other)
        return *this;

    Release();

    _cb = other._cb;
    _size = other._size;

    if (_cb)
        ++(_cb->_strong);

    return *this;
}

template <ArrayType T, FuegoAllocator Allocator>
inline Sptr<T, Allocator>& Sptr<T, Allocator>::operator=(Sptr<T, Allocator>&& other) noexcept
{
    if (this == &other)
        return *this;

    Release();

    _cb = std::exchange(other._cb, nullptr);
    _size = other._size;

    return *this;
}

template <ArrayType T, FuegoAllocator Allocator>
inline Sptr<T, Allocator>::~Sptr()
{
    Release();
}

template <ArrayType T, FuegoAllocator Allocator>
inline std::remove_extent_t<T> const* Sptr<T, Allocator>::Get() const noexcept
{
    return _cb->_ptr;
}

template <ArrayType T, FuegoAllocator Allocator>
inline std::remove_extent_t<T>* Sptr<T, Allocator>::Get() noexcept
{
    return _cb->_ptr;
}

template <ArrayType T, FuegoAllocator Allocator>
inline std::remove_extent_t<T> const& Sptr<T, Allocator>::operator[](size_t Idx) const noexcept
{
    FU_CORE_ASSERT(_cb, "Nullptr dereferencing");
    FU_CORE_ASSERT(_cb->_ptr, "Nullptr dereferencing");
    FU_CORE_ASSERT(Idx < _size, "Index out of bounds");
    return _cb->_ptr[Idx];
}

template <ArrayType T, FuegoAllocator Allocator>
inline std::remove_extent_t<T>& Sptr<T, Allocator>::operator[](size_t Idx) noexcept
{
    FU_CORE_ASSERT(_cb, "Nullptr dereferencing");
    FU_CORE_ASSERT(_cb->_ptr, "Nullptr dereferencing");
    FU_CORE_ASSERT(Idx < _size, "Index out of bounds");
    return _cb->_ptr[Idx];
}

template <ArrayType T, FuegoAllocator Allocator>
inline uint32_t Sptr<T, Allocator>::UseCount() const noexcept
{
    return _cb ? _cb->_strong : 0;
}

template <ArrayType T, FuegoAllocator Allocator>
inline Sptr<T, Allocator>::operator bool() const noexcept
{
    return _cb ? _cb->_ptr != nullptr : false;
}

template <ArrayType T, FuegoAllocator Allocator>
inline void Sptr<T, Allocator>::Reset(size_t size, std::remove_extent_t<T>* ptr)
{
    Release();

    if (ptr)
    {
        _cb = reinterpret_cast<_ControlBlock*>(_alloc.allocate(sizeof(_ControlBlock)));
        std::construct_at(_cb);

        _cb->_ptr = ptr;
        _size = size;

        ++(_cb->_strong);
    }
}

template <ArrayType T, FuegoAllocator Allocator>
inline void Sptr<T, Allocator>::Release()
{
    if (!_cb)
        return;

    --(_cb->_strong);

    if (_cb->_strong == 0)
    {
        for (size_t Idx = 0; Idx < _size; ++Idx) std::destroy_at(_cb->_ptr + Idx);

        _alloc.deallocate(reinterpret_cast<uint8_t*>(_cb->_ptr), sizeof(std::remove_extent_t<T>));
        _cb->_ptr = nullptr;

        if (_cb->_weak == 0)
        {
            std::destroy_at(_cb);
            _alloc.deallocate(reinterpret_cast<uint8_t*>(_cb), sizeof(_ControlBlock));
        }
    }

    _cb = nullptr;
    _size = 0;
}

template <ArrayType T, FuegoAllocator Allocator>
inline size_t Sptr<T, Allocator>::GetSize() const noexcept
{
    return _size;
}

template <ArrayType T, FuegoAllocator Allocator>
inline void Sptr<T, Allocator>::Swap(Sptr<T, Allocator>& other) noexcept
{
    std::swap(_cb, other._cb);
    std::swap(_size, other._size);
}

template <ArrayType T, FuegoAllocator Allocator>
bool operator!=(const Sptr<T, Allocator>& lhs, std::nullptr_t) noexcept
{
    return lhs.Get() != nullptr;
}

template <ArrayType T, FuegoAllocator Allocator>
bool operator==(std::nullptr_t, const Sptr<T, Allocator>& rhs) noexcept
{
    return rhs.Get() == nullptr;
}

template <ArrayType T, FuegoAllocator Allocator>
bool operator!=(std::nullptr_t, const Sptr<T, Allocator>& rhs) noexcept
{
    return rhs.Get() != nullptr;
}

template <ArrayType T, FuegoAllocator Allocator, class U>
auto operator<=>(const Sptr<T, Allocator>& lhs, U const* rhs) noexcept
{
    return std::compare_three_way{}(lhs.Get(), rhs);
}

template <class U, ArrayType T, FuegoAllocator Allocator>
auto operator<=>(U const* lhs, const Sptr<T, Allocator>& rhs) noexcept
{
    return std::compare_three_way{}(lhs, rhs.Get());
}

template <ArrayType T, FuegoAllocator Allocator>
auto operator<=>(std::nullptr_t, const Sptr<T, Allocator>& rhs) noexcept
{
    return std::compare_three_way{}(nullptr, rhs.Get());
}

template <ArrayType T, FuegoAllocator Allocator>
auto operator<=>(const Sptr<T, Allocator>& lhs, std::nullptr_t) noexcept
{
    return std::compare_three_way{}(lhs.Get(), nullptr);
}

template <ArrayType T, FuegoAllocator Allocator, class U>
bool operator==(const Sptr<T, Allocator>& lhs, U const* rhs) noexcept
{
    return lhs.Get() == rhs;
}

template <class U, ArrayType T, FuegoAllocator Allocator>
bool operator==(U const* rhs, const Sptr<T, Allocator>& lhs) noexcept
{
    return rhs == lhs.Get();
}

template <ArrayType T, FuegoAllocator Allocator, class U>
bool operator!=(const Sptr<T, Allocator>& lhs, U const* rhs) noexcept
{
    return lhs.Get() != rhs;
}

template <class U, ArrayType T, FuegoAllocator Allocator>
bool operator!=(U const* lhs, const Sptr<T, Allocator>& rhs) noexcept
{
    return lhs != rhs.Get();
}

template <ArrayType T, FuegoAllocator AllocatorT, ArrayType U, FuegoAllocator AllocatorU>
ptrdiff_t operator-(Sptr<T, AllocatorT>& lhs, Sptr<U, AllocatorU>& rhs)
{
    return lhs.Get() - rhs.Get();
}

template <ArrayType T, FuegoAllocator AllocatorT, class U, FuegoAllocator AllocatorU>
ptrdiff_t operator-(Sptr<T, AllocatorT>& lhs, Sptr<U, AllocatorU>& rhs)
{
    return lhs.Get() - rhs.Get();
}

template <class T, FuegoAllocator AllocatorT, ArrayType U, FuegoAllocator AllocatorU>
ptrdiff_t operator-(Sptr<T, AllocatorT>& lhs, Sptr<U, AllocatorU>& rhs)
{
    return lhs.Get() - rhs.Get();
}

template <ArrayType T, FuegoAllocator Allocator>
std::remove_extent_t<T>* operator+(Sptr<T, Allocator>& ptr, ptrdiff_t n)
{
    return ptr.Get() + n;
}

template <ArrayType T, FuegoAllocator Allocator>
std::remove_extent_t<T>* operator-(Sptr<T, Allocator>& ptr, ptrdiff_t n)
{
    return ptr.Get() - n;
}

template <ArrayType T, FuegoAllocator Allocator>
Sptr<T, Allocator>::Sptr(Sptr<T, Allocator>::_ControlBlock* cb) noexcept
    : _cb(cb)
{
    if (_cb)
        ++(_cb->_strong);
}
template <ArrayType T, FuegoAllocator Allocator = DefaultAllocator, class... Args>
Sptr<T, Allocator> MakeShared(size_t size, Args&&... args)
{
    Sptr<T, Allocator> Sp(size, nullptr);

    uint8_t* MemBlock = Sp._alloc.allocate(Sp._size * sizeof(std::remove_extent_t<T>));

    std::remove_extent_t<T>* Instance = nullptr;
    size_t Idx = 0;
    try
    {
        Instance = reinterpret_cast<std::remove_extent_t<T>*>(MemBlock);
        for (; Idx < Sp._size; ++Idx) std::construct_at(Instance + Idx, std::forward<Args>(args)...);
    }
    catch (...)
    {
        for (; Idx > 0; --Idx) std::destroy_at(Instance + (Idx - 1));

        Sp._alloc.deallocate(MemBlock, sizeof(std::remove_extent_t<T>));
        throw;
    }

    Sp._cb->_ptr = Instance;
    ++(Sp._cb->_strong);

    return Sp;
}


template <class T, FuegoAllocator Allocator = DefaultAllocator>
class FUEGO_API Wptr
{
public:
    Wptr(const Wptr<T, Allocator>& other) noexcept;
    Wptr(Wptr<T, Allocator>&& other) noexcept;

    Wptr<T, Allocator>& operator=(const Wptr<T, Allocator>& other) noexcept;
    Wptr<T, Allocator>& operator=(Wptr<T, Allocator>&& other) noexcept;

    ~Wptr();

    explicit Wptr(const Sptr<T, Allocator>& shared) noexcept;
    Wptr<T, Allocator>& operator=(const Sptr<T, Allocator>& shared) noexcept;

    Sptr<T, Allocator> Lock() const noexcept;

    bool Expired() const noexcept;

    void Swap(Wptr<T, Allocator>& other) noexcept;

private:
    typename Sptr<T, Allocator>::_ControlBlock* _cb = nullptr;
    Allocator _alloc{};

    void Release() noexcept;
};

template <class T, FuegoAllocator Allocator>
inline Wptr<T, Allocator>::Wptr(const Wptr<T, Allocator>& other) noexcept
    : _cb(other._cb)
{
    if (_cb)
        ++(_cb->_weak);
}

template <class T, FuegoAllocator Allocator>
inline Wptr<T, Allocator>::Wptr(Wptr<T, Allocator>&& other) noexcept
    : _cb(std::exchange(other._cb, nullptr))
{
}

template <class T, FuegoAllocator Allocator>
inline Wptr<T, Allocator>& Wptr<T, Allocator>::operator=(const Wptr<T, Allocator>& other) noexcept
{
    if (this == &other)
        return *this;

    Release();

    _cb = other._cb;

    if (_cb)
        ++(_cb->_weak);

    return *this;
}

template <class T, FuegoAllocator Allocator>
inline Wptr<T, Allocator>& Wptr<T, Allocator>::operator=(Wptr<T, Allocator>&& other) noexcept
{
    if (this == &other)
        return *this;

    Release();

    _cb = std::exchange(other._cb, nullptr);

    return *this;
}

template <class T, FuegoAllocator Allocator>
inline Wptr<T, Allocator>::~Wptr()
{
    Release();
}

template <class T, FuegoAllocator Allocator>
inline Wptr<T, Allocator>::Wptr(const Sptr<T, Allocator>& shared) noexcept
    : _cb(shared._cb)
{
    if (_cb)
        ++(_cb->_weak);
}

template <class T, FuegoAllocator Allocator>
inline Wptr<T, Allocator>& Wptr<T, Allocator>::operator=(const Sptr<T, Allocator>& shared) noexcept
{
    Release();

    _cb = shared._cb;

    if (_cb)
        ++(_cb->_weak);

    return *this;
}

template <class T, FuegoAllocator Allocator>
inline Sptr<T, Allocator> Wptr<T, Allocator>::Lock() const noexcept
{
    if (_cb && _cb->_strong > 0)
        return Sptr<T, Allocator>(_cb);

    return Sptr<T, Allocator>();
}

template <class T, FuegoAllocator Allocator>
inline bool Wptr<T, Allocator>::Expired() const noexcept
{
    return !_cb || _cb->_strong == 0;
}

template <class T, FuegoAllocator Allocator>
inline void Wptr<T, Allocator>::Release() noexcept
{
    if (!_cb)
        return;

    --(_cb->_weak);

    if (_cb->_weak == 0 && _cb->_strong == 0)
    {
        std::destroy_at(_cb);
        _alloc.deallocate(reinterpret_cast<uint8_t*>(_cb), sizeof(typename Sptr<T, Allocator>::_ControlBlock));
    }

    _cb = nullptr;
}

template <class T, FuegoAllocator Allocator>
inline void Wptr<T, Allocator>::Swap(Wptr<T, Allocator>& other) noexcept
{
    std::swap(_cb, other._cb);
}


template <ArrayType T, FuegoAllocator Allocator>
class FUEGO_API Wptr<T, Allocator>
{
public:
    Wptr(const Wptr<T, Allocator>& other) noexcept;
    Wptr(Wptr<T, Allocator>&& other) noexcept;

    Wptr<T, Allocator>& operator=(const Wptr<T, Allocator>& other) noexcept;
    Wptr<T, Allocator>& operator=(Wptr<T, Allocator>&& other) noexcept;

    ~Wptr();

    explicit Wptr(const Sptr<T, Allocator>& shared) noexcept;
    Wptr<T, Allocator>& operator=(const Sptr<T, Allocator>& shared) noexcept;

    Sptr<T, Allocator> Lock() const noexcept;

    bool Expired() const noexcept;

    void Swap(Wptr<T, Allocator>& other) noexcept;

private:
    typename Sptr<T, Allocator>::_ControlBlock* _cb = nullptr;
    Allocator* _alloc = nullptr;
    size_t _size = 0;

    void Release() noexcept;
};

template <ArrayType T, FuegoAllocator Allocator>
inline Wptr<T, Allocator>::Wptr(const Wptr<T, Allocator>& other) noexcept
    : _cb(other._cb)
    , _size(other._size)
{
    if (_cb)
        ++(_cb->_weak);
}

template <ArrayType T, FuegoAllocator Allocator>
inline Wptr<T, Allocator>::Wptr(Wptr<T, Allocator>&& other) noexcept
    : _cb(std::exchange(other._cb, nullptr))
    , _size(std::exchange(other._size, 0))
{
}

template <ArrayType T, FuegoAllocator Allocator>
inline Wptr<T, Allocator>& Wptr<T, Allocator>::operator=(const Wptr<T, Allocator>& other) noexcept
{
    if (this == &other)
        return *this;

    Release();

    _cb = other._cb;
    _size = other._size;

    if (_cb)
        ++(_cb->_weak);

    return *this;
}

template <ArrayType T, FuegoAllocator Allocator>
inline Wptr<T, Allocator>& Wptr<T, Allocator>::operator=(Wptr<T, Allocator>&& other) noexcept
{
    if (this == &other)
        return *this;

    Release();

    _cb = std::exchange(other._cb, nullptr);
    _size = std::exchange(other._size, 0);

    return *this;
}

template <ArrayType T, FuegoAllocator Allocator>
inline Wptr<T, Allocator>::~Wptr()
{
    Release();
}

template <ArrayType T, FuegoAllocator Allocator>
inline Wptr<T, Allocator>::Wptr(const Sptr<T, Allocator>& shared) noexcept
    : _cb(shared._cb)
    , _size(shared._size)
{
    if (_cb)
        ++(_cb->_weak);
}

template <ArrayType T, FuegoAllocator Allocator>
inline Wptr<T, Allocator>& Wptr<T, Allocator>::operator=(const Sptr<T, Allocator>& shared) noexcept
{
    Release();

    _cb = shared._cb;
    _size = shared._size;

    if (_cb)
        ++(_cb->_weak);

    return *this;
}

template <ArrayType T, FuegoAllocator Allocator>
inline Sptr<T, Allocator> Wptr<T, Allocator>::Lock() const noexcept
{
    if (_cb && _cb->_strong > 0)
        return Sptr<T, Allocator>(_size, _cb);

    return Sptr<T, Allocator>();
}

template <ArrayType T, FuegoAllocator Allocator>
inline bool Wptr<T, Allocator>::Expired() const noexcept
{
    return !_cb || _cb->_strong == 0;
}

template <ArrayType T, FuegoAllocator Allocator>
inline void Wptr<T, Allocator>::Release() noexcept
{
    if (!_cb)
        return;

    --(_cb->_weak);

    if (_cb->_weak == 0 && _cb->_strong == 0)
    {
        std::destroy_at(_cb);
        _alloc->deallocate(reinterpret_cast<uint8_t*>(_cb), sizeof(typename Sptr<T, Allocator>::_ControlBlock));
    }

    _cb = nullptr;
    _size = 0;
}

template <ArrayType T, FuegoAllocator Allocator>
inline void Wptr<T, Allocator>::Swap(Wptr<T, Allocator>& other) noexcept
{
    std::swap(_cb, other._cb);
    std::swap(_size, other._size);
}

}  // namespace Fuego
