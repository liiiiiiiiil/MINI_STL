//#ifndef DEQUE_IMPL_H_
//#define DEQUE_IMPL_H_
namespace MINI_STL{
	namespace DEQUE_IT{
		template <class T>
		void deque_it<T>::set_node(map_pointer new_node){
			node = new_node;
			first = *node;
			last = first + difference_type(buffer_size());
		}
		template <class T>
		deque_it<T>& deque_it<T>::operator++(){
			if (++cur == last){
				set_node(node++);
				cur = first;
			}
			return *this;
		}
		template <class T>
		deque_it<T> deque_it<T>::operator++(int){
			auto temp = *this;
			++(*this);
			return temp;
		}
		template <class T>
		deque_it<T>& deque_it<T>::operator--(){
			if (cur == first){
				set_node(node--);
				cur = last;
			}
			--cur;
			return *this;
		}
		template <class T>
		deque_it<T> deque_it<T>::operator--(int){
			auto temp = (*this);
			--(*this);
			return temp;
		}
		template <class T>
		deque_it<T>& deque_it<T>::operator+=(difference_type n){
			difference_type offset = n + cur - first;
			if (offset >= 0 && offset < difference_type(buffer_size())){
				cur += n;
			}
			else{
				difference_type node_offset = offset > 0 ? offset / difference_type(buffer_size())
					: -difference_type((-offset - 1) / buffer_size()) - 1;
				set_node(node + node_offset);
				cur = node_offset > 0 ? first + offset%difference_type(buffer_size())
					: last + offset%difference_type(buffer_size());
			}
			return *this;
		}
		template <class T>
		deque_it<T>& deque_it<T>::operator-=(difference_type n){
			return (*this) += (-n);
		}
		template <class T>
		deque_it<T> deque_it<T>::operator+(difference_type n){
			auto temp = *this;
			temp += n;
			return temp;
		}
		template <class T>
		deque_it<T> deque_it<T>::operator-(difference_type n){
			auto temp = *this;
			temp -= n;
			return temp;
		}

	}
	//end of deque_it
	
	/*
	start of deque
	*/
	template <class T,class Alloc>
	deque<T, Alloc>::deque(){
		this->deque(0,value_type());
	}
	template <class T, class Alloc>
	deque<T, Alloc>::deque(size_type n, const_reference value)
	:start(),finish(),map(0),map_size(0)
	{
		fill_initialize(n, value);
	}
	template <class T, class Alloc>
	deque<T, Alloc>::deque(const deque& other){
		size_t value_num = other.size();
		create_map_and_nodes(value_num);
		auto copy_pos = start.first + (map_size*BUFFER_SIZE / 2);
		uninitialized_copy(first, last, copy_pos);
		return;
	}
	template <class T,class Alloc>
	template<class InputIterator>
	deque<T, Alloc>::deque(InputIterator first, InputIterator last){
		deque_aux(first, last, typename std::is_integral<InputIterator>::type())
	}
	template <class T, class Alloc>
	size_t deque<T, Alloc>::size(){
		auto result=distance(start, finsh);//返回的是difference_type
		return  (size_t)result;
	}


	/*下面是private函数*/
	template <class T, class Alloc>
	template<class InputIterator>
	void deque<T, Alloc>::deque_aux(InputIterator first, InputIterator last, std::false_type){
		copy_initialize(first, last);
	}
	template <class T, class Alloc>
	void deque<T, Alloc>::deque_aux(size_type n, const_reference value, std::true_type){
		fill_initialize(n, value);
	}
	template <class T,class Alloc>
	void deque<T, Alloc>::fill_initialize(size_type n, const_reference value){
		create_map_and_nodes(n);
		map_pointer  cur;
		for (cur = start.node; cur < finish.node; ++cur){
			uninitialized_fill(*cur, *cur + BUFFER_SIZE, value);
		}
		uninitialized_fill(finish.first, finish.cur, value);
		return;
	}
	template <class T,class Alloc>
	template <class InputIterator>
	void deque<T, Alloc>::copy_initialize(InputIterator first,InputIterator last){
		size_type value_num = last - first+1;
		create_map_and_nodes(value_num);
		auto copy_pos = start.first + (map_size*BUFFER_SIZE / 2);
		uninitialized_copy(first, last, copy_pos);
		return;
	}
	template <class T, class Alloc>
	void deque<T, Alloc>::create_map_and_nodes(size_type n=0){
		size_type num_nodes = n / BUFFER_SIZE + 1;
		map_size = max(num_nodes+2, FIRST_MAP_SIZE);
		map = map_allocator::allocate(map_size);
		for (int i = 0; i < map_size; ++i){
			*(map + i) = data_allocator::allocate(BUFFER_SIZE);
		}
		map_pointer  nstart = map + (map_size-num_nodes)/ 2;
		map_pointer  nfinish = map + num_nodes - 1;
		start.set_node(nstart);
		finish.set_node(nfinish);
		start.cur = nstart.first;
		start.cur = nfinish.first + n%BUFFER_SIZE;
		return;
	}



}


//#endif