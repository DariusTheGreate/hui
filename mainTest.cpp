#include <iostream>
#include "vector.h"
#include "allocator.h"
#include "exception.h"
#include "string.h"
#include "shared_ptr.h"
#include "unique_ptr.h"
#include "deleters.h"
#include "lf_queue.h"
#include "deleters.h"

template<typename T, typename = hui::enable_if_t<hui::is_same_v<T, int>>>
void test_vector_and_sfinae(const T&) 
{
	hui::vector<hui::string> v;
	for (int i = 0; i < 100; ++i) {
		v.emplace_back((char*)"|");
	}

    hui::vector<hui::string> v2 = hui::move(v);
    
    for (auto& i : v)
	    std::cout << i.c_str();//empty
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
    
    hui::string str2((char*)"asdsad");
    std::cout << str2.c_str() << "\n";    
    
    hui::string str3((char*)"assssssssssssssssssssssdasdsadsadasdasdsadasdsadasdas\n");
    std::cout << str3.c_str();
    
    hui::string str4 = str3;//copy constr
    std::cout << str4.c_str();
    std::cout << str3.c_str();
    str4 = str3;
    std::cout << str4.c_str();
    std::cout << str3.c_str();
    
    hui::string str5((char*)"moved strasssssssssssssssssssssssssss");
    std::cout << "1 "  << str5.c_str() << "|\n";
    hui::string str6(hui::move(str5));
    std::cout << "2 " << str6.c_str() << "|\n";
    std::cout << "3 " << str5.size();
}

void pass_shared_ptr_by_value(hui::shared_ptr<hui::string> sp){
    std::cout << "inside function: " << (*sp).c_str() << "\n";
    sp.print_status();
}

void test_shred_ptr(){
    hui::shared_ptr<hui::string> sp(new hui::string((char*)"asdasdasd"), hui::Allocator<hui::string>(), hui::standartDeleter<hui::string>); 
    sp.print_status();
    {
        auto sp2 = sp;
        sp.print_status();
        pass_shared_ptr_by_value(sp2);
    }

    sp.print_status();
}

void test_unique_ptr(){
    hui::unique_ptr<hui::string> ptr(new hui::string((char*)"hello from shitter\n"));
    std::cout << "1: " << (*ptr).c_str();
    auto ptr2 = hui::move(ptr);
    std::cout << "2: " << (*ptr2).c_str();
    std::cout << "3: " << (ptr.get_ptr()) << "|";    
}

void test_lf_queue(){
    hui::lf_queue<hui::string> q;
    q.push("asdasdas");
}

int main()
{
    //test_unique_ptr();
    //test_shred_ptr();
    //test_string();
    //test_exceptions();
    //test_vector_and_sfinae(9);
    std::cout << "exit\n";
}

