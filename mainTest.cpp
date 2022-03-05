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
	//while (1) {
	//	test(std::string());
	//	std::cout << "end test\n";
	//}
}
