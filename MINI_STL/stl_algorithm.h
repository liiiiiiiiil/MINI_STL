#ifndef   _STL_ALGORITHM_H_
#define _STL_ALGORITHM_H_
#include <cstring>
namespace MINI_STL{

	//////////////////////////////////////////////////////////////////////         fill
	template <class ForwardIterator, class T>
	inline void fill(ForwardIterator first, ForwardIterator last, const T& value){
		for (; first != last; ++first){
			*first = value;
		}
	}
	inline void fill(char* first, char * last, const char& value){
		memset(first, static_cast<unsigned char>(value), last - first);
	}
	inline void fill(wchar_t* first, wchar_t * last, const wchar_t& value){
		memset(first, static_cast<unsigned char>(value), (last - first)*sizeof(wchar_t));
		//wchar_t大小和编译器相关，但是肯定比char大
	}
	///////////////////////////////////////////////////////////////////////////           fill_n
	template <class OutputIterator,class Size,class T>
	inline OutputIterator fill_n(OutputIterator first, Size n, const T& value){
		for (; n > 0; --n, ++first){
			*first = value;
		}
		return first;
	}
	template<class Size>
	inline char* fill_n(char* first, Size n, const char& value){
		memset(first, static_cast<unsigned char>(value), n);
		return first + n;		
	}

	template<class Size>
	inline wchar_t * fill_n(wchar_t* first, Size n, const wchar_t& value){
		memset(first, static_cast<unsigned char>(value), sizeof(wchar_t)*n);
		return first + n;
	}
	///////////////////////////////////////////////////////////////////////////   max
	template<class T>
     const T& max(const T& lhs, const T& rhs){
		return lhs > rhs ? lhs : rhs;
	}
	//////////////////////////////////////////////////////////////////////////// swap
	 template <class T>
	 void swap(T& lhs, T&rhs){
		 T& temp = lhs;
		 lhs = rhs;
		 rhs = temp;
	 }	

}

#endif
