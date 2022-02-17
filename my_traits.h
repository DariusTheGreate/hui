//need to add is_class(), yes_type, no_type
#ifndef _MY_TRAITS_
#define _MY_TRAITS_

namespace hui{

	template<typename T>
	struct remove_reference {
		using type = T;
	};
	
	template<typename T>
	struct remove_reference<T&> {
		using type = T;
	};
	
	template<typename T>
	using remove_reference_t = typename remove_reference<T>::type;
	
	template<typename T, typename U>
	struct is_same {
		static const bool val = false;
	};
	
	template<typename T>
	struct is_same<T, T>{
		static const bool val = true;
	};
	
	template<typename T, typename U>
	const bool is_same_v = is_same<T, U>::val;
	
	template<typename T>
	struct is_const{
		static const bool val = false;
	};
	
	template<typename T>
	struct is_const<const T>{
		static const bool val = true;
	};
	
	template<typename T>
	const bool is_const_v = is_const<T>::val;
	
	template<bool B>
	struct enable_if{
	};
	
	template<>
	struct enable_if<true>{
		using type = bool;
	};
	
	template<bool B>
	using enable_if_t = typename enable_if<B>::type;
};
#endif

