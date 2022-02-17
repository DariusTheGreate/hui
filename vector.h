#ifndef _VECTOR_
#define _VECTOR_

#include <iostream>
#include "reverse_iterator.h"
#include "allocator.h"
#include "memory.h"
#include "utility.h"

namespace hui{
	
	template<typename T, typename Allocator = hui::Allocator<T>>
	class vector final{
	public:
		using value_type = T;
		using allocator_type = Allocator;
		using size_type = int;
		using reference = value_type&;
		using r_reference = value_type&&;
		using const_reference = const T&;
		using pointer = T*;
		using iterator = T*;
		using const_iterator = const iterator;
		using reverse_iterator = hui::reverse_iterator<const_iterator>;
	
	private:
		T* buffer{nullptr};
		size_type size{0};//not size_t
		size_type cap{0};
		Allocator alloc;
	
	public:
		vector() = default;
		~vector();
		vector(const_reference v);
		vector(r_reference v);
		vector<T, Allocator>& operator =(const hui::vector<T, Allocator>& v);
		vector<T, Allocator>& operator =(hui::vector<T, Allocator>&& v);

		void push_back(const_reference val);

		template<typename... Args>
		void emplace_back(Args&&... args);

		void reserve(size_type sz);
		void clear();

		value_type& at(int i) const {if(i >= size || i < 0) throw std::out_of_range("at doesnt work\n"); return buffer[i]; }
		int get_size() const { return size; }

		iterator begin() const {
			return buffer;
		}	

		iterator end() const {
			return buffer + size;
		}

		//other trivial stuff
	};

	template<typename T, typename Allocator>
	vector<T, Allocator>::~vector(){
		clear();
		alloc.deallocate(buffer, size);
	}

	template<typename T, typename Allocator>
	void vector<T, Allocator>::clear(){
		//add destroy_n in memory.h
		for(int i = 0; i < size; ++i)
			alloc.destroy(&(buffer[i]));
	}

	template<typename T, typename Allocator>
	vector<T, Allocator>::vector(const_reference v) : size(v.size), cap(v.cap), buffer(alloc.allocate(v.cap)){
		hui::uninitialized_copy_n(v.buffer, v.size, buffer);
	}

	template<typename T, typename Allocator>
	vector<T, Allocator>::vector(r_reference v) : size(v.size), cap(v.cap), buffer(v.buffer){
		v.buffer = nullptr;
		v.cap = 0;
		v.size = 0;
	}

	template<typename T, typename Allocator>
	vector<T, Allocator>& vector<T, Allocator>::operator =(const hui::vector<T, Allocator>& v){
		if(this == &v)
			return *this;
		if(size < v.size)
			reserve(v.size);

		hui::uninitialized_copy_n(v.buffer, v.size, buffer);
		size = v.size;
	}

	template<typename T, typename Allocator>
	void hui::vector<T, Allocator>::reserve(size_type cap_in){
		//std::cout << "\nreserve1\n";
		if(cap_in == 0)
			cap_in = 1;
		
		pointer tmp = alloc.allocate(cap_in);
		//pointer tmp = new T[cap_in];
		hui::uninitialized_move_n(buffer, size, tmp);
		clear();
		alloc.deallocate(buffer, size);
		buffer = tmp;
		cap = cap_in;
		//std::cout << "\nreserve2\n";
	}

	template<typename T, typename Allocator>
	void vector<T, Allocator>::push_back(const_reference val){
		//std::cout << "\npush_back1\n";
		if(size >= cap)
			reserve(2 * cap);
		//buffer[size++] = val;
		alloc.construct(buffer + size, val);
		size++;
		//std::cout << "\npush_back2\n";
	}

	template<typename T, typename Allocator>
	template<typename... Args>
	void vector<T, Allocator>::emplace_back(Args&&... val){
		if(size >= cap)
			reserve(2 * cap);
		//buffer[size++] = new T(std::forward(val)...);
		alloc.construct(buffer + size, hui::forward<Args>(val)...);
		size++;
	}
};

#endif

