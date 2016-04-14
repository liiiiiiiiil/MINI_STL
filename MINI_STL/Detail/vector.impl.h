#ifndef _VECTOR_DE_H_
#define _VECTOR_DE_H_


namespace MINI_STL{
	template<class T,class Alloc>
	vector<T,Alloc>::~vector(){
		destroyAndDeallocateAll();
	}
	template<class T, class Alloc> 
	vector<T, Alloc>::vector(const size_type& n){
		allocateAndFillN(n,value_type());
	}
	template<class T, class Alloc>
	vector<T, Alloc>::vector(const size_type n, const value_type& x){
		allocateAndFillN(n, x);
	}
	template<class T, class Alloc>
	template<class  InputIterator>
	vector<T, Alloc>::vector(InputIterator first, InputIterator last){
		vector_aux(first, last, typename std::is_integral<InputIterator>::type());
	}
	template<class T, class Alloc>
	vector<T, Alloc>::vector(const vector& v){
		allocateAndCopy(v.start, v.finish);
	}
	template<class T, class Alloc>
	vector<T, Alloc>::vector(vector&& v){
		start = v.start;
		finish = v.finish;
		end_of_storage = v.end_of_storage;
		v.start = v.finish = v.end_of_storage = 0;
	}
	template<class T,class Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator=(const vector& v){
		if (this != &v){
			allocateAndCopy(v.start,v.finish);
		}
		return *this;
	}
	template<class T, class Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator=(vector&& v){
		if (this != &v){
			destroyAndDeallocateAll();
			start = v.start;
			finish = v.finish;
			end_of_storage = v.end_of_storage;
			v.start = v.finish = v.end_of_storage = 0;
		}
		return *this;
	}
	////容器容量的相关操作
	template<class T, class Alloc>///resize 重新定义容器大小
	void vector<T, Alloc>::resize(size_type n,value_type x = value_type()){
		if (n < size()){//小于现有元素数量，析构多余的，并且调整finish指针
			data_allocator::destroy(start + n, finish);
			finish = start + n;
		}
		else if (n > size() && n <= capacity()){//多余现有元素数量，但是少于总的容器容量
			auto InsertSize= n - size();
			finish = TinySTL::uninitialized_fill_n(finish, InsertSize, x);
		}
		else if (n > capacity()){//多余总容量，先allocate出新的
			auto InsertSize = n - size();//需要增加的容量
			T *newStart = data_allocator::allocate(getNewCapacity(InsertSize));//通过getNewCapacity来增加
			T *newFinish = uninitialized_copy(begin(), end(), newStart);//将原来的元素copy进新的空间
			newFinish = uninitialized_fill_n(newFinish, InsertSize, x);//把x装进剩余空间

			destroyAndDeallocateAll();//将原来的容器析构了
			start = newStart;
			finish = newFinish;
			end_of_storage = start + n;
		}
	}
	template<class T, class Alloc>//reserve 保留容器中n个元素
	void vector<T, Alloc>::reserve(size_type n){
		if (n <= capacity())
			return;
		T *newStart = data_allocator::allocate(n);
		T *newFinish =uninitialized_copy(begin(), end(), newStart);
		destroyAndDeallocateAll();

		start = newStart;
		finish = newFinish;
		end_of_storage = start + n;
	}
	//一些具体的对容器的操作，比如删除等
	template<class T, class Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator position){
		return erase(position, position + 1);
	}
	template<class T, class Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator first, iterator last){
		//尾部残留对象数
		difference_type lenOfTail = end() - last;
		//删去的对象数目
		difference_type lenOfRemoved = last - first;//将last到finsh的对象搬到从first开始的空间
		finish = finish - lenOfRemoved;
		for (; lenOfTail != 0; --lenOfTail){
			auto temp = (last - lenOfRemoved);
			*temp = *(last++);
		}
		return (first);
	}
	template<class T, class Alloc>
	template<class InputIterator>
	void vector<T, Alloc>::reallocateAndCopy(iterator position, InputIterator first, InputIterator last){
		difference_type newCapacity = getNewCapacity(last - first);
		//以下是从新申请一个新空间，先把start到position的元素插入到新空间内，
		//再把first到last的元素紧跟position插入到新空间中，
		//最后再把原先position到end的元素紧跟last插入到新空间
		T *newStart = data_allocator::allocate(newCapacity);
		T *newEndOfStorage = newStart + newCapacity;
		T *newFinish = MINI_STL::uninitialized_copy(begin(), position, newStart);

		newFinish = MINI_STL::uninitialized_copy(first, last, newFinish);
		newFinish = MINI_STL::uninitialized_copy(position, end(), newFinish);

		destroyAndDeallocateAll();
		start = newStart;
		finish = newFinish;
		end_of_storage = newEndOfStorage;
	}
	template<class T, class Alloc>
	void vector<T, Alloc>::reallocateAndFillN(iterator position, const size_type& n, const value_type& val){
		difference_type newCapacity = getNewCapacity(n);
		//与rellocateAndCopy同理
		T *newStart = data_allocator::allocate(newCapacity);
		T *newEndOfStorage = newStart + newCapacity;
		T *newFinish = MINI_STL::uninitialized_copy(begin(), position, newStart);
		newFinish = MINI_STL::uninitialized_fill_n(newFinish, n, val);
		newFinish = MINI_STL::uninitialized_copy(position, end(), newFinish);

		destroyAndDeallocateAll();
		start = newStart;
		finish = newFinish;
		end_of_storage = newEndOfStorage;
	}
	template<class T, class Alloc>
	template<class InputIterator>
	void vector<T, Alloc>::insert_aux(iterator position,InputIterator first,InputIterator last,std::false_type){
		difference_type locationLeft = end_of_storage - finish; //备用空间
		difference_type locationNeed = distance(first, last);//新增的元素

		if (locationLeft >= locationNeed){//备用空间足够
			if (finish - position > locationNeed){//插入后现有的元素个数大于新增元素
				MINI_STL::uninitialized_copy(finish - locationNeed, finish, finish);
				MINI_STL::uninitialized_copy_backward(position, finish - locationNeed, finish);
				MINI_STL::uninitialized_copy(first, last, position);
			}
			else{//插入后现有的元素少于新增元素个数
				iterator temp = MINI_STL::uninitialized_copy(first + (finish - position), last, finish);
				MINI_STL::uninitialized_copy(position, finish, temp);
				MINI_STL::uninitialized_copy(first, first + (finish - position), position);
			}
			finish += locationNeed;
		}
		else{
			reallocateAndCopy(position, first, last);
		}
	}
	template<class T, class Alloc>
	template<class Integer>
	void vector<T, Alloc>::insert_aux(iterator position, Integer n, const value_type& value, std::true_type){
		if (n == 0)return;//后改
		difference_type locationLeft = end_of_storage - finish; // 备用空间
		difference_type locationNeed = n;//新增元素个数

		if (locationLeft >= locationNeed){//备用空间大于新增元素个数
			auto tempPtr = end() - 1;//finish-1
			for (; tempPtr - position >= 0; --tempPtr){//先把原来的元素移动过去
				construct(tempPtr + locationNeed, *tempPtr);
			}
			MINI_STL::uninitialized_fill_n(position, n, value);//在原来的位置填充新元素
			finish += locationNeed;
		}
		else{
			reallocateAndFillN(position, n, value);//申请新空间，填充新元素，并且把原来的元素移动过去
		}
	}
	template<class T, class Alloc>
	template<class InputIterator>
	void vector<T, Alloc>::insert(iterator position, InputIterator first, InputIterator last){//insert
		insert_aux(position, first, last, typename std::is_integral<InputIterator>::type());
	}
	template<class T, class Alloc>
	void vector<T, Alloc>::insert(iterator position, const size_type& n, const value_type& value){
		insert_aux(position, n, value, typename std::is_integral<size_type>::type());
	}
	template<class T, class Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator position, const value_type& value){//insert
		const auto index = position - begin();
		insert(position, 1, value);/////这他妈都可以,c++这么diao!!!!待会儿回来再来看/好像只是直接调用上面的函数，那就没什么diao的啊
		return begin() + index;
	}
	template<class T, class Alloc>
	void vector<T, Alloc>::push_back(const value_type& value){
		insert(end(), value);
	}

	////////////逻辑比较
	template<class T, class Alloc>
	bool vector<T, Alloc>::operator == (const vector& v)const{
		if (size() != v.size()){
			return false;
		}
		else{
			auto ptr1 = start;
			auto ptr2 = v.start;
			for (; ptr1 != finish && ptr2 != v.finish; ++ptr1, ++ptr2){
				if (*ptr1 != *ptr2)
					return false;
			}
			return true;
		}
	}
	template<class T, class Alloc>
	bool vector<T, Alloc>::operator != (const vector& v)const{
		return !(*this == v);
	}
	template<class T, class Alloc>
	bool operator == (const vector<T, Alloc>& v1, const vector<T, Alloc>& v2){
		return v1.operator==(v2);
	}
	template<class T, class Alloc>
	bool operator != (const vector<T, Alloc>& v1, const vector<T, Alloc>& v2){
		return !(v1 == v2);
	}
	template<class T, class Alloc>
	void vector<T, Alloc>::shrink_to_fit(){
		T* t = (T*)data_allocator::allocate(size());
		finish = MINI_STL::uninitialized_copy(start, finish, t);
		data_allocator::deallocate(start, capacity());
		start = t;
		end_of_storage = finish;
	}
	template<class T, class Alloc>
	void vector<T, Alloc>::clear(){
		data_allocator::destroy(start, finish);
		finish = start;
	}
	template<class T, class Alloc>
	void vector<T, Alloc>::swap(vector& v){
		if (this != &v){
			MINI_STL::swap(start, v.start);
			MINI_STL::swap(finish, v.finish);
			MINI_STL::swap(end_of_storage, v.end_of_storage);
		}
	}
	template<class T, class Alloc>
	void vector<T, Alloc>::pop_back(){
		--finish;
		data_allocator::destroy(finish);
	}
	template<class T, class Alloc>
	void vector<T, Alloc>::destroyAndDeallocateAll(){
		if (capacity() != 0){
			data_allocator::destroy(start, finish);
			data_allocator::deallocate(start, capacity());
		}
	}
	template<class T, class Alloc>
	void vector<T, Alloc>::allocateAndFillN(const size_type n, const value_type& value){
		start = data_allocator::allocate(n);
		MINI_STL::uninitialized_fill_n(start, n, value);
		finish = end_of_storage = start + n;
	}
	template<class T, class Alloc>
	template<class InputIterator>
	void vector<T, Alloc>::allocateAndCopy(InputIterator first, InputIterator last){
		start = data_allocator::allocate(last - first);
		finish = MINI_STL::uninitialized_copy(first, last, start);
		end_of_storage = finish;
	}
	template<class T, class Alloc>
	template<class InputIterator>
	void vector<T, Alloc>::vector_aux(InputIterator first, InputIterator last, std::false_type){
		allocateAndCopy(first, last);
	}
	template<class T, class Alloc>
	template<class Integer>
	void vector<T, Alloc>::vector_aux(Integer n, const value_type& value, std::true_type){
		allocateAndFillN(n, value);
	}
	template<class T, class Alloc>
	typename vector<T, Alloc>::size_type vector<T, Alloc>::getNewCapacity(size_type len)const{
		size_type oldCapacity = end_of_storage - start;
		auto res = MINI_STL::max(oldCapacity, len);
		size_type newCapacity = (oldCapacity != 0 ? (oldCapacity + res) : len);
		return newCapacity;
	}
}
#endif