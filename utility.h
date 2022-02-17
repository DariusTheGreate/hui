#pragma once

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
}
