#pragma once

#include "Engine/Fuego/Core.h"

#include <memory>
#include <utility>


namespace Fuego
{
	template<class DeleterType, class PointerType>
	concept DeleterOf = requires (DeleterType deleter, PointerType* ptr)
	{
		{ deleter(ptr) } -> std::convertible_to<void>;
	};

	template<class T, class Deleter = std::default_delete<T>> requires DeleterOf<Deleter, T>
	class UniquePtr
	{
	public:
		UniquePtr() = default;
		explicit UniquePtr(T* Ptr) noexcept;

		~UniquePtr();

		UniquePtr(const UniquePtr&) = delete;
		UniquePtr& operator=(const UniquePtr&) = delete;

		UniquePtr(UniquePtr&& Other) noexcept;
		UniquePtr& operator=(UniquePtr&& Other) noexcept;

		T const & operator* () const noexcept;
		T& operator* () noexcept;

		T const * operator->() const noexcept;
		T* operator->() noexcept;

		explicit operator bool() const noexcept;

		void Swap(UniquePtr<T>& Other) noexcept;

		T* Release() noexcept;
		void Reset(T* Ptr = nullptr) noexcept;

		T const * Get() const noexcept;
		T* Get() noexcept;

		Deleter const & GetDeleter() const noexcept;
		Deleter& GetDeleter() noexcept;

	private:
		T* Ptr_ = nullptr;
		Deleter Deleter_ {};
	};

	template<class T, class Deleter> requires DeleterOf<Deleter, T>
	inline UniquePtr<T, Deleter>::UniquePtr(T* Ptr) noexcept
		: Ptr_(Ptr)
	{
	}

	template<class T, class Deleter>
	inline UniquePtr<T, Deleter>::~UniquePtr()
	{
		if (Ptr_)
		{
			Deleter_(Ptr_);
		}
	}

	template<class T, class Deleter> requires DeleterOf<Deleter, T>
	inline UniquePtr<T, Deleter>::UniquePtr(UniquePtr<T, Deleter>&& Other) noexcept
		: Ptr_(Other.Release()), Deleter_(std::move_if_noexcept(Other.Deleter_))
	{
	}

	template<class T, class Deleter> requires DeleterOf<Deleter, T>
	inline UniquePtr<T, Deleter>& UniquePtr<T, Deleter>::operator=(UniquePtr<T, Deleter>&& Other) noexcept
	{
		if (this != &Other)
		{
			Reset(Other.Release());
			Deleter_ = std::move_if_noexcept(Other.Deleter_);
		}

		return *this;
	}

	template<class T, class Deleter> requires DeleterOf<Deleter, T>
	inline T const& UniquePtr<T, Deleter>::operator*() const noexcept
	{
		FU_ASSERT(Ptr_, "Null pointer dereferencing");
		return *Ptr_;
	}

	template<class T, class Deleter> requires DeleterOf<Deleter, T>
	inline T& UniquePtr<T, Deleter>::operator*() noexcept
	{
		FU_ASSERT(Ptr_, "Null pointer dereferencing");
		return *Ptr_;
	}

	template<class T, class Deleter> requires DeleterOf<Deleter, T>
	inline T const* UniquePtr<T, Deleter>::operator->() const noexcept
	{
		return Ptr_;
	}

	template<class T, class Deleter> requires DeleterOf<Deleter, T>
	inline T* UniquePtr<T, Deleter>::operator->() noexcept
	{
		return Ptr_;
	}

	template<class T, class Deleter> requires DeleterOf<Deleter, T>
	inline UniquePtr<T, Deleter>::operator bool() const noexcept
	{
		return Ptr_ != nullptr;
	}

	template<class T, class Deleter> requires DeleterOf<Deleter, T>
	inline void UniquePtr<T, Deleter>::Swap(UniquePtr<T>& Other) noexcept
	{
		std::swap(Ptr_, Other.Ptr_);
		std::swap(Deleter_, Other.Deleter_);
	}

	template<class T, class Deleter> requires DeleterOf<Deleter, T>
	inline T* UniquePtr<T, Deleter>::Release() noexcept
	{
		T* Temp = Ptr_;
		Ptr_ = nullptr;
		return Temp;
	}

	template<class T, class Deleter> requires DeleterOf<Deleter, T>
	inline void UniquePtr<T, Deleter>::Reset(T* Ptr) noexcept
	{
		T* OldPtr = Ptr_;
		Ptr_ = Ptr;

		if (OldPtr)
		{
			Deleter_(OldPtr);
		}
	}

	template<class T, class Deleter> requires DeleterOf<Deleter, T>
	inline T const* UniquePtr<T, Deleter>::Get() const noexcept
	{
		return Ptr_;
	}

	template<class T, class Deleter> requires DeleterOf<Deleter, T>
	inline T* UniquePtr<T, Deleter>::Get() noexcept
	{
		return Ptr_;
	}

	template<class T, class Deleter> requires DeleterOf<Deleter, T>
	inline Deleter const& UniquePtr<T, Deleter>::GetDeleter() const noexcept
	{
		return Deleter_;
	}

	template<class T, class Deleter> requires DeleterOf<Deleter, T>
	inline Deleter& UniquePtr<T, Deleter>::GetDeleter() noexcept
	{
		return Deleter_;
	}

}
