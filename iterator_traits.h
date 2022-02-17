#ifndef _ITERATOR_TRAITS_
#define _ITERATOR_TRAITS_

#include "iterator.h"
#include "iterators_tags.h"
namespace hui{
	
	template<class Iter>
	class iterator_traits{
	public:
		using difference_type = typename Iter::difference_type;
		using value_type = typename Iter::value_type;
		using pointer = typename Iter::pointer;
		using reference = typename Iter::reference;
		using iterator_category = typename Iter::iterator_category;
	};
	
	template<class T>
	class iterator_traits<T*>{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = hui::contiguous_iterator_tag;//here is some problems
	};
	
	template<class T>
	class iterator_traits<const T*>{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = const T*;
		using reference = const T&;
		using iterator_category = hui::contiguous_iterator_tag;//here is some problems
	};
	
};
#endif

