#ifndef  LIST_TEST_H_
#define   LIST_TEST_H_


#include "list.h"
#include <cassert>

namespace  MINI_STL{
	namespace   LIST_TEST{

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

		template <class T>
		using myList = MINI_STL::list<T>;
		template <class T>
		void show_detail(MINI_STL::list<T>& rhs);

		void test0(){//测试构造函数
			//int
			myList<int>l1(3, 2);
			show_detail(l1);

			int arr[3] = { 1,2,3 };
			myList<int>l2(std::begin(arr), std::end(arr));
			show_detail(l2);

			auto l3 = l2;
			show_detail(l3);

			auto l4(l2);
			show_detail(l4);

			//char*
			myList<char*>l5(5,"fuck");
			show_detail(l5);

			//string
			std::string stringList[] = { "fuck", "you", "all" };
			myList<std::string>l6(5,"temp");
			show_detail(l6);

			//float
			myList<float>l7;
			show_detail(l7);

			//wchar_t*
			myList<wchar_t*>l8;
			show_detail(l8);
		}

		void test1(){//成员函数的测试
			myList<int>IntListTest;
			IntListTest.push_back(3);
			IntListTest.push_back(1);
			IntListTest.push_back(4);
			IntListTest.push_back(5);
			IntListTest.push_back(2);

			// empty(),size()/////////////////////////////
			std::cout << "listTest->test1:" << std::endl;
			std::string StringList[] = { "one", "two", "three" };
			myList<std::string>tlist(1, "zero");
			assert(!tlist.empty());
			assert(tlist.size()==1);


			//push_back,push_front/////////////////////////////
			tlist.push_back("zeror");
			tlist.push_front("zerol");
			assert(tlist.front() == "zerol");
			assert(tlist.back() == "zeror");


			//pop_back，pop_front/////////////////////////////
			tlist.pop_back();
			tlist.pop_front();
			assert(tlist.front() == tlist.back());

			//insert///////////////////////////////
			tlist.insert(tlist.begin(), "zerol");
			tlist.insert(tlist.end(), "zeror");
			assert(tlist.front() == "zerol");
			assert(tlist.back() == "zeror");

			tlist.insert(tlist.end(), 1, "one");
			assert(tlist.size() == 4);
			assert(tlist.back() == "one");

			tlist.insert(tlist.end(),std::begin(StringList), std::end(StringList));
			assert(tlist.size() == 7);
			//show_detail(tlist);
			assert(tlist.back() == "three");

			//erase,clear/////////////////////////////////////////
			tlist.erase(tlist.begin());
			assert(tlist.front() == "zero");
			
			tlist.erase(tlist.begin(),tlist.end());
			assert(tlist.size()==0);

			tlist.insert(tlist.end(), std::begin(StringList), std::end(StringList));
			tlist.clear();
			assert(tlist.size() == 0);

			//splice/////////////////////////////////////////////
			tlist.insert(tlist.end(), std::begin(StringList), std::end(StringList));
			myList<std::string>tlist2(1, "fuck");
			tlist.splice(tlist.end(), tlist2);
			assert(tlist.back() == "fuck");


			myList<std::string>tlist3(1, "fuck");
			tlist3.push_back("you");
			tlist3.push_back("all");
			tlist3.push_back("fimilly");
			tlist3.push_back("people");
			assert(tlist3.front() == "fuck");
			assert(tlist3.back()=="people");

			tlist.clear();
			assert(tlist.empty());

			tlist.splice(tlist.begin(),tlist3,tlist3.begin(),tlist3.end());
			/*show_detail(tlist);
			show_detail(tlist3);*/
			auto s = tlist.size();
			assert(tlist.size() == 5);
			assert(tlist.front() == "fuck");
			assert(tlist.back() == "people");
			assert(tlist3.empty());

			auto temp = tlist.back();
			tlist.remove("people");
			assert(tlist.size() == 4);
			temp = tlist.back();
			//show_detail(tlist);
			assert(tlist.back() == "fimilly");

			//unique////////////////////////////////////
			tlist.clear();
			tlist.push_back("one");
			tlist.push_back("one");
			tlist.push_back("two");
			tlist.unique();
			assert(tlist.size() == 2);
			//show_detail(tlist);

			//merge////////////////////////////////
			myList<int>intList1;
			myList<int>intList2;
			intList1.push_back(1);
			intList1.push_back(3);
			intList1.push_back(5);
			intList2.push_back(2);
			intList2.push_back(4);
			intList2.push_back(6);
			intList1.merge(intList2);
			//show_detail(intList1);
			assert(intList1.size() == 6);

			//sort//////////////////////////////
			myList<int>tlist5 = IntListTest;
			assert(tlist5.size() == 5);
			tlist5.sort();
			show_detail(tlist5);
		}
		void testAll(){
			test0();
			test1();
		}


		template <class T>//
		void show_detail(MINI_STL::list<T>& rhs){
			for (auto it = rhs.begin(); it != rhs.end(); ++it){
				std::cout << *it << std::endl;
			}
		}

	}
}
#endif