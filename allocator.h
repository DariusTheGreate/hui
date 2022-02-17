#ifndef _ALLOCATOR_
#define _ALLOCATOR_

#include <limits>

namespace hui{

	template<typename T>
	class Allocator{
	public:
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using r_reference = T&&;
		using const_pointer = const T*;
		using const_reference = const T&;
		using size_type = size_t;
		using difference_type = ptrdiff_t;

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

		pointer allocate(size_type n){
			if(n > this->max_size())
				return nullptr;
			return reinterpret_cast<pointer>(::operator new(n * sizeof(value_type)));
		}

		void deallocate(pointer ptr, size_type n){
			::operator delete(ptr);
		}

		void construct(pointer p, const_reference val){
			::new (reinterpret_cast<void*>(p)) value_type(val);
		}

		template<typename... Args>
		void construct(pointer p, Args&&... val) {
			::new (reinterpret_cast<void*>(p)) value_type(std::forward<Args>(val)...);
		}

		void destroy(pointer p){
			p->~value_type();
		}

		//departed in c++20
		size_type max_size() const{
			return std::numeric_limits<size_type>::max() / sizeof(value_type);
		}
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

