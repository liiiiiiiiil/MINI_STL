#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <assert.h>
//#include "stl_alloc.h"
#include "stl_construct.h"
#include "stl_iterator.h"
#include "stl_type_traits.h"
#include "stl_uninitialized.h"
#include "stl_algorithm.h"
#include "stl_allocator.h"
namespace MINI_STL{
	template <class T,class Alloc=allocator<T>>
	class vector{
	public:
		typedef T                            value_type;
		typedef value_type*            pointer;
		typedef const T&                const_pointer;
		typedef value_type*            iterator;
		typedef const iterator         const_iterator;
		typedef value_type&          reference;
		typedef const value_type& const_reference;
		typedef size_t                      size_type;
		typedef ptrdiff_t                  difference_type;
	protected:
		typedef  Alloc data_allocator;
		iterator start;
		iterator finish;
		iterator end_of_storage;
	public://构造,析构,拷贝和赋值函数
		vector() :start(0), finish(0), end_of_storage(0){}
		explicit vector(const size_type& n);
		vector(const size_type n,const value_type& x);
		template<class InputIterator>
		vector(InputIterator first, InputIterator last);
		vector(const vector&);
		vector(vector &&);
		vector& operator =(const vector& v);
		vector& operator =(vector&& v);
		~vector();
	public://访问元素相关
		reference operator[](const difference_type i){
			return *(begin() + i);
		}
		const_reference operator[](const difference_type i)const{
			return *(cbegin() + i);
		}
		reference front(){ return (begin()); }
		reference back(){ return (end()); }
		pointer data(){ return start; }
	public:
		bool operator==(const vector& v)const;
		bool operator!=(const vector & v)const;

	public://容量相关
		difference_type size()const{ return finish - start; }
		difference_type capacity()const{ return end_of_storage - start; }
		bool empty()const{ return finish == start; }
		void resize(size_type n, value_type x=value_type());
		void reserve(size_type n);
		void shrink_to_fit();
	public://迭代器相关
		iterator begin(){ return start; }
		const_iterator begin()const{ return start; }
		const_iterator cbegin()const{ return start; }
		iterator end(){ return finish; }
		const_iterator end()const{ return finish; }
		const_iterator cend()const{ return finish; }
		//没有写reverse

	public://具体操作，如插入，清空，删除等
		void clear();
		void swap(vector&);
		void push_back(const value_type&);
		void pop_back();
		iterator insert(iterator position, const value_type& value);//!
		void insert(iterator position, const size_type& n, const value_type& x);//!
		template<class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last);
		iterator erase(iterator position);//!	
		iterator erase(iterator first, iterator last);//!
	public:
		Alloc get_allocator(){ return data_allocator; }
	private://空间配置等函数
		void destroyAndDeallocateAll();
		void allocateAndFillN(const size_type n, const value_type& x);
		template<class InputIterator>
		void allocateAndCopy(InputIterator first, InputIterator last);

		template<class InputIterator>
		void vector_aux(InputIterator first, InputIterator last, std::false_type);
		template <class Integer>
		void vector_aux(Integer n, const value_type& x, std::true_type);

		template <class InputIterator>
		void insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type);
		template<class Integer>
		void insert_aux(iterator position, Integer n, const value_type& x, std::true_type);
		
		template <class InputIterator>
		void reallocateAndCopy(iterator position, InputIterator first, InputIterator last);
		void reallocateAndFillN(iterator position, const size_type& n, const value_type& x);
		size_type getNewCapacity(size_type len)const;//!

	public:
		template <class T,class Alloc>
		friend bool operator==(const vector<T, Alloc>&v1,const vector<T, Alloc>&v2);
		template <class T,class Alloc>
		friend bool operator!=(const vector<T, Alloc>&v1, const vector<T, Alloc>&v2);
	};
}
#include "Detail\vector.impl.h"
#endif