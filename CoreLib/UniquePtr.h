#pragma once


#include "CoreLibConcepts.h"

#include <Core.h>


namespace Fuego
{
    template<class T, class Deleter = std::default_delete<T>> requires IsDefaultCompatibleDeleter<T, Deleter>
    class UniquePtr
    {
    public:
        FUEGO_NON_COPYABLE(UniquePtr)

        UniquePtr() = default;
        explicit UniquePtr(std::remove_extent_t<T>* ptr) noexcept;

        UniquePtr(UniquePtr<T, Deleter>&& other) noexcept;
        UniquePtr& operator=(UniquePtr<T, Deleter>&& other) noexcept;

        ~UniquePtr();

        std::remove_extent_t<T> const * Get() const noexcept;
        std::remove_extent_t<T>* Get() noexcept;

        void Reset(std::remove_all_extents_t<T>* ptr = nullptr) noexcept;
        std::remove_extent_t<T>* Release() noexcept;

        void Swap(UniquePtr<T, Deleter>& other) noexcept;

        Deleter GetDeleter() const noexcept;

        explicit operator bool() const noexcept;

        std::remove_extent_t<T> const & operator*() const noexcept;
        std::remove_extent_t<T>& operator*() noexcept;

        std::remove_extent_t<T> const * operator->() const noexcept;
        std::remove_extent_t<T>* operator->() noexcept;

        template<class U=T> requires std::is_array_v<U>
        std::remove_extent_t<T> const & operator[](size_t index) const noexcept;

        template<class U = T> requires std::is_array_v<U>
        std::remove_extent_t<T>& operator[](size_t index) noexcept;

    private:
        std::remove_extent_t<T>* _ptr = nullptr;
        Deleter _deleter {};
    };

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline UniquePtr<T, Deleter>::UniquePtr(std::remove_extent_t<T>* ptr) noexcept
        : _ptr(ptr)
    {
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline UniquePtr<T, Deleter>::UniquePtr(UniquePtr<T, Deleter>&& other) noexcept
        : _ptr(other._ptr), _deleter(std::move_if_noexcept(other._deleter))
    {
        other._ptr = nullptr;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline UniquePtr<T, Deleter>& UniquePtr<T, Deleter>::operator=(UniquePtr<T, Deleter>&& other) noexcept
    {
        if (this == &other)
            return *this;

        Reset(other.Release());
        _deleter = std::move_if_noexcept(other._deleter);

        return *this;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline UniquePtr<T, Deleter>::~UniquePtr()
    {
        Reset();
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline std::remove_extent_t<T> const* UniquePtr<T, Deleter>::Get() const noexcept
    {
        return _ptr;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline std::remove_extent_t<T>* UniquePtr<T, Deleter>::Get() noexcept
    {
        return _ptr;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline void UniquePtr<T, Deleter>::Reset(std::remove_all_extents_t<T>* ptr) noexcept
    {
        auto* oldPtr = _ptr;
        _ptr = ptr;

        if (oldPtr)
            _deleter(oldPtr);
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline std::remove_extent_t<T>* UniquePtr<T, Deleter>::Release() noexcept
    {
        auto* oldPtr = _ptr;
        _ptr = nullptr;

        return oldPtr;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline void UniquePtr<T, Deleter>::Swap(UniquePtr<T, Deleter>& other) noexcept
    {
        std::swap(_ptr, other._ptr);
        std::swap(_deleter, other._deleter);
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline Deleter UniquePtr<T, Deleter>::GetDeleter() const noexcept
    {
        return _deleter;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline UniquePtr<T, Deleter>::operator bool() const noexcept
    {
        return _ptr != nullptr;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline std::remove_extent_t<T> const& UniquePtr<T, Deleter>::operator*() const noexcept
    {
        FU_ASSERT(_ptr);
        return *_ptr;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline std::remove_extent_t<T>& UniquePtr<T, Deleter>::operator*() noexcept
    {
        FU_ASSERT(_ptr);
        return *_ptr;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline std::remove_extent_t<T> const* UniquePtr<T, Deleter>::operator->() const noexcept
    {
        return _ptr;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline std::remove_extent_t<T>* UniquePtr<T, Deleter>::operator->() noexcept
    {
        return _ptr;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    template<class U> requires std::is_array_v<U>
    inline std::remove_extent_t<T> const& UniquePtr<T, Deleter>::operator[](size_t index) const noexcept
    {
        FU_ASSERT(_ptr);
        return _ptr[index];
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    template<class U> requires std::is_array_v<U>
    inline std::remove_extent_t<T>& UniquePtr<T, Deleter>::operator[](size_t index) noexcept
    {
        FU_ASSERT(_ptr);
        return _ptr[index];
    }

}
