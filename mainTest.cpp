#include <iostream>

#include "my_traits.h"
#include "vector.h"
#include "allocator.h"

int main(){
	hui::remove_reference_t<int&> x = 8;
	std::cout << hui::is_same_v<int, int>;
	std::cout << hui::is_const_v<decltype(x)>;
	hui::vector<int> v;
	return 0;
}
