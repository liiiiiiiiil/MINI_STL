#ifndef STL_FUNCTiONAL_H_
#define STL_FUNCTIONAL_H_
namespace MINI_STL{
	template <class T>
	struct less{
		typedef T first_argument_type;
		typedef T second_argument_type;
		typedef bool result_type;
		result_type operator()(const first_argument_type& lhs, const second_argument_type& rhs){
			return lhs < rhs;
		}
	};
	template <class T>
	struct equal_to{
		typedef T first_argument_type;
		typedef T second_argument_type;
		typedef bool result_type;
		result_type operator()(const first_argument_type& lhs, const second_argument_type& rhs){
			return lhs == rhs;
		}
	};

}



#endif