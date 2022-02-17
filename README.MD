## My simple implementation of c++ stl. (Currently only vector)

Planned to write:
- [x] basic type_traits
- [x] some iterator/allocator stuff
- [x] vector
- [ ] variant
- [ ] shared_ptr
- [ ] lockFree API

You can clone this repo and add to ypur project any file you want. Then you need to access hui namespace, like this:
```
#include <iostream>
#include "vector.h"
#include "allocator.h"


template<typename T, typename = hui::enable_if_t<hui::is_same_v<T, int>>>
void test(const T&) 
{
	hui::vector<std::string> v;
	for (int i = 0; i < 50000; ++i) {
		v.emplace_back("ssssssssssssssssssssssssssss");
	}

	for (auto& i : v)
		std::cout << i;
}

void test(...) {
	std::cout << "not allowed\n";
}

int main()
{
	while (1) {
		test(std::string());
		std::cout << "end test\n";
	}
}
```
##### Note: Of course its impossible to use!
