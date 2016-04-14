#ifndef _ALLOC_TEST_H_
#define _ALLOC_TEST_H_
#include"stl_allocator.h"
#include "stl_uninitialized.h"
#include <string>
#define VALUE_NUM 10
namespace MINI_STL{
	namespace AllocTest{
		template<class T,class Alloc=allocator<T>>//此类是为了验证allocator的正确性
		                                                                   //从分配内存再到填充内容
		class v{
		public:
			typedef T value_type;
			typedef T* pointer;
			typedef Alloc allocator;
		public:
			v(const value_type& value);
		};
		template<class T,class Alloc>
		v<T,Alloc>::v(const value_type& value){
			T* p=static_cast<T*>(allocator::allocate(VALUE_NUM));
			MINI_STL::uninitialized_fill_n(p,VALUE_NUM,value);
			return;
		}
		void test_0(){
			v<int>li(10);
			return;
		}
		void test_1(){
			v<char>li('a');
			return;
		}
		void test_2(){
			v<std::string>li("fuck");
			return;
		}
		void test_3(){
			v<float>li(13.50);
			return;
		}
		void test_4(){
			v<double>li(34.6);
			return;
		}
		void test_5(){
			auto li = std::string("fuck");
			v<std::string>hao(li);
			return;
		}
		void testAll(){
			MINI_STL::AllocTest::test_0();
			MINI_STL::AllocTest::test_1();
			MINI_STL::AllocTest::test_2();
			MINI_STL::AllocTest::test_3();
			MINI_STL::AllocTest::test_4();
			MINI_STL::AllocTest::test_5();
		}
	}
}



#endif