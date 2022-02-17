#include "iterator_traits.h"

namespace hui {
	//much simplier then std version:(. there is important stuff with exceptions
	template<class Iter, class Size>
	Iter uninitialized_copy_n(Iter f, Size sz, Iter cf){
		using T = typename hui::iterator_traits<Iter>::value_type;
		Iter curr = cf;
		for(; sz > 0; ++f, ++curr, --sz){
			::new (reinterpret_cast<void*>(curr)) T(*f);
		}

		return curr;
	}

	template<class Iter, class Size>
	Iter uninitialized_move_n(Iter f, Size sz, Iter cf){
		using T = typename hui::iterator_traits<Iter>::value_type;
		Iter curr = cf;
		for(; sz > 0; ++f, ++curr, --sz){
			//*curr = std::move(*f);
			::new (reinterpret_cast<void*>(&*curr)) T(std::move(*f));
		}

		return curr;
	}


};

