#ifndef _STL_CONSTRUCT_H_
#define _STL_CONSTRUCT_H_

#include <new>
#include "stl_type_traits.h"
#include "stl_iterator.h"

namespace MINI_STL{

	template <class T1, class T2>
	inline void construct(T1* p, const T2& value) {
		new ((void*)p) T1(value);
	}

	template <class T>
	inline void construct(T* p) {//在p指向的空间构造对象，这个和::operarte new不一样
		new ((void*)p) T();
	}

	template <class T>
	inline void destroy(T* p) {
		p->~T();
	}

	template <class ForwardIterator>//false_type,不是trivial_destructor
	inline void destroy_aux(ForwardIterator first, ForwardIterator last, _false_type)
	{
			for (; first != last; ++first)
				destroy(&*first);
		}

	template <class ForwardIterator>//true_type,是trivial_destructor
	inline void destroy_aux(ForwardIterator, ForwardIterator, _true_type) {}

	template <class ForwardIterator, class T>
	inline void _destroy(ForwardIterator first, ForwardIterator last, T*)
	{
		typedef typename _type_traits<T>::has_trivial_destructor trivial_destructor;
		destroy_aux(first, last, trivial_destructor());
	}

	template <class ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last) {
		_destroy(first, last, value_type(first));
	}

	inline void destroy(char*, char*){}
	inline void destroy(wchar_t*, wchar_t*){}

}

#endif