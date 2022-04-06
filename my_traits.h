//need to add is_class(), is_base_of(), is_pod(), is_scalar(), is_enum(), etc..
#ifndef _MY_TRAITS_
#define _MY_TRAITS_

#include <type_traits>

#include "huidef.h"

namespace hui{
    
    template<typename T, T val>
    struct integral_constant{
        static constexpr T value = val;
        using value_type = T;
        using type = integral_constant;
        constexpr operator value_type() const noexcept { return value; }
        constexpr value_type operator() () const noexcept {return value; }
    };
    
    struct yes_type : integral_constant<bool, true> {};
    struct no_type : integral_constant<bool, false> {};
 
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
    
    template<typename>
    struct is_array : public no_type { };

    template<typename T, hui::size_t Size>
    struct is_array<T[Size]> : public yes_type { };

    template<typename T>
    struct is_array<T[]> : public yes_type { };

	template<bool B>
	struct enable_if{
	};
	
	template<>
	struct enable_if<true>{
		using type = bool;
	};
	
	template<bool B>
	using enable_if_t = typename enable_if<B>::type;
    
    //TODO:add sepcializations
    template<typename T, bool IsArray = hui::is_array<T>::value, bool IsFunc = std::is_function<T>::value>
    struct decay_dispatch;

    template<typename T>
    struct decay_dispatch<T, false, false>{
        using type = remove_reference_t<T>;
    };

    template<typename T>
    class decay
    {
        using remove_type = hui::remove_reference_t<T>;
    public:
        using type = decay_dispatch<remove_type>;//here should be decaying in case its 
    };
   

};
#endif

