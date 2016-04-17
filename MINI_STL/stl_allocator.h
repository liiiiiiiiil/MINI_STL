#ifndef _STL_ALLOCATOR_H_
#define _STL_ALLOCATOR_H_
#include "stl_alloc.h"
#include "stl_construct.h"
#include <cassert>
#include <new>
namespace MINI_STL{
	template<class T>
	class allocator{
	public:
		typedef T value_type;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T&   reference;
		typedef const T&  const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		template <class _Other>
		struct rebind
		{
			typedef allocator<_Other> other;
		};

	public:
		static T* allocate();
		static T* allocate(size_t n);
		static void deallocate(T* ptr);
		static void deallocate(T* ptr, size_t n);
		static void construct(T* ptr);
		static void construct(T* ptr, const T& value);
		static void destroy(T* ptr);
		static void destroy(T* first, T* last);
		static void 
	};
	template<class T>
	T* allocator<T>::allocate(){
		return static_cast<T*>(alloc::allocate(sizeof(T)));
	}
	template<class T>
	T* allocator<T>::allocate(size_t n){
		return static_cast<T*>(alloc::allocate(n*sizeof(T)));
	}
	template<class T>
	void allocator<T>::deallocate(T* ptr){
		alloc::deallocate(static_cast<void*>(ptr), sizeof(T));
	}
	template<class T>
	void allocator<T>::deallocate(T* ptr,size_t n){
		if (0 == n)return;
		alloc::deallocate(static_cast<void*>(ptr), n*sizeof(T));
	}
	template<class T>
	void allocator<T>::construct(T *ptr){
		new(ptr)T();
	}
	template <class T>
	void allocator<T>::construct(T* ptr, const T& value){
		new(ptr)T(value);
	}
	template <class T>
	void allocator<T>::destroy(T* ptr){
		ptr->~T();
	}
	template<class T>
	void allocator<T>::destroy(T* first, T* last){
		for (; first != last; ++first){
			first->~T();
		}
	}
}

#endif