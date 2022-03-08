#ifndef _MEMORY_
#define _MEMORY_

#include "iterator_traits.h"
#include "my_traits.h"
#include "utility.h"
#include "exception.h"

#include <iostream>

namespace hui {
	
    template<typename Alloc>
    class allocator_traits{
        public:
            using allocator_type = Alloc;
            using value_type = typename Alloc::value_type;
            using size_type = typename Alloc::size_type;
            using pointer = typename Alloc::pointer;
            using reference = typename Alloc::reference;
            using r_reference = typename Alloc::r_reference;
            using const_pointer = typename Alloc::const_pointer;
            using const_void_pointer = const void*;
            using difference_type = typename Alloc::difference_type; 
            using propagate_on_container_move_assignment = hui::no_type;//same with swap, copy, etc

            static pointer allocate(Alloc& a_in, size_type sz){
                return a_in.allocate(sz);
            } 
            
            static void deallocate(Alloc& a_in, pointer p, size_type sz){
                a_in.deallocate(p, sz);
            }
            //static destroy, construct

        private:
    };

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
	Iter uninitialized_copy_n(Iter from, Size sz, Iter to){
		using T = typename hui::iterator_traits<Iter>::value_type;
		Iter curr = to;
		try {
			for (; sz > 0; ++from, ++curr, --sz) {
				::new (reinterpret_cast<void*>(curr)) T(*from);
			}
		}
		catch (hui::exception) {
			hui::destroy(to, curr);//stl treats exception in destructor like something that never happens
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
				::new (reinterpret_cast<void*>(&*curr)) T(hui::move(*f));
			}
		}
		catch (hui::exception) {
			hui::destroy(cf, curr);
			throw;
		}

		return curr;
	}
};

#endif

