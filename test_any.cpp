#include <gtest/gtest.h>

#include "any.h"
#include "utility.h"
#include "string.h"

//using test_type = std::experimental::any;
using test_type = hui::any;

struct Data{
        int a;
        int b;
        int c;
        int o;
        hui::string p; 
    Data() : a(1) {}
    Data(const Data&) = default;
    Data(Data&&) {}
};

struct SimpleData{
    int a;
    SimpleData(){
        creations++;
        memoryLeaks++;
        std::cout << "SimpleData()\n";
    }

    ~SimpleData(){
        memoryLeaks--;
        std::cout << "~SimpleData()\n";
    }

    static int creations;
    static int memoryLeaks;
};

int SimpleData::creations = 0;
int SimpleData::memoryLeaks = 0;

template<typename T>
int test_any_creation(T&& v){
    test_type a = hui::forward<T>(v); 
    return 0;
}

TEST(any_compilation_test, PositiveNos){
    EXPECT_TRUE(test_any_creation(6) == 0) << "failed test with rvalue int";
    EXPECT_TRUE(test_any_creation(Data()) == 0) << "failed test with rvalue Data";
    EXPECT_TRUE(test_any_creation(std::string()) == 0) << "failed test with rvalue std::string";
    EXPECT_TRUE(test_any_creation(hui::string()) == 0) << "failed test with rvalue hui::string";
    EXPECT_TRUE(test_any_creation(std::vector<int>()) == 0) << "failed test with rvalue std::vector<int>";
    EXPECT_TRUE(test_any_creation(std::vector<std::vector<std::vector<std::vector<std::vector<std::string>>>>>()) == 0) << "failed test with rvalue std::vector<...<std::string>>";
}

bool test_any_creation(){
    test_type a = SimpleData();
    std::cout << SimpleData::creations << " " << SimpleData::memoryLeaks << "\n"; 
    return (SimpleData::creations) == 1 && (SimpleData::memoryLeaks == 0); 
}

bool test_any_copying(){
    test_type a = 5;
    test_type b = a;
    return (SimpleData::creations) == 1 && (SimpleData::memoryLeaks == 0); 
}

TEST(any_methods_test, PositiveNos){
    EXPECT_TRUE(test_any_creation()) << "test_any_creation failed\n";
    EXPECT_TRUE(test_any_copying()) << "test_any_copying failed\n";
}

int main(){
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
