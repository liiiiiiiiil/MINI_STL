#include "../stl_alloc.h"
#include <new>
#include <iostream>
///下面是第一级配置器
namespace MINI_STL{
	void(*Malloc_alloc::malloc_alloc_oom_handler)() = 0;
	void* Malloc_alloc::oom_malloc(size_t size){
		void(*my_malloc_handler)();
		void* result;
		for (;;){
			my_malloc_handler = malloc_alloc_oom_handler;//可以自己设置malloc_alloc_oom_handler，
			//当分配失败时候就可以调用malloc_alloc_oom_handler，类似new handler机制
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
			my_malloc_handler = malloc_alloc_oom_handler;//同上
			if (0 == my_malloc_handler){
				std::cout << "out of memory" << std::endl;
				exit(1);
			}
			(*my_malloc_handler)();
			result = realloc(p, new_size);
			if (result)return(result);
		}
	}
	//第二级配置器,
	char* Default_alloc::start_free = 0;//静态成员变量赋值
	char* Default_alloc::end_free = 0;
	size_t Default_alloc::heap_size = 0;
	Default_alloc::obj* Default_alloc::free_list[NFREELISTS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	void* Default_alloc::allocate(size_t size){
		if (size > 128){//bytes大于128，所以调用malloc_alloc
			return Malloc_alloc::allocate(size);
		}
		obj** my_free_list;
		obj* result;
		my_free_list = free_list + FREELISTINDEX(size);
		result = *my_free_list;
		if (0 == result){//该处自由链表空间不足
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
	void * Default_alloc::refill(size_t size){//默认size是ALIGN的倍数
		int objs = OBJS;
		char* chunks;
		obj** my_free_list;
		obj* f_cur_obj, *f_next_obj;
		void *result;
		chunks = chunk_alloc(size, objs);//从内存池获取空间的操作，当内存池空间不足时
		//通过malloc给内存池增加空间,objs按引用传递
		if (1 == objs){//刚好一块空间
			return chunks;
		}
		//获取了多个区块
		my_free_list = free_list + FREELISTINDEX(size);
		result = chunks;
		*my_free_list = f_cur_obj = (obj*)(chunks + size);
		//以下将获取的多个区块加入到free_list中，所以以下是节点的链接操作
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
		if (size_left >= size_needed){//内存池的空间远远大于所需空间，objs不变
			result = start_free;
			start_free += size_needed;
			return result;
		}
		else if (size_left >= size){//内存池只能提供一个或者一个以上的区块（少于所需个数）
			objs = size_left / size;//objs改变，小于原先值
			result = start_free;
			start_free += objs*size;
			return result;
		}
		else{//内存池一个size大小的区块都无法提供了，因此要给内存池加水
			size_t bytes_to_get = size_needed * 2 + ROUND_UP(heap_size >> 4);//两倍所需空间，
			//加上随着空间配置次数增加的附加量
			obj** p;
			if (size_left > 0){//一个size大小的空间无法提供，但是不代内存池没有空间了，
				//此处将残余的空间利用起来，加入到free_list中
				p = free_list + FREELISTINDEX(size_left);
				((obj*)start_free)->next_obj = *p;
				(*p)->next_obj = (obj*)start_free;
			}
			start_free = (char*)malloc(bytes_to_get);
			if (0 == start_free){//使用malloc失败，所以在free_list中寻找空余空间，加入到内存池中
				for (int i = size; i <= MAX_BYTES; i += ALIGN){//搜寻free_list，查找空余空间
					p = free_list + FREELISTINDEX(i);
					obj* temp = *p;
					if (temp != 0){
						start_free = (char*)temp;
						(*p)->next_obj = temp->next_obj;
						end_free = start_free + i;
						return (chunk_alloc(size, objs));//递归调用chunk_alloc函数，修改objs的值
					}
				}
				end_free = 0;
				start_free = (char*)Malloc_alloc::allocate(bytes_to_get);//实在找不到空间，
				                                      //只有调用第一级配置器
			}
			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;
			return chunk_alloc(size, objs);
		}
	}
}


