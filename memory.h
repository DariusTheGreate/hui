#include "iterator_traits.h"
#include "utility.h"

namespace hui {
	
	template<class Iter>
	Iter destroy(Iter f, Iter l) {
		using T = typename hui::iterator_traits<Iter>::value_type;
		Iter curr = f;
		for (; curr < l; curr++) {
			curr->~T();//not really
		}
		return curr;
	}

	template<class Iter, class Size>
	Iter uninitialized_copy_n(Iter f, Size sz, Iter cf){
		using T = typename hui::iterator_traits<Iter>::value_type;
		Iter curr = cf;
		try {

			for (; sz > 0; ++f, ++curr, --sz) {
				::new (reinterpret_cast<void*>(curr)) T(*f);
			}
		}
		catch (std::exception) {
			hui::destroy(cf, curr);//stl treats exception in destructor like something that never happens
			throw;
		}

		return curr;
	}

	template<class Iter, class Size>
	Iter uninitialized_move_n(Iter f, Size sz, Iter cf){
		using T = typename hui::iterator_traits<Iter>::value_type;
		Iter curr = cf;
		try {
			for (; sz > 0; ++f, ++curr, --sz) {
				//*curr = std::move(*f);
				::new (reinterpret_cast<void*>(&*curr)) T(hui::move(*f));
			}
		}
		catch (std::exception) {
			hui::destroy(cf, curr);
			throw;
		}

		return curr;
	}
};

