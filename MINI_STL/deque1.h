#ifndef STL_DEQUE1_H_
#define STL_DEQUE1_H_
#include "stl_alloc.h"
#include "stl_iterator.h"
#include "stl_algorithm.h"
#include "stl_type_traits.h"
#include "stl_allocator.h"

namespace MINI_STL{
	template <class T,class Alloc=allocator<T>>
	class deque;

	namespace DEQUE_DETAIL{
		/*some global variables */
		static const enum{FIRST_BLOCK_NUM=5};
		static const enum{BLOCK_SIZE=10};


		template <class T>
		class deque_it :public iterator<bidirectional_iterator_tag, T>{
		private:
			template <class T, class Alloc = alloctor<T>>
			friend class ::MINI_STL::deque;
		private:
			pointer* current_block;
			size_t current_block_index;
			size_t first;
			size_t last;
		public:
			friend bool operator==(const deque_it& lhs, const deque_it&rhs){
				return (lhs.current_block == rhs.current_block) && (lhs.current_block_index == rhs.current_block_index);
			}
			friend bool operator !=(const deque_it& lhs, const deque_it&rhs){
				return !(lhs == rhs);
			}
			friend deque_it operator +(deque_it&it, difference_type n){
				return it += n;
			}
			friend deque_it operator -(deque_it& it, defference_type n){
				return it -= n;
			}
		public:
			deque_it();
			deque_it(pointer* block, size_t index);

			reference operator*()const{
				return (*current_block)[current_block_index];
			}
			pointer operator ->()const{
				return &**this;
			};

			deque_it& operator ++();
			deque_it operator ++(int);

			deque_it& operator--();
			deque_it operator--(int);

			deque_it& operator +=(difference_type n);
			deque_it& operator -=(difference_type n);

			pointer* get_block_address()const;
			size_t get_block_index()const;
			void set_block_address(pointer* new_current_block);
		};
	}//end of deque_it


#define BLOCK_NUM  MINI_STL::DEQUE_DETAIL::FIRST_BLOCK_NUM

		/*
		deque
		*/
		template<class T,class Alloc>
		class deque{
		private:
			template <class T>
			friend class MINI_STL::DEQUE_DETAIL::deque_it;
		public:
			typedef Alloc   allocator_type;

			typedef allocator_type::value_type value_type;
			typedef allocator_type::size_type size_type;
			typedef allocator_type::difference_type difference_type;
			typedef allocator_type::pointer pointer;
			typedef allocator_type::const_pointer const_pointer;
			typedef allocator_type::reference reference;
			typedef allocator_type::const_reference const_reference;

			typedef MINI_STL::DEQUE_DETAIL::deque_it<T> iterator;
			typedef MINI_STL::DEQUE_DETAIL::deque_it<const T> const_iterator;
		private:
			allocator_type data_alloc;
			typename allocator_type::template rebind<pointer>::other data_alloc_out;
			pointer* first_block;
			pointer* last_block;
			iterator begin_iterator;
			iterator end_iterator;
			size_type value_num;//元素总个数
			static size_t block_size;//每个block所拥有的空间大小
		public:
			explicit deque(const allocator_type&a = allocator_type());
			explicit deque(size_type s, const_reference v = value_type(), const allocator_type& a = allocator_type());
			explicit deque(const deque& rhs);
			deque& operator=(const deque& rhs);
			~deque();
		public:
			reference operator[](size_type index);
			const_reference operator[](size_type index);
			reference at(size_type index);
			const_reference at(size_type index);
			reference front();
			const_reference front()const;
			reference back();
			const_reference back()const;

		public:
			iterator begin();
			const_iterator begin()const;
			iterator end();
			const_iterator end()const;
		public:
			void clear();
			void empty();
			iterator erase(iterator position);
			iterator insert(iterator position, const_reference value);
			void push_front(const_reference value);
			void pop_front();
			void push_back(const_reference value);
			void pop_back();
		public:
			size_type size()const;
			void swap(deque& rhs);
		private:
			void resize(size_type newSize, const_reference value = value_type());
		};
		template <class T,class Alloc>
	    size_t deque<T, Alloc>::block_size = DEQUE_DETAIL::BLOCK_SIZE;
}

#include "deque1.impl.h"

#endif
