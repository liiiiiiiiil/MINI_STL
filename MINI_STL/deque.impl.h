#ifndef DEQUE_IMPL_H_
#define DEQUE_IMPL_H_

namespace MINI_STL{
	namespace DEQUE_DETAIL{
		template <class T>
		deque_it<T>::deque_it(){
			current_block = static_cast<pointer*>(0);
			current_block_index = 0;
		}
		template <class T>
		deque_it<T>::deque_it(pointer* block,size_t block_index){
			current_block = block;
			current_block_index = block_index;
		}

		template <class T>
		deque_it<T>& deque_it<T>::operator++(){
			if (++current_block_index == block_size){
				current_block_index = 0;
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
			if (current_block_index-- = 0){
				current_block_index = block_size - 1;
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
			current_block += n / block_size;
			int index_inc = n%block_size;
			if ((current_block_index + index_inc) >= block_size){
				current_block_size = (current_block_index + index) % block_size;
				++current_block;
			}
			else{
				current_block_index += index_inc;
			}
			return *this;
		}
		template <class T>
		deque_it<T>& deque_it<T>::operator-=(typename deque_it<T>::difference_type n){
			current_block -= n / block_size;
			int index_dec = d%block_size;
			if (current_block_index - index_dec < 0){
				current_block_index = (block_size-1)/*Ç°±Õºó¿ª*/ - (index_dec - current_block_index);
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
		//end of iterator






	}



}



#endif