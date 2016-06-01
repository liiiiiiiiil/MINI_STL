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
		/*д�����ͻȻ����һ������vector���ڵ�һ�����⣬
		�������vector�Ĵ�С����other������vector��������
		operator=���Ƿ���ܳ����ڴ�й©���ڴˣ��ǲ�����Ҫ
		��Ҫ����ԭ����*/
		/*����������Ҫ������ԭ��������*/
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
	/*д�����ͻȻ����һ������vector���ڵ�һ�����⣬
	�������vector�Ĵ�С����other������vector��������
	operator=���Ƿ���ܳ����ڴ�й©���ڴˣ��ǲ�����Ҫ
	��Ҫ����ԭ����*/
	/*����������Ҫ������ԭ��������*/
	deque<T, Alloc>& deque<T, Alloc>::operator=(const deque& other){
		if (*this != other){
			while (start != finish){//������ԭ�ȵ�Ԫ��
				destroy(start.cur);
				desrtoy(finish.cur);
				++start;
				--finish;
			}
			for (auto start_temp = other.start; start_temp != other.finish; ++start_temp){//������Ԫ�أ�ʹ��pushback
				this->push_back(*start_temp);
			}
		}
		return *this;
	}
	template <class T, class Alloc>
	deque<T, Alloc>::~deque(){
		clear();
		map_allocator::deallocate(map,map_size);//�ͷ�map�ռ�
		return;
	}
	/*�����ǹ��������Ⱥ���*/

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
		iterator next = position;
		++next;
		difference_type index = pos - start;
		if (index < size() >> 1){
			uninitialized_copy_backward(start, position, next);
			pop_front();
		}
		else{
			uninitialized_copy(next, finish, postion);
			pop_back();
		}
		return start + index; 
	}
	template <class T, class Alloc>
	deque<T, Alloc>::iterator deque<T, Alloc>::erase(iterator first,iterator last){
		if (first == start&&last == finish){
			clear();
			return start;
		}
		difference_type pre_index = first - start;
		difference_type next_index = finish - last;
		difference_type num = last - first;
		if (pre_index < next_index){
			iterator new_start = start + n;
			uninitialized_copy_backward(start, first, last);
			destroy(start, new_start);
			for (map_pointer cur = start.node; cur < new_start.node; ++cur){
				data_allocator::deallocate(*cur, DEQUE_BUFFER_SIZE);
			}
			start = new_start;
		}
		else{
			iterator new_finish = finish - num;
			uninitialized_copy(last, finish, first);
			destroy(new_finish, finish);
			for (map_pointer cur = new_finish.node; cur <= finish.node; ++cur){
				data_allocator::deallocate(*cur, DEQUE_BUFFER_SIZE);
			}
			finish = new_finish;
		}
		return start + pre_index;
	}
	template <class T, class Alloc>
	deque<T, Alloc>::iterator deque<T, Alloc>::insert(iterator position, const_reference value){

	}
	template <class T, class Alloc>
	size_t deque<T, Alloc>::size(){
		auto result=distance(start, finsh);//���ص���difference_type
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
			construct(finish.cur, value);//cur��ָ���λ��û��Ԫ�أ�stlǰ�պ󿪾�����Ȼ��constructʹ��ָ����õ�������
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
	//finish.curָ���λ��û�����ݣ�stl ǰ�պ󿪣�
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
	template <class T,class Alloc>
	deque<T,Alloc>::iterator deque<T, Alloc>::insert(iterator position, const_reference value){
		return insert(position, 1, value);
	}
	template<class T,class Alloc>
	deque<T, Alloc>::iterator deque<T, Alloc>::insert(iterator position, size_type n, const_reference value){
		return insert_aux(position, n, value, typename std::is_integral<size_type>::type());
	}
	template<class T,class Alloc>
	template<class Inputiterator>
	deque<T, Alloc>::iterator deque<T, Alloc>::insert(iterator position, Inputiterator first, Inputiterator last){
		return insert_aux(position, first, last, typename std::is_integral<iterator>::type());
	}
	/*������private����*////////////////////////////
	template<class T, class Alloc>
	template<class Integer>
	deque<T, Alloc>::iterator deque<T, Alloc>::insert_aux(iterator position, Integer n,
		const_reference value, std::true_type){
		difference_type index_before = position - start;
		if (index_before < DEQUE_BUFFER_SIZE/2){//�ƶ�ǰ���Ԫ��
			if (n < start.cur - start.first){//�ռ��㹻
				iterator new_start = start - n;
				uninitialized_copy(start, position, new_start);
				uninitialized_fill_n(position - n, n, value);
				start = new_start;
				return position - n;
			}
			else{//�ռ䲻��,�����¿ռ�
				size_t nodes_to_add =(n-index_before) / DEQUE_BUFFER_SIZE+1;
				reserve_map_at_front(nodes_to_add);
				for (int i = 1; i <= nodes_to_add; ++i){
					*(start.node - i) = data_allocator::allocate(DEQUE_BUFFER_SIZE);
				}
				auto new_start = start - n;
				uninitialized_copy(start, position, new_start);
				uninitialized_fill_n(position - n, n, value);
				start = new_start;
				return position - n;
			}
		}
		else{//�ƶ�����Ԫ��
			if (n < finish.last - finish.cur){//�ռ��㹻
				auto new_finish = finish + n;
				uninitialized_copy_backward(position, finish, new_finish);
				uninitialized_fill_n(position, n, value);
				finish = new_finish;
				return position + n;
			}
			else{//�ռ䲻��
				size_type index_after = finish.last - finish.cur;
				size_t nodes_to_add = (n - index_after) / DEQUE_BUFFER_SIZE + 1;
				reserve_map_at_back(nodes_to_add);
				for (int i = 1; i <= nodes_to_add; ++i){
					*(finish.node + i) = data_allocator::allocate(DEQUE_BUFFER_SIZE);
				}
				auto new_finish = finish + n;
				uninitialized_copy_backward(position, finish, new_finish);
				uninitialized_fill_n(position, n, value);
				finish = new_finish;
				return position + n;
			}
		}
	}
	template<class T, class Alloc>
	template<class Inputiterator>
	deque<T, Alloc>::iterator deque<T, Alloc>::insert_aux(iterator position, Inputiterator first,
		Inputiterator last, std::false_type){
		difference_type n = last - first;
		difference_type index_before = position - start;
		if (index_before < DEQUE_BUFFER_SIZE / 2){//�ƶ�ǰ���Ԫ��
			if (n < start.cur - start.first){//�ռ��㹻
				iterator new_start = start - n;
				uninitialized_copy(start, position, new_start);
				uninitialized_copy(first, last, position - n)
				start = new_start;
				return position - n;
			}
			else{//�ռ䲻��,�����¿ռ�
				size_t nodes_to_add = (n - index_before) / DEQUE_BUFFER_SIZE + 1;
				reserve_map_at_front(nodes_to_add);
				for (int i = 1; i <= nodes_to_add; ++i){
					*(start.node - i) = data_allocator::allocate(DEQUE_BUFFER_SIZE);
				}
				auto new_start = start - n;
				uninitialized_copy(start, position, new_start);
				uninitialized_copy(first, last, position - n)
				start = new_start;
				return position - n;
			}
		}
		else{//�ƶ�����Ԫ��
			if (n < finish.last - finish.cur){//�ռ��㹻
				auto new_finish = finish + n;
				uninitialized_copy_backward(position, finish, new_finish);
				uninitialized_copy(first, last, position);
				finish = new_finish;
				return position + n;
			}
			else{//�ռ䲻��
				size_type index_after = finish.last - finish.cur;
				size_t nodes_to_add = (n - index_after) / DEQUE_BUFFER_SIZE + 1;
				reserve_map_at_back(nodes_to_add);
				for (int i = 1; i <= nodes_to_add; ++i){
					*(finish.node + i) = data_allocator::allocate(DEQUE_BUFFER_SIZE);
				}
				auto new_finish = finish + n;
				uninitialized_copy_backward(position, finish, new_finish);
				uninitialized_copy(first, last, position);
				finish = new_finish;
				return position + n;
			}
		}
	}

	//���˻�����ͷ��c
	template <class T, class Alloc>
	void deque<T, Alloc>::pop_back_aux(){
		--finish;
		data_allocator::deallocate(*(finish.node + 1),DEQUE_BUFFER_SIZE);
		destroy(finish.cur);
	}
	//���˻�����β��
	template <class T, class Alloc>
	void deque<T, Alloc>::pop_front_aux(){
		destroy(start.cur);
		++start;
		data_allocator::deallocate(*(start.node - 1),DEQUE_BUFFER_SIZE);

	}
	//���˻�����β��
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
	//�Ѿ����˻�����ͷ�����õĺ���
	template <class T, class Alloc>
	void deque<T, Alloc>::push_front_aux(const_reference value){
		value_type value_temp = value;
		reserve_map_at_front();
		*(start.node - 1) = data_allocator::allocate(DEQUE_BUFFER_SIZE);
		start.set_node(start.node - 1);
		construct(start.cur, value_temp);
		return;
	}
	//����Ƿ���Ҫ����map�ڵ�
	template <class T, class Alloc>
	void deque<T, Alloc>::reserve_map_at_back(size_type nodes_to_add=1){
		if (nodes_to_add + 1 > map_size - (finish.node - map)){
			reallocate_map(nodes_to_add, false);
		}
	}
	//ͬ��
	template <class T, class Alloc>
	void deque<T, Alloc>::reserve_map_at_front(size_type nodes_to_add = 1){
		if (nodes_to_add > start.node - map){
			reallocate_map(nodes_to_add, true);
		}
	}
	/*����map�ڵ㣬���Ǻ���Ҫ����Ҫ����Ҫ��һ���ǣ��������Ȳ��Ի����������ڴ棬
	���Ǹ�������Ҫ��ʱ�����������������ڴ棬�ɴ˿��Խ�Լ�ڴ棡��������*/
	/************************************************************************/
	/*��Ҫ˼�룺����Ҫ��ʱ���ٷ����ڴ棡��Ҫ����Ԥ�ȷ�������ڴ�*/
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
	/*������friend ����*/
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