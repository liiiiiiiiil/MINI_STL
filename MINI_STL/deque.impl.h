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
		iterator new_start, new_finish;
		size_t value_num = other.size(); 
		create_map_and_nodes(value_num);
		new_start = start + (capacity() / 2);
		new_finish=uninitialized_copy(first, last, new_start);
		template <class T, class Alloc>
		/*写到这里，突然发现一个当初vector存在的一个问题，
		如果当初vector的大小大于other，按照vector那样调用
		operator=，是否可能出现内存泄漏？在此，是不是需要
		需要析构原来的*/
		/*最后决定还是要先析构原来的内容*/
		deque<T, Alloc>& deque<T, Alloc>::operator=(const deque& other){
			if (*this != other){

			}
			return *this;
		}
		return;
	}
	template <class T,class Alloc>
	template<class InputIterator>
	deque<T, Alloc>::deque(InputIterator first, InputIterator last){
		deque_aux(first, last, typename std::is_integral<InputIterator>::type())
	}
	template <class T, class Alloc>
	/*写到这里，突然发现一个当初vector存在的一个问题，
	如果当初vector的大小大于other，按照vector那样调用
	operator=，是否可能出现内存泄漏？在此，是不是需要
	需要析构原来的*/
	/*最后决定还是要先析构原来的内容*/
	deque<T, Alloc>& deque<T, Alloc>::operator=(const deque& other){
		if (*this != other){
			while (start != finish){//析构掉原先的元素
				destroy(start.cur);
				desrtoy(finish.cur);
				++start;
				--finish;
			}
			for (auto start_temp = other.start; start_temp != other.finish; ++start_temp){//加入新元素，使用pushback
				this->push_back(*start_temp);
			}
		}
		return *this;
	}
	template <class T, class Alloc>
	deque<T, Alloc>::~deque(){
		clear();
		map_allocator::deallocate(map,map_size);//释放map空间
		return;
	}
	/*以上是构造析构等函数*/

	template <class T, class Alloc>
	void deque<T, Alloc>::clear(){
		for (map_pointer node = start.node + 1; node < finish.node; ++node){
			destroy(*node, *node + DEQUE_BUFFER_SIZE);
			data_allocator::deallocate(*node, DEQUE_BUFFER_SIZE);
		}
		if (start.node != finish.node){
			destroy(start.cur, start.last);
			destroy(finish.cur, finish.last);
			data_allocator::deallocate(finish.first, DEQUE_BUFFER_SIZE);
		}
		else{
			destroy(start.cur, start.last);
		}
		finish = start;
	}
	template <class T, class Alloc>
	deque<T, Alloc>::iterator deque<T, Alloc>::erase(iterator position){
		
	}
	template <class T, class Alloc>
	deque<T, Alloc>::iterator deque<T, Alloc>::erase(iterator first,iterator last){

	}
	template <class T, class Alloc>
	deque<T, Alloc>::iterator deque<T, Alloc>::insert(iterator position, const_reference value){

	}
	template <class T, class Alloc>
	size_t deque<T, Alloc>::size(){
		auto result=distance(start, finsh);//返回的是difference_type
		return  (size_t)result;
	}
	template <class T, class Alloc>
	size_t deque<T, Alloc>::capacity(){
		return map_size*DEQUE_BUFFER_SIZE;
	}
	
	
	template <class T, class Alloc>
	bool deque<T, Alloc>::operator==(const deque& other){
		if (this->size() != other.size())return false;
		if (this->size() == 0 && other.size() == 0)return true;
		auto first1 = this->start;
		auto first2 = other.start;
		auto last1 = this->finish;
		auto last2 = other.finish;
		while (first1 != last!&&first2 != last2){
			if (*first1 != *first2){
				return false;
			}
			++first1;
			++first2;
		}
		return true;
	}
	template <class T, class Alloc>
	bool deque<T, Alloc>::operator!=(const deque& other){
		return !(*this == other);
	}
	template <class T, class Alloc>
	void deque<T, Alloc>::push_back(const_reference value){
		if (finish.cur != finish.last - 1){
			construct(finish.cur, value);//cur所指向的位置没有元素（stl前闭后开决定，然后construct使用指针别用迭代器）
			++finish;
		}
		else{
			push_back_aux(value);
		}
		return;
	}
	template <class T, class Alloc>
	void deque<T, Alloc>::push_front(const_reference value){
		if (start.cur != start.first){
			construct(start.cur-1, value);
			--start;
		}
		else{
			push_front_aux(value);
		}
		return;
	}
	//finish.cur指向的位置没有数据（stl 前闭后开）
	template <class T, class Alloc>
	void deque<T, Alloc>::pop_back(){
		if (finish.cur != finish.first){
			--finish;
			destroy(finish.cur);
		}
		else{
			pop_back_aux();
		}
	}
	template <class T, class Alloc>
	void deque<T, Alloc>::pop_front(){
		if (start.cur != start.last - 1){
			destroy(start.cur);
			++start;
		}
		else{
			pop_front_aux();
		}
	}
	/*下面是private函数*////////////////////////////

	//到了缓冲区头部c
	template <class T, class Alloc>
	void deque<T, Alloc>::pop_back_aux(){
		--finish;
		data_allocator::deallocate(*(finish.node + 1),DEQUE_BUFFER_SIZE);
		destroy(finish.cur);
	}
	//到了缓冲区尾部
	template <class T, class Alloc>
	void deque<T, Alloc>::pop_front_aux(){
		destroy(start.cur);
		++start;
		data_allocator::deallocate(*(start.node - 1),DEQUE_BUFFER_SIZE);

	}
	//到了缓冲区尾部
	template <class T, class Alloc>
	void deque<T, Alloc>::push_back_aux(const_reference value){
		value_type value_temp = value;
		reserve_map_at_back();
		*(finish.node + 1) = data_allocator::allocate(DEQUE_BUFFER_SIZE);
		construc(finish.cur, value_temp);
		finish.set_node(finish.node + 1);
		finish.cur = finish.first;
		return;
	}
	//已经到了缓冲区头而调用的函数
	template <class T, class Alloc>
	void deque<T, Alloc>::push_front_aux(const_reference value){
		value_type value_temp = value;
		reserve_map_at_front();
		*(start.node - 1) = data_allocator::allocate(DEQUE_BUFFER_SIZE);
		start.set_node(start.node - 1);
		construct(start.cur, value_temp);
		return;
	}
	//检查是否需要申请map节点
	template <class T, class Alloc>
	void deque<T, Alloc>::reserve_map_at_back(size_type nodes_to_add=1){
		if (nodes_to_add + 1 > map_size - (finish.node - map)){
			reallocate_map(nodes_to_add, false);
		}
	}
	//同上
	template <class T, class Alloc>
	void deque<T, Alloc>::reserve_map_at_front(size_type nodes_to_add = 1){
		if (nodes_to_add > start.node - map){
			reallocate_map(nodes_to_add, true);
		}
	}
	/*扩充map节点，但是很重要很重要很重要的一点是，在这里先不对缓冲区申请内存，
	而是根据所需要的时候来给缓存区分配内存，由此可以节约内存！！！！！*/
	/************************************************************************/
	/*重要思想：在需要的时候再分配内存！不要总是预先分配大量内存*/
	template <class T, class Alloc>
	void deque<T, Alloc>::reallocate_map(size_type node_to_add, bool add_at_front){
		size_type old_num_nodes = finish.node - start.node + 1;
		size_type new_num_nodes = node_to_add + old_num_nodes;
		map_pointer new_nstart;
		if (map_size > 2 * new_num_nodes){
			new_nstart = map + (map_size - new_num_nodes) / 2 + (add_at_front ? node_to_add : 0);
			if (new_nstart < start.node){
				uninitialized_copy(start.node, finish.node, new_nstart);
			}
			else
				uninitialized_copy_backward(start.node, finish.node, new_nstart + old_num_nodes);
		}
		else{
			size_type new_map_size = map_size + max(map_size, node_to_add) + 2;
			map_pointer new_map = map_allocator::allocate(new_map_size);
			new_nstart = new_map + (new_map_size - new_num_nodes) / 2 + (add_at_front ? node_to_add : 0);
			copy(start.node, finish.node, new_nstart);
			map_allocator::deallocate(map, map_size);
			map = new_map;
			map_size = new_map_size;
		}
		start.set_node(new_nstart);
		finish.set_node(new_nstart + old_num_nodes - 1);
	}
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
			uninitialized_fill(*cur, *cur + DEQUE_BUFFER_SIZE, value);
		}
		uninitialized_fill(finish.first, finish.cur, value);
		return;
	}
	template <class T,class Alloc>
	template <class InputIterator>
	void deque<T, Alloc>::copy_initialize(InputIterator first,InputIterator last){
		size_type value_num = last - first+1;
		create_map_and_nodes(value_num);
		auto copy_pos = start.first + (map_size*DEQUE_BUFFER_SIZE / 2);
		uninitialized_copy(first, last, copy_pos);
		return;
	}
	template <class T, class Alloc>
	void deque<T, Alloc>::create_map_and_nodes(size_type n=0){
		size_type num_nodes = n / DEQUE_BUFFER_SIZE + 1;
		map_size = max(num_nodes+2, DEQUE_FIRST_MAP_SIZE);
		map = map_allocator::allocate(map_size);
		for (int i = 0; i < map_size; ++i){
			*(map + i) = data_allocator::allocate(DEQUE_BUFFER_SIZE);
		}
		map_pointer  nstart = map + (map_size-num_nodes)/ 2;
		map_pointer  nfinish = map + num_nodes - 1;
		start.set_node(nstart);
		finish.set_node(nfinish);
		start.cur = nstart.first;
		start.cur = nfinish.first + n%DEQUE_BUFFER_SIZE;
		return;
	}
	/*下面是friend 函数*/
	template <class T, class Alloc>
	friend bool operator==(const deque<T, Alloc>&lhs, const deque<T, Alloc>&rhs){
		return lhs.operator==(rhs);
	}
	template <class T, class Alloc>
	friend bool operator!=(const deque<T, Alloc>&lhs, const deque<T, Alloc>&rhs){
		return !(lhs.operator==(rhs));
	}


}


//#endif