#ifndef _STL_ALLOC_H_
#define _STL_ALLOC_H_
#include <iostream>

namespace MINI_STL{
	class Malloc_alloc{///��һ����������������ռ����128bytesʱ��ʹ�ô�������
	private:
		static void *oom_malloc(size_t);//��malloc���ɹ�ʱ��ʹ�ô˺���
		static void *oom_realloc(void*, size_t);//ͬ��
		static void(*malloc_alloc_oom_handler)();
	public:
		static void *allocate(size_t n){
			void* result = malloc(n);
			if (0 == n)result = oom_malloc(n);//�˴����䲻�ɹ�������oom malloc
			return result;
		}
		static void deallocate(void *p, size_t n){
			free(p);
		}
		static void* reallocate(void* p, size_t old_size, size_t new_size){
			void * result = realloc(p, new_size);
			if (0 == result){
				result = oom_realloc(p, new_size);//ͬ��
			}
			return result;
		}
		static void(*set_new_handler(void(*f)())){//�˴������ѯnew handler���ƣ�
			//�˴�ʹ��cʵ��c++��::operator new�Ĺ���
			void(*old)() = malloc_alloc_oom_handler;
			malloc_alloc_oom_handler = f;
			return (old);
		}
	};
	
	typedef Malloc_alloc malloc_alloc;

	//������Ĭ�Ͽռ���������Ҳ�ǵڶ��������������潫Ĭ�Ͻ��˶���Ϊalloc�ı�׼�ӿ�
	enum{ ALIGN = 8 };
	enum{ MAX_BYTES = 128 };
	enum{ NFREELISTS = MAX_BYTES / ALIGN };
	enum{ OBJS = 20 };
	class Default_alloc{
	private:
		static char* start_free;//ָ���ڴ�ؿ�ͷ
		static char* end_free;//��β
		static size_t heap_size;   
	private:
		union obj{//free list �ڵ㣬ʹ��union������struct������freelist�����ԣ�objҪôָ��ʵ���ڴ����飬
			//Ҫôָ����һ��obj�����˷������Խ�Լ�ڴ�
			union obj* next_obj;
			char client_data[1];//Ϊʲô����client_data,��ʹ�����飿
		};
		static obj* free_list[NFREELISTS];//free_list

	private:
		static size_t ROUND_UP(size_t bytes){//��bytes����ΪALIGN�ı�����λ�����ٶȺܿ�
			return (bytes + ALIGN - 1) &~(ALIGN - 1);
		}
		static size_t FREELISTINDEX(size_t bytes){//����freelist�ĺ���
			return (((bytes)+ALIGN - 1) / ALIGN - 1);
		}
		static void *refill(size_t n);//��ȡ��СΪn�����飬nĬ����align�ı���
		static char* chunk_alloc(size_t size, int& nobjs);//���ڴ�ػ�ȡ�ռ�Ĳ��������ڴ�ؿռ䲻��ʱ
		//ͨ��malloc���ڴ�����ӿռ�
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