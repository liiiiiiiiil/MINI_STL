/*
*deque and the iterator of deque
*2016/4/26
*icesunL
*/
#ifndef STL_DEQUE_H_
#define STL_DEQUE_H_
#include "stl_alloc.h"
#include "stl_iterator.h"
#include "stl_algorithm.h"
#include "stl_type_traits.h"
#include "stl_allocator.h"
#include "stl_uninitialized.h"

namespace MINI_STL{
	template <class T, class Alloc = allocator<T>>
	class deque;
	namespace DEQUE_IT{
		template <class T>
		class deque_it :public iterator<bidirectional_iterator_tag, T>{
		private:
			//template <class T, class Alloc = alloctor<T>>
			///*friend class ::MINI_STL::deque;*/
		public:
			typedef     
			typedef     deque_it<T>            self;
			typedef     T**                           map_pointer;
   
			T* cur;
			T* first;
			T* last;
			map_pointer node;

		public:
			friend bool operator==(const deque_it& lhs, const deque_it&rhs){
				return (lhs.cur == rhs.cur);
			}

			friend bool operator !=(const deque_it& lhs, const deque_it&rhs){
				return !(lhs == rhs);
			}
			friend bool operator >(const self& lhs, const self& rhs){
				return (lhs.node > rhs.node) && (lhs.cur > rhs.node);
			}
			friend deque_it operator +(deque_it&it, difference_type n){
				return it += n;
			}

			friend deque_it operator -(deque_it& it, defference_type n){
				return it -= n;
			}
		public:
			reference operator*()const{ return *cur; }
			pointer operator->()const { return &(operator*()) };
			self& operator++();
			self operator++(int);
			self& operator--();
			self operator --(int);
			self& operator +=(difference_type n);
			self operator+(difference_type n);
			self& operator -= (difference_type n);
			self operator-(difference_type n);
			reference operator[](difference_type n)const{ return *(*this + n); }
		private:
			size_t DEQUE_BUFFER_SIZE(){
				return MINI_STL::DEQUE_BUFFER_SIZE;
			}
			void set_node(map_pointer new_node);
		
		};
	}
	//end of deque_it


	/*
	start of deque
	*/
	template<class T, class Alloc=allocator<T>>
	class deque{
	private:
		template <class T>
		friend class DEQUE_IT::deque_it;
	public:
		typedef  T                                value_type;
		typedef  T*                              pointer;
		typedef  T&                            reference;
		typedef  const pointer            const_pointer;
		typedef  const reference      const_reference;
		typedef  ptrdiff_t                     difference_type;
		typedef  size_t                           size_type;
		typedef  Alloc                        allocator_type;
		typedef  DEQUE_IT::deque_it<T>       iterator;
	protected:
		typedef   pointer*                    map_pointer;
		typedef   allocator<T>             data_allocator;
		typedef   allocator<pointer>    map_allocator;
	private:
		size_type                   map_size;
		iterator                      start;
		iterator                      finish;
		map_pointer              map;
	public:
		deque();
		deque(size_type n, const_reference value);
		deque(const_reference value);
		template <class InputIterator>
		deque(InputIterator first, InputIterator last);
		deque(const deque& other);
		deque& operator=(const deque& other);
		~deque();
	public:
		size_t size();
		size_t capacity();
		bool operator==(const deque& other);
		bool operator!=(const deque& other);
		void push_back(const_reference value);
		void push_front(const_reference value);
		void pop_front();
		void pop_back();
		void clear();
		iterator erase(iterator position);
		iterator erase(iterator first, iterator last);
		iterator insert(iterator position,size_type n,const_reference value);
		iterator insert(iterator position, const_reference value);
		template <class InputIterator>
		iterator insert(iterator positon,InputIterator first, InputIterator last);
	private:
		size_type  map_init_size()const{
			return DEQUE_FIRST_MAP_SIZE;
		}
		void reallocate_map(size_type nodes_to_add, bool add_at_front);
		void  create_map_and_nodes(size_type n=0);
		void  fill_initialize(size_type n, const_reference value);
		template <class InputIterator>
		void copy_initialize(InputIterator first, InputIterator last);
		template <class InputIterator>
		void deque_aux(InputIterator first, InputIterator last, std::false_type);
		void deque_aux(size_type n, const_reference value, std::true_type);
		void push_back_aux(const_reference value);
		void push_front_aux(const_reference value);
		void pop_front_aux();
		void pop_back_aux();
		void reserve_map_at_back(size_type nodes_to_add);//���pushback�Ƿ���Ҫ����map
		void reserve_map_at_front(size_type nodes_to_add);
		template <class integer>
		iterator insert_aux(iterator pos,integer n, const_reference value,std::true_type);
		template <class InputIterator>
		iterator insert_aux(iterator pos, InputIterator first, InputIterator last, std::false_type);

	public:
		template <class T,class Alloc>
		friend bool operator==(const deque<T, Alloc>&lhs, const deque<T, Alloc>&rhs);
		template <class T, class Alloc>
		friend bool operator!=(const deque<T, Alloc>&lhs, const deque<T, Alloc>&rhs);
	};
	static const enum{DEQUE_BUFFER_SIZE=20};
	static const enum{DEQUE_FIRST_MAP_SIZE=8};
}
#include "deque.impl.h"
#endif
