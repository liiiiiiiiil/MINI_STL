//#ifndef DEQUE_IMPL_H_
//#define DEQUE_IMPL_H_

namespace MINI_STL{
	namespace DEQUE_DETAIL{
		template <class T>
		deque_it<T>::deque_it(){
			current_block = static_cast<pointer*>(0);
			current_block_index = 0;
			first = 0;
			last = BLOCK_SIZE - 1;
		}
		template <class T>
		deque_it<T>::deque_it(pointer* block, size_t block_index){
			current_block = block;
			current_block_index = block_index;
			first = 0;
			last = BLOCK_SIZE - 1;
		}

		template <class T>
		deque_it<T>& deque_it<T>::operator++(){
			if (++current_block_index == last){
				current_block_index = first;
				++current_block;
			}
			return*this;
		}
		template <class T>
		deque_it<T> deque_it<T>::operator++(int){
			auto temp = *this;
			++(*this);
			return temp;
		}

		template <class T>
		deque_it<T>& deque_it<T>::operator--(){
			if (current_block_index-- = first){
				current_block_index = last - 1;
				--current_block;
			}
			return *this;
		}
		template <class T>
		deque_it<T> deque<T>::operator--(int){
			auto temp = *this;
			--(*this);
			return temp;
		}

		template <class T>
		deque_it<T>& deque_it<T>::operator+=(deque_it<T>::difference_type n){
			current_block += n / (block_size - 1);
			int index_inc = n % (block_size - 1);
			if ((current_block_index + index_inc) >= last){
				current_block_index = (current_block_index + index) % (block_size - 1);
				++current_block;
			}
			else{
				current_block_index += index_inc;
			}
			return *this;
		}
		template <class T>
		deque_it<T>& deque_it<T>::operator-=(typename deque_it<T>::difference_type n){
			current_block -= n / (block_size - 1);
			int index_dec = d % (block_size - 1);
			if (current_block_index - index_dec < 0){
				current_block_index = last/*前闭后开*/ - (index_dec - current_block_index);
				--current_block;
			}
			else{
				current_block_index -= index_dec;
			}
			return *this;
		}
		template <class T>
		typename deque_it<T>::pointer*  deque_it<T>::get_block_address()const{
			return current_block;
		}
		template <class T>
		size_t deque_it<T>::get_block_index()const{
			return current_block_index;
		}
		template <class T>
		void deque_it<T>::set_block_address(typename deque_it<T>::pointer* newCurrent_block){
			current_block = newCurrent_block;
		}
	}//end of iterator
	
	
	//start of deque
	template <class T,class Alloc>
	explicit deque<T, Alloc>::deque(const allocator_type&a = allocator_type()) :data_alloc(a){
		value_num = 0;
		first_block = data_alloc_out.allocate(BLOCK_NUM);
		for (int i = 0; i < BLOCK_NUM; i++){
			*(first_block + i) = data_alloc.allocate(block_size);
		}

		last_block = first_block + BLOCK_NUM;//为什么不是+(BLOCK_NUM-1)，这是由于stl前闭后开的性质。
		begin_iterator = iterator(first_block + BLOCK_NUM / 2, block_size / 2);
		end_iterator = iterator(first_block + BLOCK_NUM / 2, block_size / 2);
	}
	template <class T,class Alloc>
	explicit deque<T, Alloc>::deque(size_type n, const_reference value,const allocator_type& a = allocator_type())
		:data_alloc(a){
		value_num=n;
		size_t block_num = n / (block_size-1) + 1;

		first_block = data_alloc_out.allocate(block_num);
		for (int i = 0; i < block_num; i++){
			*(first_block + i) = data_alloc.allocate(block_size);
		}
		last_block = first_block + BLOCK_NUM;

		begin_iterator = iterator(first_block + BLOCK_NUM / 2, block_size / 2);
		end_iterator = iterator(first_block + BLOCK_NUM / 2, block_size / 2);

		for (int i = 0; i < n / 2; i++){
			data_alloc.construct(--begin_iterator, value);
			data_alloc.construct(end_iterator, value);
			++end_iterator;
		}
		if (n % 2 == 1){
			data_alloc.construct(--begin_iterator, value);
		}
	}
	template <class T,class Alloc>
	explicit deque<T, Alloc>::deque(const deque& rhs){
		value_num = rhs.size();
		size_t block_num = rhs.first_block - rhs.last_block;

		first_block = data_alloc_out.allocate(block_num);
		last_block = first_block + block_num;

		for (int i = 0; i < block_num; i++){
			*(first_block + i) = data_alloc.construct(block_size); 
		}

		begin_iterator = iterator(first_block + (rhs.begin_iterator.get_block_address() - rhs.first_block)
			, rhs.begin_iterator.get_block_index());
		end_iterator = MINI_STL::uninitialized_copy(rhs.begin_iterator, rhs.end_iterator, begin_iterator);
	}
	template <class T,class Alloc>
	deque<T, Alloc>& deque<T, Alloc>::operator=(const deque& rhs){
		if (this == &rhs)return *this;//有缺陷，因为未对==重载
		size_t block_num = last_block - first_block;
		if (value_num >= rhs.value_num){
			iterator new_begin = begin_iterator + (value_num / 2);
			iterator new_end = end_iterator - (value_num / 2);
			if (value_num % 2 == 1){
				--new_end;
			}
			size_t offset = rhs.value_num / 2;

			


		}
	}





}



//#endif