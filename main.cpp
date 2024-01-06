#include<iostream>
#include"tests.hpp"
//main.cpp
//Runs all the tests and exits silently
int main(){

  using namespace test;
//Division 1
std::cout << "Division 1" << std::endl;
elem_test();
and_test();
or_test();
all_test();
any_test();
length_test();
sum_test();
min_test();
max_test();
std::cout << "Division 1 tests passed" << std::endl;
//Division 2
std::cout << "Division 2" << std::endl;
foldr_test();
foldr1_test();
foldl_test();
foldl1_test();
product_test();
replicate_test();
iterateN_test();
zip_test();
std::cout << "Division 2 tests passed" << std::endl;
//Division 3
std::cout << "Division 3" << std::endl;
concat_test();
std::cout << "All tests passed" << std::endl;
}
