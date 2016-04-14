#ifndef _STL_ITERATOR_H_
#define _STL_ITERATOR_H_
#include <cstddef>
namespace MINI_STL{

	struct input_iterator_tag{};
	struct output_iterator_tag{};
	struct forward_iterator_tag:public input_iterator_tag{};
	struct bidirectional_iterator_tag:public forward_iterator_tag{};
	struct random_access_iterator_tag:public bidirectional_iterator_tag{};

	template<class T,class Distance>
	struct input_iterator{
		typedef input_iterator_tag iterator_category;
		typedef T value_type;
		typedef Distance difference_type;
		typedef T* pointer;
		typedef T& reference;
	};
	struct output_iterator{
		typedef output_iterator_tag iterator_category;
		typedef void value_type;
		typedef void difference_type;
		typedef void pointer;
		typedef void reference;
	};
	template <class T, class Distance> struct forward_iterator
	{
		typedef forward_iterator_tag	iterator_category;
		typedef T						value_type;
		typedef Distance				difference_type;
		typedef T*						pointer;
		typedef T&						reference;
	};
	template <class T, class Distance> struct bidirectional_iterator
	{
		typedef bidirectional_iterator_tag	iterator_category;
		typedef T						value_type;
		typedef Distance				difference_type;
		typedef T*						pointer;
		typedef T&						reference;
	};
	template <class T, class Distance> struct random_access_iterator
	{
		typedef random_access_iterator_tag	iterator_category;
		typedef T						value_type;
		typedef Distance				difference_type;
		typedef T*						pointer;
		typedef T&						reference;
	};
	template<class Category, class T, class Distance = ptrdiff_t,
	class Pointer = T*, class Reference = T&>
	struct iterator{
		typedef Category iterator_category;
		typedef T value_type;
		typedef Distance difference_type;
		typedef Pointer pointer;
		typedef Reference reference;
	};
	template <class Iterator>
	struct iterator_traits{
		typedef typename Iterator::iterator_category       iterator_category;
		typedef typename Iterator::value_type           value_type;
		typedef typename Iterator::difference_type        difference_type;
		typedef typename Iterator::pointer              pointer;
		typedef typename Iterator::reference            reference;
	};
	template <class T>
	struct iterator_traits<T*>{
		typedef random_access_iterator_tag  iterator_category;
		typedef T value_type;
		typedef ptrdiff_t difference_type;
		typedef T* pointer;
		typedef T& reference;
	};
	template <class T>
	struct iterator_traits<const T*>{
		typedef random_access_iterator_tag  iterator_category;
		typedef T value_type;
		typedef ptrdiff_t difference_type;
		typedef T* pointer;
		typedef T& reference;
	};
	template <class Iterator>
	inline typename iterator_traits<Iterator>::iterator_category
		iterator_category(const Iterator&){
			typedef typename iterator_traits<Iterator>::iterator_category category;
			return category();
		}
	template <class Iterator>
	inline typename iterator_traits<Iterator>::value_type
		value_type(const Iterator&){
			typedef typename iterator_traits<Iterator>::value_type value_type;
			return value_type();
		}
	template <class Iterator>
	inline typename iterator_traits<Iterator>::difference_type
		distance_type(const Iterator&){
			typedef typename iterator_traits<Iterator>::difference_type difference_type;
			return difference_type();
		}
	template<class InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type
		_distance(InputIterator first, InputIterator last, input_iterator_tag){
			typename iterator_traits<InputIterator>::difference_type n=0;
			while (first != last){
				++first;
				++n;
			}
			return n;
		}
	template<class RandomAccessIterator>
	inline typename iterator_traits<RandomAccessIterator>::difference_type
		_distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag){
			return last - first;
		}
	template<class InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type
		distance(InputIterator first, InputIterator last){
			typedef typename iterator_traits<InputIterator>::iterator_category category;
			return _distance(first, last, category());
		}
	


}
#endif