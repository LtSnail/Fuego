#pragma once

#include "Engine/Fuego/Core.h"

#include <memory>
#include <utility>


namespace Fuego
{
	template<class DeleterType, class PointerType>
	concept DeleterOf =
	requires (DeleterType deleter, PointerType * ptr)
	{
			{ deleter(ptr) } -> std::convertible_to<void>;
	} 
	||
	(std::is_array_v<PointerType> && requires (DeleterType deleter, PointerType ptr)
	{
		{ deleter(ptr) } -> std::convertible_to<void>;
	});

	template<class T, class Deleter = std::default_delete<T>> requires DeleterOf<Deleter, T>
	class FUEGO_API UniquePtr
	{
	public:
		FUEGO_NON_COPYABLE(UniquePtr)

		UniquePtr() = default;
		explicit UniquePtr(std::remove_extent_t<T>* ptr) noexcept;

		~UniquePtr();

		UniquePtr(UniquePtr&& Other) noexcept;
		UniquePtr& operator=(UniquePtr&& Other) noexcept;

		T const & operator* () const noexcept;
		T& operator* () noexcept;

		T const * operator->() const noexcept;
		T* operator->() noexcept;

		explicit operator bool() const noexcept;

		void Swap(UniquePtr<T>& Other) noexcept;

		T* Release() noexcept;
		void Reset(T* ptr = nullptr) noexcept;

		T const * Get() const noexcept;
		T* Get() noexcept;

		Deleter const & GetDeleter() const noexcept;
		Deleter& GetDeleter() noexcept;

		template<class U = T>
		std::remove_extent_t<U> const & operator[](size_t Index) const noexcept
		requires std::is_array_v<U>;

		template<class U = T>
		std::remove_extent_t<U>& operator[](size_t Index) noexcept
		requires std::is_array_v<U>;

	private:
		std::remove_extent_t<T>* _ptr = nullptr;
		Deleter _deleter {};
	};

	template<class T, class Deleter> requires DeleterOf<Deleter, T>
	inline UniquePtr<T, Deleter>::UniquePtr(std::remove_extent_t<T>* ptr) noexcept
		: _ptr(ptr)
	{
	}

	template<class T, class Deleter> requires DeleterOf<Deleter, T>
	inline UniquePtr<T, Deleter>::~UniquePtr()
	{
		if (_ptr)
		{
			_deleter(_ptr);
		}
	}

	template<class T, class Deleter> requires DeleterOf<Deleter, T>
	inline UniquePtr<T, Deleter>::UniquePtr(UniquePtr<T, Deleter>&& Other) noexcept
		: _ptr(Other.Release()), _deleter(std::move_if_noexcept(Other._deleter))
	{
	}

	template<class T, class Deleter> requires DeleterOf<Deleter, T>
	inline UniquePtr<T, Deleter>& UniquePtr<T, Deleter>::operator=(UniquePtr<T, Deleter>&& Other) noexcept
	{
		if (this != &Other)
		{
			Reset(Other.Release());
			_deleter = std::move_if_noexcept(Other._deleter);
		}

		return *this;
	}

	template<class T, class Deleter> requires DeleterOf<Deleter, T>
	inline T const& UniquePtr<T, Deleter>::operator*() const noexcept
	{
		FU_ASSERT(_ptr, "Null pointer dereferencing");
		return *_ptr;
	}

	template<class T, class Deleter> requires DeleterOf<Deleter, T>
	inline T& UniquePtr<T, Deleter>::operator*() noexcept
	{
		FU_ASSERT(_ptr, "Null pointer dereferencing");
		return *_ptr;
	}

	template<class T, class Deleter> requires DeleterOf<Deleter, T>
	inline T const* UniquePtr<T, Deleter>::operator->() const noexcept
	{
		return _ptr;
	}

	template<class T, class Deleter> requires DeleterOf<Deleter, T>
	inline T* UniquePtr<T, Deleter>::operator->() noexcept
	{
		return _ptr;
	}

	template<class T, class Deleter> requires DeleterOf<Deleter, T>
	inline UniquePtr<T, Deleter>::operator bool() const noexcept
	{
		return _ptr != nullptr;
	}

	template<class T, class Deleter> requires DeleterOf<Deleter, T>
	inline void UniquePtr<T, Deleter>::Swap(UniquePtr<T>& Other) noexcept
	{
		std::swap(_ptr, Other._ptr);
		std::swap(_deleter, Other._deleter);
	}

	template<class T, class Deleter> requires DeleterOf<Deleter, T>
	inline T* UniquePtr<T, Deleter>::Release() noexcept
	{
		T* Temp = _ptr;
		_ptr = nullptr;
		return Temp;
	}

	template<class T, class Deleter> requires DeleterOf<Deleter, T>
	inline void UniquePtr<T, Deleter>::Reset(T* ptr) noexcept
	{
		T* OldPtr = _ptr;
		_ptr = ptr;

		if (OldPtr)
		{
			_deleter(OldPtr);
		}
	}

	template<class T, class Deleter> requires DeleterOf<Deleter, T>
	inline T const* UniquePtr<T, Deleter>::Get() const noexcept
	{
		return _ptr;
	}

	template<class T, class Deleter> requires DeleterOf<Deleter, T>
	inline T* UniquePtr<T, Deleter>::Get() noexcept
	{
		return _ptr;
	}

	template<class T, class Deleter> requires DeleterOf<Deleter, T>
	inline Deleter const& UniquePtr<T, Deleter>::GetDeleter() const noexcept
	{
		return _deleter;
	}

	template<class T, class Deleter> requires DeleterOf<Deleter, T>
	inline Deleter& UniquePtr<T, Deleter>::GetDeleter() noexcept
	{
		return _deleter;
	}

	template<class T, class Deleter>  requires DeleterOf<Deleter, T>
	template<class U>
	inline std::remove_extent_t<U> const& UniquePtr<T, Deleter>::operator[](size_t Index) const noexcept
	requires std::is_array_v<U>
	{
		return _ptr[Index];
	}

	template<class T, class Deleter> requires DeleterOf<Deleter, T>
	template<class U>
	inline std::remove_extent_t<U>& UniquePtr<T, Deleter>::operator[](size_t Index) noexcept
		requires std::is_array_v<U>
	{
		return _ptr[Index];
	}

}
