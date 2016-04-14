#ifndef _STL_ALLOC_H_
#define _STL_ALLOC_H_
#include <iostream>

namespace MINI_STL{
	class Malloc_alloc{///第一级配置器，当所需空间大于128bytes时候使用此配置器
	private:
		static void *oom_malloc(size_t);//当malloc不成功时候使用此函数
		static void *oom_realloc(void*, size_t);//同理
		static void(*malloc_alloc_oom_handler)();
	public:
		static void *allocate(size_t n){
			void* result = malloc(n);
			if (0 == n)result = oom_malloc(n);//此处分配不成功，调用oom malloc
			return result;
		}
		static void deallocate(void *p, size_t n){
			free(p);
		}
		static void* reallocate(void* p, size_t old_size, size_t new_size){
			void * result = realloc(p, new_size);
			if (0 == result){
				result = oom_realloc(p, new_size);//同理
			}
			return result;
		}
		static void(*set_new_handler(void(*f)())){//此处具体查询new handler机制，
			//此处使用c实现c++中::operator new的功能
			void(*old)() = malloc_alloc_oom_handler;
			malloc_alloc_oom_handler = f;
			return (old);
		}
	};
	
	typedef Malloc_alloc malloc_alloc;

	//以下是默认空间配置器，也是第二季配置器，下面将默认将此定义为alloc的标准接口
	enum{ ALIGN = 8 };
	enum{ MAX_BYTES = 128 };
	enum{ NFREELISTS = MAX_BYTES / ALIGN };
	enum{ OBJS = 20 };
	class Default_alloc{
	private:
		static char* start_free;//指向内存池开头
		static char* end_free;//结尾
		static size_t heap_size;   
	private:
		union obj{//free list 节点，使用union而不是struct是由于freelist的特性（obj要么指向实际内存区块，
			//要么指向另一个obj），此方法可以节约内存
			union obj* next_obj;
			char client_data[1];//为什么不用client_data,而使用数组？
		};
		static obj* free_list[NFREELISTS];//free_list

	private:
		static size_t ROUND_UP(size_t bytes){//将bytes上升为ALIGN的倍数，位运算速度很快
			return (bytes + ALIGN - 1) &~(ALIGN - 1);
		}
		static size_t FREELISTINDEX(size_t bytes){//返回freelist的号数
			return (((bytes)+ALIGN - 1) / ALIGN - 1);
		}
		static void *refill(size_t n);//获取大小为n的区块，n默认是align的倍数
		static char* chunk_alloc(size_t size, int& nobjs);//从内存池获取空间的操作，当内存池空间不足时
		//通过malloc给内存池增加空间
	public:
		static void *allocate(size_t n);
		static void deallocate(void *p, size_t n);
		static void reallocate(void *p, size_t old_size, size_t new_size);
	};
	
	typedef Default_alloc default_alloc;
#ifdef USE_MALLOC_ALLOC
	typedef malloc_alloc alloc;
#else
	typedef default_alloc alloc;
#endif
}
#endif

//template <class T, class Alloc >
//class Simple_alloc{
//public:
//	static T* allocate(size_t n){
//		return 0 == n ? 0 : (T*)Alloc::allocate(n*sizeof(T));
//	}
//	static T* allocate(void){
//		return (T*)Alloc::allocate(sizeof(T));
//	}
//	static void deallocate(T* p, size_t n){
//		if (0 != n){
//			Alloc::deallocate(p, n*sizeof(T));
//		}
//	}
//	static void deallocate(T* p){
//		Alloc::deallocate(p, sizeof(T));
//	}
//};