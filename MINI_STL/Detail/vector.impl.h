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
	////������������ز���
	template<class T, class Alloc>///resize ���¶���������С
	void vector<T, Alloc>::resize(size_type n,value_type x = value_type()){
		if (n < size()){//С������Ԫ����������������ģ����ҵ���finishָ��
			data_allocator::destroy(start + n, finish);
			finish = start + n;
		}
		else if (n > size() && n <= capacity()){//��������Ԫ�����������������ܵ���������
			auto InsertSize= n - size();
			finish = TinySTL::uninitialized_fill_n(finish, InsertSize, x);
		}
		else if (n > capacity()){//��������������allocate���µ�
			auto InsertSize = n - size();//��Ҫ���ӵ�����
			T *newStart = data_allocator::allocate(getNewCapacity(InsertSize));//ͨ��getNewCapacity������
			T *newFinish = uninitialized_copy(begin(), end(), newStart);//��ԭ����Ԫ��copy���µĿռ�
			newFinish = uninitialized_fill_n(newFinish, InsertSize, x);//��xװ��ʣ��ռ�

			destroyAndDeallocateAll();//��ԭ��������������
			start = newStart;
			finish = newFinish;
			end_of_storage = start + n;
		}
	}
	template<class T, class Alloc>//reserve ����������n��Ԫ��
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
	//һЩ����Ķ������Ĳ���������ɾ����
	template<class T, class Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator position){
		return erase(position, position + 1);
	}
	template<class T, class Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator first, iterator last){
		//β������������
		difference_type lenOfTail = end() - last;
		//ɾȥ�Ķ�����Ŀ
		difference_type lenOfRemoved = last - first;//��last��finsh�Ķ���ᵽ��first��ʼ�Ŀռ�
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
		//�����Ǵ�������һ���¿ռ䣬�Ȱ�start��position��Ԫ�ز��뵽�¿ռ��ڣ�
		//�ٰ�first��last��Ԫ�ؽ���position���뵽�¿ռ��У�
		//����ٰ�ԭ��position��end��Ԫ�ؽ���last���뵽�¿ռ�
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
		//��rellocateAndCopyͬ��
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
		difference_type locationLeft = end_of_storage - finish; //���ÿռ�
		difference_type locationNeed = distance(first, last);//������Ԫ��

		if (locationLeft >= locationNeed){//���ÿռ��㹻
			if (finish - position > locationNeed){//��������е�Ԫ�ظ�����������Ԫ��
				MINI_STL::uninitialized_copy(finish - locationNeed, finish, finish);
				MINI_STL::uninitialized_copy_backward(position, finish - locationNeed, finish);
				MINI_STL::uninitialized_copy(first, last, position);
			}
			else{//��������е�Ԫ����������Ԫ�ظ���
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
		if (n == 0)return;//���
		difference_type locationLeft = end_of_storage - finish; // ���ÿռ�
		difference_type locationNeed = n;//����Ԫ�ظ���

		if (locationLeft >= locationNeed){//���ÿռ��������Ԫ�ظ���
			auto tempPtr = end() - 1;//finish-1
			for (; tempPtr - position >= 0; --tempPtr){//�Ȱ�ԭ����Ԫ���ƶ���ȥ
				construct(tempPtr + locationNeed, *tempPtr);
			}
			MINI_STL::uninitialized_fill_n(position, n, value);//��ԭ����λ�������Ԫ��
			finish += locationNeed;
		}
		else{
			reallocateAndFillN(position, n, value);//�����¿ռ䣬�����Ԫ�أ����Ұ�ԭ����Ԫ���ƶ���ȥ
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
		insert(position, 1, value);/////�����趼����,c++��ôdiao!!!!���������������/����ֻ��ֱ�ӵ�������ĺ������Ǿ�ûʲôdiao�İ�
		return begin() + index;
	}
	template<class T, class Alloc>
	void vector<T, Alloc>::push_back(const value_type& value){
		insert(end(), value);
	}

	////////////�߼��Ƚ�
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