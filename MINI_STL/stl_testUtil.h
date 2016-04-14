#ifndef _STL_TESTUTIL_H_
#define _STL_TESTUTIL_H_
#include <iterator>
#include <iostream>
#include <string>

namespace MINI_STL{
	namespace TEST_UTIL{
		template<class Container1,class Container2>
		bool container_equl(Container1& con1, Container2& con2){
			auto first1 = std::begin(con1), last1 = std::end(con1);
			auto first2 = std::begin(con2), last2 = std::end(con2);
			for (; first1 != last1&&first2 != last2; ++first1, ++first2){
				if (*first1 != *first2)return false;
			}
			return(first1 == last1&&first2 == last2);
		}



	}


}



#endif