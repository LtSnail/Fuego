#pragma once


#include "CoreLibConcepts.h"

#include <Core.h>

#include <stdint.h>

namespace Fuego
{
    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    class SharedPtr;

    template<class T, class Deleter = std::default_delete<T>> requires IsDefaultCompatibleDeleter<T, Deleter>
    class FUEGO_API WeakPtr
    {
    public:

        WeakPtr() noexcept = default;
        WeakPtr(const WeakPtr<T, Deleter>& other) noexcept;
        WeakPtr(WeakPtr<T, Deleter>&& other) noexcept;

        WeakPtr<T, Deleter>& operator=(const WeakPtr<T, Deleter>& other) noexcept;
        WeakPtr<T, Deleter>& operator=(WeakPtr<T, Deleter>&& other) noexcept;

        ~WeakPtr();

        WeakPtr(const  SharedPtr<T, Deleter>& shared) noexcept;
        WeakPtr<T, Deleter>& operator=(const SharedPtr<T, Deleter>& shared) noexcept;

        SharedPtr<T, Deleter> Lock() const noexcept;

        bool Expired() const noexcept;
        void Release() noexcept;
        void Swap(WeakPtr<T, Deleter>& other) noexcept;

    private:
        typename SharedPtr<T, Deleter>::_ControlBlock* _control_block = nullptr;
    };

    template<class T, class Deleter = std::default_delete<T>> requires IsDefaultCompatibleDeleter<T, Deleter>
    class FUEGO_API SharedPtr
    {
    public:
        SharedPtr() noexcept = default;

        explicit SharedPtr(std::remove_extent_t<T>* ptr);
        SharedPtr(const SharedPtr<T, Deleter>& other) noexcept;
        SharedPtr(SharedPtr<T, Deleter>&& other) noexcept;

        SharedPtr<T, Deleter>& operator=(const SharedPtr<T, Deleter>& other) noexcept;
        SharedPtr<T, Deleter>& operator=(SharedPtr<T, Deleter>&& other) noexcept;

        ~SharedPtr();

        std::remove_extent_t<T> const * Get() const noexcept;
        std::remove_extent_t<T>* Get() noexcept;

        std::remove_extent_t<T>  const & operator*() const noexcept;
        std::remove_extent_t<T>& operator*() noexcept;

        std::remove_extent_t<T>  const * operator->() const noexcept;
        std::remove_extent_t<T>* operator->() noexcept;

        uint32_t UseCount() const noexcept;
        explicit operator bool() const noexcept;

        void Reset(std::remove_extent_t<T>* ptr = nullptr);
        void Release();

        void Swap(SharedPtr<T, Deleter>& other) noexcept;

        template<class U, class DeleterU> requires IsDefaultCompatibleDeleter<U, DeleterU>
        friend class WeakPtr;

        template<class U>
        bool operator==(std::remove_extent_t<U>* other) const noexcept;

        template<class U>
        bool operator<(std::remove_extent_t<U>* other) const noexcept;

        template<class U>
        bool operator>(std::remove_extent_t<U>* other) const noexcept;

        template<class U>
        bool operator<=(std::remove_extent_t<U>* other) const noexcept;

        template<class U>
        bool operator>=(std::remove_extent_t<U>* other) const noexcept;

        template<class U>
        bool operator!=(std::remove_extent_t<U>* other) const noexcept;

        bool operator==(std::remove_extent_t<T>* other) const noexcept;
        bool operator<(std::remove_extent_t<T>* other) const noexcept;
        bool operator>(std::remove_extent_t<T>* other) const noexcept;
        bool operator<=(std::remove_extent_t<T>* other) const noexcept;
        bool operator>=(std::remove_extent_t<T>* other) const noexcept;
        bool operator!=(std::remove_extent_t<T>* other) const noexcept;

        bool operator==(std::nullptr_t other) const noexcept;
        bool operator!=(std::nullptr_t other) const noexcept;

        template<class U = T> requires std::is_array_v<U>
        std::remove_extent_t<T> const & operator[](size_t index) const noexcept;

        template<class U = T> requires std::is_array_v<U>
        std::remove_extent_t<T>& operator[](size_t index) noexcept;

    private:
        struct _ControlBlock
        {
            std::remove_extent_t<T>* _ptr = nullptr;
            uint32_t _strong_count = 0; 
            uint32_t _weak_count = 0;
            Deleter _deleter {};

            explicit _ControlBlock(std::remove_extent_t<T>* ptr = nullptr)
                : _ptr(ptr), _strong_count(1)
            {}

            ~_ControlBlock()
            {
                if (_ptr)
                    _deleter(_ptr);
            }
        };

        _ControlBlock* _control_block = nullptr;

        SharedPtr<T, Deleter>::_ControlBlock* GetControlBlock() const noexcept;

        explicit SharedPtr(SharedPtr<T, Deleter>::_ControlBlock* cb) noexcept;
    };


    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline WeakPtr<T, Deleter>::WeakPtr(const WeakPtr<T, Deleter>& other) noexcept
        : _control_block(other._control_block)
    {
        if (_control_block)
            ++(_control_block->_weak_count);
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline WeakPtr<T, Deleter>::WeakPtr(WeakPtr<T, Deleter>&& other) noexcept
        : _control_block(std::exchange(other._control_block, nullptr))
    {
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline WeakPtr<T, Deleter>& WeakPtr<T, Deleter>::operator=(const WeakPtr<T, Deleter>& other) noexcept
    {
        if (this == &other)
            return *this;

        Release();

        _control_block = other._control_block;
        ++(_control_block->_weak_cout);

        return *this;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline WeakPtr<T, Deleter>& WeakPtr<T, Deleter>::operator=(WeakPtr<T, Deleter>&& other) noexcept
    {
        if (this == &other)
            return *this;

        Release();

        _control_block = std::exchange(other._control_block, nullptr);

        return *this;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline WeakPtr<T, Deleter>::~WeakPtr()
    {
        Release();
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline WeakPtr<T, Deleter>::WeakPtr(const SharedPtr<T, Deleter>& shared) noexcept
        : _control_block(shared.GetControlBlock())
    {
        ++(_control_block->_weak_count);
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline WeakPtr<T, Deleter>& WeakPtr<T, Deleter>::operator=(const SharedPtr<T, Deleter>& shared) noexcept
    {
        Release();
        _control_block = shared.GetControlBlock();
        ++(_control_block->_weak_count);

        return *this;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline SharedPtr<T, Deleter> WeakPtr<T, Deleter>::Lock() const noexcept
    {
        if (_control_block && _control_block->_strong_count > 0)
            return SharedPtr<T, Deleter>(_control_block);

        return SharedPtr<T, Deleter>();
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline bool WeakPtr<T, Deleter>::Expired() const noexcept
    {
        return !_control_block || _control_block->_strong_count == 0;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline void WeakPtr<T, Deleter>::Release() noexcept
    {
        if (_control_block)
            --(_control_block->_weak_count);

        _control_block = nullptr;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline void WeakPtr<T, Deleter>::Swap(WeakPtr<T, Deleter>& other) noexcept
    {
        std::swap(_control_block, other._control_block);
    }



    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline SharedPtr<T, Deleter>::SharedPtr(std::remove_extent_t<T>* ptr)
        : _control_block(ptr ? new _ControlBlock(ptr) : nullptr)
    {
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline SharedPtr<T, Deleter>::SharedPtr(const SharedPtr<T, Deleter>& other) noexcept
        : _control_block(other._control_block)
    {
        if (_control_block)
            ++(_control_block->_strong_count);
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline SharedPtr<T, Deleter>::SharedPtr(SharedPtr<T, Deleter>&& other) noexcept
        : _control_block(std::exchange(other._control_block, nullptr))
    {
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline SharedPtr<T, Deleter>& SharedPtr<T, Deleter>::operator=(const SharedPtr<T, Deleter>& other) noexcept
    {
        if (this == &other)
            return *this;

        Release();

        _control_block = other._control_block;

        if (_control_block)
            ++(_control_block->_strong_count);

        return *this;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline SharedPtr<T, Deleter>& SharedPtr<T, Deleter>::operator=(SharedPtr<T, Deleter>&& other) noexcept
    {
        if (this == &other)
            return *this;

        Release();

        _control_block = std::exchange(other._control_block, nullptr);

        return *this;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline SharedPtr<T, Deleter>::~SharedPtr()
    {
        Release();
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline std::remove_extent_t<T> const* SharedPtr<T, Deleter>::Get() const noexcept
    {
        return _control_block ? _control_block->_ptr : nullptr;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline std::remove_extent_t<T>* SharedPtr<T, Deleter>::Get() noexcept
    {
        return _control_block ? _control_block->_ptr : nullptr;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline std::remove_extent_t<T> const& SharedPtr<T, Deleter>::operator*() const noexcept
    {
        FU_ASSERT(_control_block);
        FU_ASSERT(_control_block->_ptr);
        return *(_control_block->_ptr);
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline std::remove_extent_t<T>& SharedPtr<T, Deleter>::operator*() noexcept
    {
        FU_ASSERT(_control_block);
        FU_ASSERT(_control_block->_ptr);
        return *(_control_block->_ptr);
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline std::remove_extent_t<T> const* SharedPtr<T, Deleter>::operator->() const noexcept
    {
        return _control_block ? _control_block->_ptr : nullptr;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline std::remove_extent_t<T>* SharedPtr<T, Deleter>::operator->() noexcept
    {
        return _control_block ? _control_block->_ptr : nullptr;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    template<class U>
    inline bool SharedPtr<T, Deleter>::operator==(std::remove_extent_t<U>* other) const noexcept
    {
        return _control_block ? _control_block->_ptr == other : false;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    template<class U>
    inline bool SharedPtr<T, Deleter>::operator<(std::remove_extent_t<U>* other) const noexcept
    {
        return _control_block ? _control_block->_ptr < other : false;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    template<class U>
    inline bool SharedPtr<T, Deleter>::operator>(std::remove_extent_t<U>* other) const noexcept
    {
        return _control_block ? _control_block->_ptr > other : false;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    template<class U>
    inline bool SharedPtr<T, Deleter>::operator<=(std::remove_extent_t<U>* other) const noexcept
    {
        return _control_block ? _control_block->_ptr <= other : false;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    template<class U>
    inline bool SharedPtr<T, Deleter>::operator>=(std::remove_extent_t<U>* other) const noexcept
    {
        return _control_block ? _control_block->_ptr >= other : false;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    template<class U>
    inline bool SharedPtr<T, Deleter>::operator!=(std::remove_extent_t<U>* other) const noexcept
    {
        return _control_block ? _control_block->_ptr != other : false;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline bool SharedPtr<T, Deleter>::operator==(std::remove_extent_t<T>* other) const noexcept
    {
        return _control_block ? _control_block->_ptr == other : false;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline bool SharedPtr<T, Deleter>::operator<(std::remove_extent_t<T>* other) const noexcept
    {
        return _control_block ? _control_block->_ptr < other : false;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline bool SharedPtr<T, Deleter>::operator>(std::remove_extent_t<T>* other) const noexcept
    {
        return _control_block ? _control_block->_ptr > other : false;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline bool SharedPtr<T, Deleter>::operator<=(std::remove_extent_t<T>* other) const noexcept
    {
        return _control_block ? _control_block->_ptr <= other : false;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline bool SharedPtr<T, Deleter>::operator>=(std::remove_extent_t<T>* other) const noexcept
    {
        return _control_block ? _control_block->_ptr >= other : false;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline bool SharedPtr<T, Deleter>::operator!=(std::remove_extent_t<T>* other) const noexcept
    {
        return _control_block ? _control_block->_ptr != other : false;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    template<class U> requires std::is_array_v<U>
    inline std::remove_extent_t<T> const& SharedPtr<T, Deleter>::operator[](size_t index) const noexcept
    {
        FU_ASSERT(_control_block);
        FU_ASSERT(_control_block->_ptr);
        return _control_block->_ptr[index];
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    template<class U> requires std::is_array_v<U>
    inline std::remove_extent_t<T>& SharedPtr<T, Deleter>::operator[](size_t index) noexcept
    {
        FU_ASSERT(_control_block);
        FU_ASSERT(_control_block->_ptr);
        return _control_block->_ptr[index];
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline uint32_t SharedPtr<T, Deleter>::UseCount() const noexcept
    {
        return _control_block ? _control_block->_strong_count : 0;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline SharedPtr<T, Deleter>::operator bool() const noexcept
    {
        return _control_block ? _control_block->_ptr != nullptr : false;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline void SharedPtr<T, Deleter>::Reset(std::remove_extent_t<T>* ptr)
    {
        Release();

        if (ptr)
            _control_block = new _ControlBlock(ptr);
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline void SharedPtr<T, Deleter>::Release()
    {
        if (!_control_block)
            return;

        --(_control_block->_strong_count);

        if (_control_block->_strong_count == 0 && _control_block->_weak_count == 0)
           delete _control_block;

        _control_block = nullptr;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline void SharedPtr<T, Deleter>::Swap(SharedPtr<T, Deleter>& other) noexcept
    {
        std::swap(_control_block, other._control_block);
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline bool SharedPtr<T, Deleter>::operator==(std::nullptr_t other) const noexcept
    {
        return _control_block ? _control_block->_ptr == other : true;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline bool SharedPtr<T, Deleter>::operator!=(std::nullptr_t other) const noexcept
    {
        return _control_block ? _control_block->_ptr != other : false;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline SharedPtr<T, Deleter>::_ControlBlock* SharedPtr<T, Deleter>::GetControlBlock() const noexcept
    {
        return _control_block;
    }

    template<class T, class Deleter> requires IsDefaultCompatibleDeleter<T, Deleter>
    inline SharedPtr<T, Deleter>::SharedPtr(SharedPtr::_ControlBlock* cb) noexcept
        : _control_block(cb)
    {
        ++(_control_block->_strong_count);
    }
}
