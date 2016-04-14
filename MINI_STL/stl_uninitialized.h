#ifndef _STL_UNINITIALIZED_H_
#define _STL_UNINITIALIZED_H_
#include "stl_iterator.h"
#include "stl_type_traits.h"
#include "stl_alloc.h"
#include "stl_construct.h"
#include"stl_algorithm.h"
namespace MINI_STL{
	//copy
	template <class InputIterator, class ForwardIterator>
	ForwardIterator uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator
		result, _true_type){
		memcpy(result, first, (last - first)*sizeof(*first));
		return result + (last - first);
	}
	template<class InputIterator,class ForwardIterator>
	ForwardIterator uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator
		result, _false_type){
		int i = 0;
		for (; first != last; first++, i++){
			construct(result + i, *first);
		}
		return result + i;
	}
	template <class InputIterator,class ForwardIterator>
	ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result){
		typedef typename _type_traits<typename iterator_traits<InputIterator>::value_type>::is_POD_type is_POD_type;
		return uninitialized_copy_aux(first, last, result, is_POD_type());
	}
	inline char* uninitialized_copy(const char* first, const char* last, char* result){
		memmove(result, first, last - first);
		return result + (last - first);
	}
	inline wchar_t* uninitialized_copy(const wchar_t* first, const wchar_t* last, wchar_t* result){
		memmove(result, first, sizeof(wchar_t)*(last - first));
		return result + (last - first);
	}

	//fill
	template <class ForwardIterator, class T>
	void uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& value, _true_type){
		fill(first, last, value);
	}
	template <class ForwardIterator, class T>
	void uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& value, _false_type){
		for (; first != last; ++first){
			construct(first, value);
		}
	}
	template<class ForwardIterator, class T>
	void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value){
		typedef typename _type_traits<typename iterator_traits< ForwardIterator>::value_type>::is_POD_type is_POD_type;
		uninitialized_copy_aux(first, last, value, is_POD_type());
	}
	//fill_n
	template <class ForwardIterator,class Size,class T>
	ForwardIterator uninitialized_fill_n_aux(ForwardIterator first, Size n, const T&value, _true_type){
		return fill_n(first, n, value);
	}
	template <class ForwardIterator, class Size,class T>
	ForwardIterator uninitialized_fill_n_aux(ForwardIterator first, Size n, const T&value, _false_type){
		ForwardIterator cur = first;
		for (; n > 0; --n, ++cur){
			construct(&*cur, value);
		}
		return cur;
	}
	template <class ForwardIterator ,class Size,class T>
	ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& value){
		typedef typename _type_traits<typename iterator_traits<ForwardIterator>::value_type>::is_POD_type
			is_POD_type;
		return uninitialized_fill_n_aux(first, n, value, is_POD_type());
	}
	///////////////////////////////////////////////////////          copy_backward
	template<class InputIterator ,class ForwardIterator>
	ForwardIterator uninitialized_copy_backward_aux(InputIterator first, InputIterator last, 
		ForwardIterator result,_false_type){
		int i = 1;
		--last;
		for (; last != first - 1; --last, ++i){
			construct((result - i), *last);
		}
		return result - i;
	}
	template<class InputIterator, class ForwardIterator>
	ForwardIterator uninitialized_copy_backward_aux(InputIterator first, InputIterator last,
		ForwardIterator result, _true_type){
		int i = 1;
		--last;
		for (; last != first - 1; --last, ++i){
			construct((result - i), *last);
		}
		return result - i;
	}
	template<class InputIterator,class ForwardIterator>
	ForwardIterator uninitialized_copy_backward(InputIterator first, InputIterator last, ForwardIterator result){
		typedef typename _type_traits<typename iterator_traits<InputIterator>::value_type>::is_POD_type 
			is_POD_type;
		return uninitialized_copy_backward_aux(first, last, result, is_POD_type());
	}
}
 
#endif