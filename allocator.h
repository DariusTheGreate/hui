#ifndef _ALLOCATOR_
#define _ALLOCATOR_

#include <limits>//make own
#include <memory>
#include "utility.h"
#include "huidef.h"

//std::allocator<int> a;

namespace hui{
    //stupid allocator, that does nothink dangerous
    template<typename T>
	class DummyAllocator{
	public:
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using r_reference = T&&;
		using const_pointer = const T*;
		using const_reference = const T&;
		using size_type = hui::size_t;
		using difference_type = hui::ptrdiff_t;

		//departed in c++20
		template<class Type>
		struct rebind {
			using other = DummyAllocator<Type>;
		};

		DummyAllocator() = default;
		DummyAllocator(const DummyAllocator& alloc) { };
		~DummyAllocator() {};

		template<class U>
		DummyAllocator(const DummyAllocator<U>& alloc) {}

		pointer address(reference x) const { return &x; }
		//const_pointer address(const_reference x) const {return &x; };

		pointer allocate(size_type n){
            return nullptr;
		}

		void deallocate(pointer ptr, size_type n){
		}

		void construct(pointer p, const_reference val){
		}

		template<typename... Args>
		void construct(pointer p, Args&&... val) {
		}

		void destroy(pointer p){
		}
	};


	template<typename T>
	class Allocator{
	public:
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using r_reference = T&&;
		using const_pointer = const T*;
		using const_reference = const T&;
		using size_type = hui::size_t;
		using difference_type = hui::ptrdiff_t;

		//departed in c++20
		template<class Type>
		struct rebind {
			using other = Allocator<Type>;
		};

		Allocator() = default;
		Allocator(const Allocator& alloc) = default;
		~Allocator() {};

		template<class U>
		Allocator(const Allocator<U>& alloc) {}

		pointer address(reference x) const { return &x; }
		//const_pointer address(const_reference x) const {return &x; };
        
        //TODO: ad preprocessor directives for enabling/disabling branches?
		pointer allocate(size_type n){
			//if(n > this->max_size())
		    //  return nullptr;
			return reinterpret_cast<pointer>(::operator new(n * sizeof(value_type)));
		}
        
        //TODO:test it; is there new overloading for alignment?
        pointer allocate(size_type n, int alignment){
			//if(n > this->max_size())
			//	return nullptr;
           
            size_type size_in_bytes = n * sizeof(value_type); 
            size_type aligned_size = (((size_in_bytes) + ((alignment) - 1)) & ~((alignment) - 1)); 

			return reinterpret_cast<pointer>(::operator new(aligned_size));
		}
        
        //TODO: delete[](char*) ptr here?
		void deallocate(pointer ptr, size_type n){
			::operator delete(ptr);
		}

		void construct(pointer p, const_reference val){
			::new (reinterpret_cast<void*>(p)) value_type(val);
		}

		template<typename... Args>
		void construct(pointer p, Args&&... val) {
			::new (reinterpret_cast<void*>(p)) value_type(hui::forward<Args>(val)...);
		}

		void destroy(pointer p){
			p->~value_type();
		}

		//departed in c++20
		size_type max_size() const{
			return std::numeric_limits<size_type>::max() / sizeof(value_type);
		}

        //no needs for ==, != operators. cause they are using same new/ delete.
	};

//template<>
//class Allocator<void>{
//public:
//	using value_type = T;
//	using pointer = T*;
//	using const_pointer = const pointer;	

//	template<class Type>
//	struct rebind{
//		using other = Allocator<Type> other;
//	}
//}
};

#endif

