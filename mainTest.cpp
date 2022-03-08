#include <iostream>
#include "vector.h"
#include "allocator.h"
#include "exception.h"
#include "string.h"


template<typename T, typename = hui::enable_if_t<hui::is_same_v<T, int>>>
void test_vector_and_sfinae(const T&) 
{
	hui::vector<std::string> v;
	for (int i = 0; i < 50000; ++i) {
		v.emplace_back("s");
	}

	for (auto& i : v)
		std::cout << i;
}

void test_vector_and_sfinae(...) {
	std::cout << "not allowed\n";
}

void test_exceptions(){
    try{
        int aboba[5];
        std::cout << aboba[1];
        if(aboba[0] != 123123124){
            throw hui::exception();
        }
    }catch(hui::exception& exceptor){
        std::cout << "can handle\n";
        std::cout << exceptor.what();
    }
}

void test_string(){
    hui::string str;
    std::cout << sizeof(str) << "\n";//32
    
    hui::string str2("asdsad");
    std::cout << str2.c_str() << "\n";    
    
    hui::string str3("assssssssssssssssssssssdasdsadsadasdasdsadasdsadasdas\n");
    std::cout << str3.c_str();
    
    hui::string str4 = str3;//copy constr
    std::cout << str4.c_str();
    std::cout << str3.c_str();
}

int main()
{
    test_string();
    test_exceptions();
    test_vector_and_sfinae('s');
}
