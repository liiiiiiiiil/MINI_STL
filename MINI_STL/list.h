#ifndef _LIST_H_
#define _LIST_H_
#include "stl_allocator.h"
#include "stl_type_traits.h"
#include "stl_algorithm.h"
#include "stl_uninitialized.h"
#include "stl_iterator.h"
#include "stl_functional.h"
#include <iostream>
namespace MINI_STL{
	template <class T>
	class list;
	namespace LIST_DETAIL{
		template<class T>
		struct list_node{
			T data;
			list_node* prev;
			list_node* next;
			list<T> *container;
			list_node(const T& d, list_node *p, list_node *n, list<T> *c) :
				data(d), prev(p), next(n), container(c){}
			bool operator ==(const list_node& n){
				return data == n.data && prev == n.prev && next == n.next && container == n.container;
			}
		};
		template<class T>
		class list_iterator{
		public:
			template <class T>
			friend class list;

			typedef list_iterator<T>  iterator;

			typedef bidirectional_iterator_tag  iterator_category;
			typedef T value_type;
			typedef T* pointer;
			typedef T& reference;
			typedef list_node<T>*  link_type;
			typedef  size_t size_type;
			typedef ptrdiff_t defference_type;

			link_type node;

			list_iterator(link_type& x) :node(x){}
			list_iterator(){}
			list_iterator(const iterator& x):node(x.node){}
			
			reference operator *(){ return node->data; }
			pointer operator ->(){ return &(operator*()); }

			list_iterator& operator++();
			list_iterator operator++(int);
			list_iterator& operator--();
			list_iterator operator--(int);

			/*template <class T>
			friend bool operator ==(const list_iterator<T>& lhs, const list_iterator<T>& rhs);*/
			template<class T>
			friend bool operator ==(const list_iterator<T>& lhs, const list_iterator<T>& rhs);
			template<class T>
			friend bool operator !=(const list_iterator<T>& lhs, const list_iterator<T>& rhs);
		};
	}
	template <class T>
	class list{
		template <class T>//friend 函数
		friend struct list_iterator;
	public://
		typedef allocator<LIST_DETAIL::list_node<T>> list_allocator;
		typedef LIST_DETAIL::list_node<T>* nodePtr;
	public://
		typedef T value_type;
		typedef T& reference;
		typedef LIST_DETAIL::list_iterator<T> iterator;
		typedef LIST_DETAIL::list_iterator<const T> const_iterator;
		typedef size_t size_type;
	private:
		iterator head;
		iterator tail;
	public://构造，析构函数
		list();
		explicit list(size_type n, const value_type& value = value_type());
		template<class InputIterator>
		list(InputIterator first, InputIterator last);
		list(const list&rhs);
		list& operator=(const list& rhs);
		~list();
	public://基本的简单操作
		bool empty()const { return head == tail; }
		size_type size()const;
		reference front(){ return head.node->data; }
		reference back(){ return tail.node->prev->data; }
		void push_front(const value_type& value);
		void pop_front();
		void push_back(const value_type& value);
		void pop_back();
		iterator begin();
		iterator end();
		const_iterator cbegin()const;
		const_iterator cend()const;
	public://插入，删除，合成，销毁等基本操纵
		iterator insert(iterator position, const value_type& value);
		void insert(iterator position, size_type n, const value_type& value);
		template<class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last);
		iterator erase(iterator position);
		iterator erase(iterator first, iterator last);
		void swap(list& rhs);
		void clear();
		void splice(iterator position, list& x);
		void splice(iterator position, list& x, iterator i);
		void splice(iterator position, list& x, iterator first, iterator last);
		void remove(const value_type& value);
		template <class Predicate>
		void remove_if(Predicate pred);
		void unique();
		template <class BinaryPredicate>
		void unique(BinaryPredicate binary_pred);
		void merge(list& rhs);
		template <class Compare>
		void merge(list& x, Compare comp);
		void sort();
		template <class Compare>
		void sort(Compare comp);
		void reverse();
	public://其他操作
		template<class T>
		friend void swap(list<T>& lhs, list<T>& rhs);
		template <class T>
		friend bool operator==(const list<T>& lhs, const list<T>&rhs);
		template <class T>
		friend bool operator!=(const list<T>& lhs, const list<T>& rhs);
	private://内部成员函数，涉及一些数据操作
		void constructor_aux(size_type n, const value_type& value, std::true_type);
		template <class InputIterator>
		void constructor_aux(InputIterator first, InputIterator last, std::false_type);
		nodePtr newNode(const T&value = T());
		void deleteNode(nodePtr ptr);
		void insert_aux(iterator position, size_type n, const value_type& value, std::true_type);
		template<class InputIterator>
		void insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type);
		const_iterator getConstIterator(iterator& it)const;
	};
}
#include "Detail\list.impl.h"
#endif