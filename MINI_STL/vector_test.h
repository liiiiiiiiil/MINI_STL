#ifndef _VEC_TEST_H_
#define _VEC_TEST_H_


#include "vector.h"
#include<vector>
namespace MINI_STL{
	namespace VecTest{
		template <class T>
		using myVec = MINI_STL::vector<T>;
		class TestItem
		{
		public:
			TestItem()
			{
				++count;
			}
			TestItem(const TestItem & other)
			{
				++count;
			}

			virtual ~TestItem()
			{
				--count;
			}

			static int getCount()
			{
				return count;
			}
		private:
			static int count;
		};
		int TestItem::count = 0;
		void test0(){
			/*auto testC = testItem(6);*/
			//
			myVec<int>v0(5, 1);
			myVec<char*>v1(4, "fdfsd");
			myVec<std::string>v2(3, "fdfsd");
			myVec<float>v3(3, 13.5);
			myVec<double>v4(4, 23.56);
			myVec<char>v5(5, 'f');
			myVec<TestItem>v6(5);
			myVec<int>v7(7);
			myVec<char>v8(9);
			myVec<char*>v9(2);
			
			return;
		}
		void test1(){
			int temp;
			int size;
			myVec<int>v(5);
			typedef std::vector<int> stdVec;
			stdVec stdv(5, 10);

			for (int i = 0; i < v.size(); ++i){
				temp = v[i];
			}
			v.push_back(10);
			temp = v[v.size()-1];

			v.insert(v.begin(),5);
			temp = v[0];

			v.insert(v.end(), 15);
			temp = v[v.size() - 1];

			v.insert(v.begin(), 5);
		    temp = v[0];
			size = v.size();

			v.insert(v.begin(), 6);
			temp = v[0];
			size = v.size();

			v.insert(v.begin(), 7);
			temp = v[0];
			size = v.size();


			int arr[] = { 0, 1 };
			v.insert(v.end(), std::begin(arr), std::end(arr));
			temp = v[v.size() - 1];
			size = v.size();

			return;
		}
		void test2(){
			
			
		}
		void test3(){

		}
		void testAll(){
			MINI_STL::VecTest::test0();
			MINI_STL::VecTest::test1();
			MINI_STL::VecTest::test2();
		}
	}

}
#endif