#ifndef LIST_IMPL_H_
#define LIST_IMPL_H_
namespace MINI_STL{
	namespace LIST_DETAIL{
		/*迭代器成员函数*/
		template<class T>
		list_iterator<T>& list_iterator<T>::operator++(){
			node = node->next;
			return *this;
		}
		template<class T>
		list_iterator<T> list_iterator<T>::operator++(int){
			auto temp = *this;
			++*this;
			return temp;
		}
		template <class T>
		list_iterator<T>& list_iterator<T>::operator--(){
			node = node->prev;
			return *this;
		}
		template<class T>
		list_iterator<T>  list_iterator<T>::operator--(int){
			auto temp = *this;
			--*this;
			return temp;
		}
		template <class T>
		bool operator==(const list_iterator<T>& lhs, const list_iterator<T>&rhs){
			return lhs.node == rhs.node;
		}
		template <class T>
		bool operator!=(const list_iterator<T>& lhs, const list_iterator<T>&rhs){
			return !(lhs == rhs);
		}
	}
	/*list构造节点和销毁节点函数*/
	template <class T>
	typename list<T>::nodePtr list<T>::newNode(const T& value = T()){
		auto res = list_allocator::allocate();
		list_allocator::construct(res, LIST_DETAIL::list_node<T>(value, nullptr, nullptr, this));
		return res;
	}
	template <class T>
	void list<T>::deleteNode(nodePtr p){
		p->prev = p->next = nullptr;
		list_allocator::destroy(p);
		list_allocator::deallocate(p);
	}
	/*以下是构造和析构函数等*/
	template<class T>
	void list<T>::constructor_aux(size_type n, const value_type& val, std::true_type){
		head.node = newNode();//add a dummy node
		tail.node = head.node;
		while (n--)
			push_back(val);
	}
	template <class T>
	template <class InputIterator>
	void list<T>::constructor_aux(InputIterator first, InputIterator last, std::false_type){
		head.node = newNode();
		tail.node = head.node;
		for (; first != last; ++first){
			push_back(*first);
		}
	}
	template <class T>
	list<T>::list(){
		head.node = newNode();
		tail.node = head.node;
	}
	template <class T>
	list<T>::list(size_type n, const value_type& value = value_type()){
		constructor_aux(n, value, std::true_type());
	}
	template <class T>
	template <class InputIterator>
	list<T>::list(InputIterator first, InputIterator last){
		constructor_aux(first, last, std::is_integral<InputIterator>());
	}
	template <class T>
	list<T>::list(const list& rhs){
		head.node = newNode();
		tail.node = head.node;
		for (auto temp = rhs.head.node; temp != rhs.tail.node; temp = temp->next){
			push_back(temp->data);
		}
	}
	template <class T>
	list<T>& list<T>::operator=(const list<T>& rhs){
		if (this != &rhs){
			list(rhs).swap(*this);
		}
		return this;
	}
	template <class T>
	list<T>::~list(){
		for (auto temp = head; temp != tail; ++temp){
			list_allocator::destroy(temp.node);
			list_allocator::deallocate(temp.node);
		}
		list_allocator::deallocate(tail.node);
	}
	/*一些基本的成员函数*/
	template <class T>
	typename list<T>::size_type list<T>::size()const{
		size_type len = 0;
		for (auto temp = head; temp != tail; ++temp){
			++len;
		}
		return len;
	}
	template <class T>
	void list<T>::push_back(const value_type& value){
		auto node = newNode();
		(tail.node)->data = value;
		(tail.node)->next = node;
		node->prev = tail.node;
		tail.node = node;
	}
	template <class T>
	void list<T>::pop_back(){
		auto newTail = tail.node->prev;
		tail.node->next = nullptr;
		deleteNode(tail.node);
		tail.node = newTail;
	}
	template<class T>
	void list<T>::push_front(const value_type& value){
		auto newHead = newNode(value);
		newHead->next = head.node;
		head.node->prev = newHead;
		head.node = newHead;
	}
	template <class T>
	void list<T>::pop_front(){
		auto temp = head.node;
		head.node = temp->next;
		head.node->prev = nullptr;
		deleteNode(temp);
	}
	/*插入操作函数*/
	template <class T>
	typename list<T>::iterator list<T>::insert(iterator position, const value_type& value){
		if (position == begin()){
			push_front(value);
			return begin();
		}
		else if (position == end()){
			push_back(value);
			return  position;
		}
		else{
			auto new_node = newNode(value);
			position.node->prev->next = new_node;
			new_node->prev = position.node->prev;
			new_node->next = position.node;
			position.node->prev = new_node;
			return iterator(new_node);
		}
	}
	template <class T>
	void list<T>::insert_aux(iterator position, size_type n, const value_type& value, std::true_type){
		for (auto i = n; i > 0; --i){
			position = insert(position, value);
		}
	}
	template <class T>
	template <class InputIterator>
	void list<T>::insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type){//break point
		if (first == last)return;
		for (--last; last != first; --last){
			position = insert(position, *last);
		}
		insert(position, *last);
	}
	template<class T>
	void list<T>::insert(iterator position, size_type n, const value_type& value){
		insert_aux(position, n, value, typename std::is_integral<size_type>::type());
	}
	template<class T>
	template <class InputIterator>
	void list<T>::insert(iterator position, InputIterator first, InputIterator last){
		insert_aux(position, first, last, typename std::is_integral<InputIterator>::type());
	}
	/*删除操作*/
	template <class T>
	typename list<T>::iterator list<T>::erase(iterator position){
		if (position == head){
			pop_front();
			return head;
		}
		else{
			auto pre = position.node->prev;
			pre->next = position.node->next;
			(position.node->next)->prev = pre;
			deleteNode(position.node);
			return iterator(pre->next);
		}
	}
	template <class T>
	typename list<T>::iterator list<T>::erase(iterator first, iterator last){
		typename list<T>::iterator pos;
		for (; first != last;){
			auto temp = first++;
			pos = erase(temp);
		}
		return pos;
	}
	template <class T>
	void list<T>::clear(){
		erase(begin(), end());
	}
	/*小操作*/
	template <class T>
	typename list<T>::iterator list<T>::begin(){
		return head;
	}
	template <class T>
	typename list<T>::iterator list<T>::end(){
		return tail;
	}
	template<class T>
	typename list<T>::const_iterator list<T>::getConstIterator(iterator& it)const{
		using nodeP = LIST_DETAIL::list_node<const T>*;
		auto temp = (list<const T>*const)this;
		auto ptr = it.p;
		LIST_DETAIL::list_node<const T> list_node(ptr->data, (nodeP)(ptr->prev), (nodeP)(ptr->next), temp);
		return const_iterator(&list_node);
	}
	template<class T>
	typename list<T>::const_iterator list<T>::cbegin()const{
		auto temp = (list*const)this;
		return getConstIterator(temp->head);
	}
	template<class T>
	typename list<T>::const_iterator list<T>::cend()const{
		auto temp = (list*const)this;
		return getConstIterator(temp->tail);
	}
	template<class T>
	void list<T>::reverse(){
		if (empty() || head.node->next == tail.node)return;
		auto curPtr = head.node;
		nodePtr prePtr;
		while (curPtr != tail.node){
			prePtr = curPtr->prev;
			curPtr->prev = curPtr->next;
			curPtr->next = prePtr;
			curPtr = curPtr->prev;
		}
		/*调整head和tail指针*/
		auto temp1 = head.node;
		auto temp2 = prePtr->prev;

		head.node = temp2;
		head.node->prev = nullptr;
		temp1->next = tail.node;
		tail.node->prev = temp1;
		tail.node->next = nullptr;
	}
	template <class T>
	void list<T>::remove(const value_type& value){
		for (auto it = begin(); it != end();){
			if (*it == value){
				it = erase(it);
			}
			else
				++it;
		}
	}
	template <class  T>
	template<class Predicate>
	void list<T>::remove_if(Predicate pred){
		for (auto it = begin(); it != end();){
			if (pred(*it))
				it = erase(it);
			else
				++it;
		}
	}
	template <class T>
	void list<T>::swap(list& rhs){
		MINI_STL::swap(head.node, rhs.head.node);
		MINI_STL::swap(tail.node, rhs.tail.node);
	}
	template <class T>
	void swap(list<T>& lhs, list<T>&rhs){
		lhs.swap(rhs);
	}
	template <class T>//待测试
	void list<T>::unique(){
		if (this->size() <= 1)return;
		nodePtr curPtr = head.node;
		nodePtr nextPtr = curPtr->next;
		while (nextPtr != tail.node && nextPtr != nullptr){
			nextPtr = curPtr->next;
			if (curPtr->data == nextPtr->data){
				curPtr->next = nextPtr->next;
				nextPtr->next->prev = curPtr;
				deleteNode(nextPtr);
			}
			else{
				curPtr = nextPtr;
			}
		}
	}
	template <class T>
	template <class Predicate>
	void list<T>::unique(Predicate pred){
		nodePtr curPtr = head.node;
		nodePtr nextPtr = curPtr->next;
		while (nextPtr != tail.node || nextPtr != nullptr){
			nextPtr = curPtr->next;
			if (pred(curPtr->data, nextPtr->data)){
				curPtr->next = nextPtr->next;
				nextPtr->next->prev = curPtr;
				deleteNode(nextPtr);
			}
			else{
				curPtr = nextPtr->next;
			}
		}
	}
	template <class T>
	void list<T>::splice(iterator position, list& rhs){
		insert(position, rhs.begin(), rhs.end());
		rhs.head.node = rhs.tail.node;
	}
	template <class T>//last指向节点本身不插入到另一个链表中(前开后闭),插入到position前面
	void list<T>::splice(iterator position, list& rhs, iterator first, iterator last){
		if (first == last)return;
		auto lastPrev = last.node->prev;
		auto firstPrev = first.node->prev;

		if (first == rhs.begin()){
			last.node->prev = nullptr;
			rhs.head.node = last.node;
		}
		else{
			firstPrev->next = last.node;
			last.node->prev = firstPrev;
		}

		if (position == begin()){
			first.node->prev = nullptr;
			lastPrev->next = position.node;
			position.node->prev = lastPrev;
			head.node = first.node;
		}
		else{
			auto positionPre = position.node->prev;
			positionPre->next = first.node;
			first.node->prev = positionPre;
			lastPrev->next = position.node;
			position.node->prev = lastPrev;
		}
	}
	template <class T>
	void list<T>::splice(iterator position, list& rhs, iterator rPos){
		auto next = rPos;
		if (next.node == nullptr)return;
		splice(position, rhs, rPos, ++next);
	}
	template <class T>
	void list<T>::merge(list& rhs){//两个list都必须先递增，才能保证接合的list有序
		auto it1 = begin();
		auto it2 = rhs.begin();
		while (it1 != end() && it2 != rhs.end()){
			if (*it1 <= *it2){
				++it1;
			}
			else{
				auto temp = it2++;
				this->splice(it1, rhs, temp);
			}
		}
		if (it1 == end()){
			this->splice(it1, rhs, it2, rhs.end());
		}
	}
	template<class T>
	template <class Compare>
	void list<T>::merge(list& rhs, Compare comp){
		auto it1 = begin();
		auto it2 = rhs.begin();
		while (it1 != end() && it2 != rhs.end()){
			if (comp(*it2, *it1)){
				auto temp = it2;
				splice(it1, rhs, temp);
				++it2;
			}
			else{
				++it1;
			}
			if (it1 == end()){
				splice(it1, rhs, it2, rhs.end());
			}
		}
	}
	template <class T>
	bool operator==(const list<T>&lhs, const list<T>&rhs){
		if (lhs.size() != rhs.size())return false;
		auto nodePtr1 = lhs.begin().node;
		auto nodePtr2 = rhs.begin().node;
		while (nodePtr1 != end().node){
			if (nodePtr1->data != nodePtr2->data)
				return false;
		}
		return true;
	}
	template <class T>
	bool operator!=(const list<T>&lhs, const list<T>&rhs){
		return !(lhs == rhs);
	}
	template<class T>
	void list<T>::sort(){
		sort(MINI_STL::less<T>());
	}
	/*template <class T>
	template <class Compare>
	void list<T>::sort(Compare comp){
		if (empty() || head.node->next == tail.node)
			return;
		list carry;
		list counter[64];
		int fill = 0;
		while (!empty()){
			carry.splice(carry.begin(), *this, begin());
			int i = 0;
			while (i < fill && !counter[i].empty()){
				counter[i].merge(carry, comp);
				carry.swap(counter[i++]);
			}
			carry.swap(counter[i]);
			if (i == fill)
				++fill;
		}
		for (int i = 0; i != fill; ++i){
			counter[i].merge(counter[i - 1], comp);
		}
		swap(counter[fill - 1]);
	}*/
	template<class T>
	template <class Compare>
	void list<T>::sort(Compare comp){
		if (empty() || head.node->next == tail.node)
			return;

		list carry;
		list counter[64];
		int fill = 0;
		while (!empty()){
			carry.splice(carry.begin(), *this, begin());
			int i = 0;
			while (i < fill && !counter[i].empty()){
				counter[i].merge(carry, comp);
				carry.swap(counter[i++]);
			}
			carry.swap(counter[i]);
			if (i == fill)
				++fill;
		}
		for (int i = 0; i != fill; ++i){
			counter[i].merge(counter[i - 1], comp);
		}
		swap(counter[fill - 1]);
	}
}

#endif