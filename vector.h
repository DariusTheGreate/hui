#ifndef _VECTOR_
#define _VECTOR_

#include <iostream>
#include "reverse_iterator.h"
#include "allocator.h"

namespace hui{
	
	template<typename T, typename Allocator = hui::Allocator<T>>
	class vector final{
	public:
		using value_type = T;
		using allocator_type = Allocator;
		using size_type = size_t;
		using reference = value_type&;
		using r_reference = value_type&&;
		using const_reference = const reference;
		using pointer = T*;
		using iterator = T*;
		using const_iterator = const iterator;
		using reverse_iterator = hui::reverse_iterator<const_iterator>;
	
	private:
		T* buffer{nullptr};
		int size{0};//not size_t
		int cap{0};
		Allocator alloc;
	
	public:
		vector() = default;
		~vector();
		vector(const_reference v);
		vector(r_reference v);
		reference operator =(const_reference v);
		reference operator =(r_reference v);

		void push_back(const_reference val);

		template<typename... Args>
		void emplace_back(Args&&... args);

		void reserve(size_type sz);
	};

	template<typename T, typename Allocator>
	vector<T, Allocator>::~vector(){
		alloc.deallocate(buffer, size);//use alloc delete here, of course;
	}

	template<typename T, typename Allocator>
	vector<T, Allocator>::~vector(){
		alloc.deallocate(buffer, size);//use alloc delete here, of course;
	}
};

#endif

