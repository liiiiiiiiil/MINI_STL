#include "../stl_alloc.h"
#include <new>
#include <iostream>
///�����ǵ�һ��������
namespace MINI_STL{
	void(*Malloc_alloc::malloc_alloc_oom_handler)() = 0;
	void* Malloc_alloc::oom_malloc(size_t size){
		void(*my_malloc_handler)();
		void* result;
		for (;;){
			my_malloc_handler = malloc_alloc_oom_handler;//�����Լ�����malloc_alloc_oom_handler��
			//������ʧ��ʱ��Ϳ��Ե���malloc_alloc_oom_handler������new handler����
			if (my_malloc_handler == 0){
				std::cout << "out of memory" << std::endl;
				exit(1);
			}
			(*my_malloc_handler)();
			result = malloc(size);
			if (result)return result;
		}
	}
	void* Malloc_alloc::oom_realloc(void* p, size_t new_size){
		void(*my_malloc_handler)();
		void * result;
		for (;;){
			my_malloc_handler = malloc_alloc_oom_handler;//ͬ��
			if (0 == my_malloc_handler){
				std::cout << "out of memory" << std::endl;
				exit(1);
			}
			(*my_malloc_handler)();
			result = realloc(p, new_size);
			if (result)return(result);
		}
	}
	//�ڶ���������,
	char* Default_alloc::start_free = 0;//��̬��Ա������ֵ
	char* Default_alloc::end_free = 0;
	size_t Default_alloc::heap_size = 0;
	Default_alloc::obj* Default_alloc::free_list[NFREELISTS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	void* Default_alloc::allocate(size_t size){
		if (size > 128){//bytes����128�����Ե���malloc_alloc
			return Malloc_alloc::allocate(size);
		}
		obj** my_free_list;
		obj* result;
		my_free_list = free_list + FREELISTINDEX(size);
		result = *my_free_list;
		if (0 == result){//�ô���������ռ䲻��
			return refill(ROUND_UP(size));
		}
		*my_free_list = result->next_obj;
		return result;
	}
	void Default_alloc::deallocate(void* p, size_t size){
		if (size > 128){
			return Malloc_alloc::deallocate(p, size);
		}
		obj** my_free_list;
		obj*q;
		q = (obj*)p;
		my_free_list = free_list + FREELISTINDEX(size);
		q->next_obj = *my_free_list;
		(*my_free_list) = q;
		return;
	}
	void * Default_alloc::refill(size_t size){//Ĭ��size��ALIGN�ı���
		int objs = OBJS;
		char* chunks;
		obj** my_free_list;
		obj* f_cur_obj, *f_next_obj;
		void *result;
		chunks = chunk_alloc(size, objs);//���ڴ�ػ�ȡ�ռ�Ĳ��������ڴ�ؿռ䲻��ʱ
		//ͨ��malloc���ڴ�����ӿռ�,objs�����ô���
		if (1 == objs){//�պ�һ��ռ�
			return chunks;
		}
		//��ȡ�˶������
		my_free_list = free_list + FREELISTINDEX(size);
		result = chunks;
		*my_free_list = f_cur_obj = (obj*)(chunks + size);
		//���½���ȡ�Ķ��������뵽free_list�У����������ǽڵ�����Ӳ���
		for (int i = 1; i < objs - 1; i++){
			f_next_obj = (obj*)((char*)f_cur_obj + size);
			f_cur_obj->next_obj = f_next_obj;
			f_cur_obj = f_next_obj;
		}
		f_cur_obj->next_obj = 0;
		return result;
	}
	char* Default_alloc::chunk_alloc(size_t size, int& objs){
		char* result;
		size_t size_needed = size*objs;
		size_t size_left = end_free - start_free;
		if (size_left >= size_needed){//�ڴ�صĿռ�ԶԶ��������ռ䣬objs����
			result = start_free;
			start_free += size_needed;
			return result;
		}
		else if (size_left >= size){//�ڴ��ֻ���ṩһ������һ�����ϵ����飨�������������
			objs = size_left / size;//objs�ı䣬С��ԭ��ֵ
			result = start_free;
			start_free += objs*size;
			return result;
		}
		else{//�ڴ��һ��size��С�����鶼�޷��ṩ�ˣ����Ҫ���ڴ�ؼ�ˮ
			size_t bytes_to_get = size_needed * 2 + ROUND_UP(heap_size >> 4);//��������ռ䣬
			//�������ſռ����ô������ӵĸ�����
			obj** p;
			if (size_left > 0){//һ��size��С�Ŀռ��޷��ṩ�����ǲ����ڴ��û�пռ��ˣ�
				//�˴�������Ŀռ��������������뵽free_list��
				p = free_list + FREELISTINDEX(size_left);
				((obj*)start_free)->next_obj = *p;
				(*p)->next_obj = (obj*)start_free;
			}
			start_free = (char*)malloc(bytes_to_get);
			if (0 == start_free){//ʹ��mallocʧ�ܣ�������free_list��Ѱ�ҿ���ռ䣬���뵽�ڴ����
				for (int i = size; i <= MAX_BYTES; i += ALIGN){//��Ѱfree_list�����ҿ���ռ�
					p = free_list + FREELISTINDEX(i);
					obj* temp = *p;
					if (temp != 0){
						start_free = (char*)temp;
						(*p)->next_obj = temp->next_obj;
						end_free = start_free + i;
						return (chunk_alloc(size, objs));//�ݹ����chunk_alloc�������޸�objs��ֵ
					}
				}
				end_free = 0;
				start_free = (char*)Malloc_alloc::allocate(bytes_to_get);//ʵ���Ҳ����ռ䣬
				                                      //ֻ�е��õ�һ��������
			}
			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;
			return chunk_alloc(size, objs);
		}
	}
}


