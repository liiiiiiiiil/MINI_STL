//#ifndef _STL_REVERSEITERATOR_H_
//#define _STL_REVERSEITERATOR_H_
//#include "stl_iterator.h"
//namespace MINI_STL{
//	template <class Iterator>
//	class reverse_iterator{
//	public:
//		typedef Iterator iterator_type;
//		typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
//		typedef typename iterator_traits<Iterator>::value_type value_type;
//		typedef typename iterator_traits<Iterator>::difference_type difference_type;
//		typedef typename iterator_traits<Iterator>::pointer pointer;
//		typedef const pointer const_pointer;
//		typedef typename iterator_traits<Iterator>::reference reference;
//		typedef const reference const_reference;
//	private:
//		Iterator base_;
//		Iterator cur_;
//	public:
//		reverse_iterator() :base_(0), cur_(0){}
//		explicit reverse_iterator(const iterator_type& it) :base_(it){
//			auto temp = it;
//			cur_ = --temp;
//		}
//		template <class Iterator>
//		reverse_iterator(const reverse_iterator<Iterator>& r_it){
//			base_ = (iterator_type)r_it.base_;
//			auto temp = base_;
//			cur_ = --temp;
//		}
//		iterator_type base(){ return base_; }
//		reference operator*(){ return *cur_; }
//		const_reference operator*()const{ return *cur_; }
//		pointer operator->(){ return &(operator*()); }
//		const_pointer operator->()const{ return &(operator*()); }
//		reverse_iterator& operator++(){
//			--cur_;
//			--base_;
//			return *this;
//		}
//		reverse_iterator& operator++(int){
//			reverse_iterator temp = *this;
//			++(*this);
//			return temp
//		}
//		reverse_iterator& operator--(){
//			++cur_;
//			++base_;
//			return *this;
//		}
//		reverse_iterator& operator--(int){
//			reverse_iterator temp = *this;
//			--(*this);
//			return temp;
//		}
//		reference operator[](difference_type n){
//			return base()[-n - 1];
//		}
//
//
//
//
//
//
//
//
//
//	};
//}
//
//#endif