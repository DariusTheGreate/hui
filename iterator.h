#ifndef _ITERATOR_
#define _ITERATOR_

//#include "my_traits.h"
namespace hui{
	template<typename Category, typename T, typename Distance = ptrdiff_t, 
		typename Pointer = T*, typename Reference = T&>
	class Iterator{
		using iterator_category = Category;
		using value_type = T;
		using difference_type = Distance;
		using pointer = Pointer;
		using reference = Reference;
	};
	
	//check if Iter is reverse_iterator/move_iterator/etc
	//template<typename Iter>
	//class is_iterator_wrapper{
	//public:
	//	static const bool value = is_same_v<decltype(test<Iter>(NULL)), yes_type>;
	//private:
	//	template<typename>
	//	static no_type test(...);
	//
	//	template<typename T>
	//	static yes_type test(typename T::wrapped_iterator_type*, typename enable_if<is_class<T>::value>::type);
	//};
};
#endif

