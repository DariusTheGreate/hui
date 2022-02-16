#ifndef _REVERSE_ITERATOR_
#define _REVERSE_ITERATOR_

#include "iterator_traits.h"
namespace hui{
	
	template<class Iter>
	class reverse_iterator{
	public:
		using iterator_type = Iter;
		using iterator_category = typename hui::iterator_traits<Iter>::iterator_category;
		using value_type = typename hui::iterator_traits<Iter>::value_type;
		using difference_type = typename hui::iterator_traits<Iter>::difference_type;
		using pointer = typename hui::iterator_traits<Iter>::pointer;
		using reference = typename hui::iterator_traits<Iter>::reference;
	private:
		iterator_type _Iter;
	public:
		constexpr reverse_iterator() = default;
		constexpr explicit reverse_iterator(iterator_type it) : _Iter(it) {}
		
		template<class SomeIter>
		reverse_iterator(const reverse_iterator<SomeIter>& it) : _Iter(it.base()) {}
	
		reference operator*() const {
			iterator_type it_tmp = _Iter;
			return (*(--it_tmp));
		}
	
		reference operator[](difference_type n) const { return _Iter[-n-1];}

		pointer operator ->() const { return _Iter; }
		
		reverse_iterator operator +(difference_type n) const { return reverse_iterator(_Iter - n); }
		reverse_iterator& operator +=(difference_type n) { _Iter-=n; return *this; }
		reverse_iterator& operator ++() { --_Iter; return *this; }
		reverse_iterator operator ++(int) { reverse_iterator tmp = *this; --(*this); return tmp; }

		reverse_iterator operator -(difference_type n) const { return reverse_iterator(_Iter + n); }
		reverse_iterator& operator -=(difference_type n) { _Iter+=n; return *this; }
		reverse_iterator& operator --() { ++_Iter; return *this; }
		reverse_iterator operator --(int) { reverse_iterator tmp = *this; ++(*this); return tmp; }

		iterator_type base() const { return _Iter; }	
	};

	template<class Iter>
	bool operator ==(const reverse_iterator<Iter>& f, const reverse_iterator<Iter>& s){
		return f.base() == s.base();
	}
	//...
};

#endif

