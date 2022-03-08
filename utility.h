#ifndef _UTILITY_
#define _UTILITY_

#include "my_traits.h"

namespace hui{
	template<typename T>
	hui::remove_reference_t<T>&& move(T&& v) noexcept{
		return static_cast<hui::remove_reference_t<T>&&>(v);
	}

	template<typename T>
	T&& forward(hui::remove_reference_t<T>& x) {
		return static_cast<T&&>(x);
	}

    template<typename T>
    void swap(T& v1, T& v2){
        T v3 = hui::move(v1);
        v1 = hui::move(v2);
        v2 = hui::move(v3);
    }
}

#endif

