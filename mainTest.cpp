#include <iostream>
#include <vector>

#include "my_traits.h"
#include "vector.h"
#include "allocator.h"
#include <string>
#include "speakableStruct.h"

int main(){
	hui::remove_reference_t<int&> x = 8;
	std::cout << hui::is_same_v<int, int>;
	std::cout << hui::is_const_v<decltype(x)>;
	//{
	//	hui::vector<int> v;
	//	v.push_back(10);
	//	v.push_back(11);
	//
	//	std::cout << v.at(1);
	//}

	//{
	//	hui::vector<std::string> vs;
	//	vs.push_back("asdasfaf");
	//	vs.push_back("sdadsasdas");
	//	std::cout << vs.at(1);
	//}

	//{
	//	std::vector<hui::Speakable> vs;
	//	for(size_t i = 0; i < 100; ++i)
	//		vs.push_back(hui::Speakable());
	//	int c =0;
	//	for(size_t i = 0; i < 100; ++i)
	//		std::cout << &vs.at(i) << "\n";
	//	//vs.push_back(hui::Speakable());
	//	//vs.push_back(hui::Speakable());
	//}
	
	{
		hui::vector<std::string> vs;
		for(size_t i = 0; i < 1000000; ++i)
			vs.push_back(std::string("asdasdasd"));
		//for(size_t i = 0; i < 1000001; ++i)
		//	std::cout << &vs.at(i) << "\n";
	}

	std::cout << "\nexit\n";
	return 0;
}
