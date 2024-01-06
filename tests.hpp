#pragma once
#include<iostream>
#include<cassert>
#include "functions.hpp"

namespace test {
void elem_test() {
static_assert(vlist::elem<1>(vlist::List<4,3,2,1> ()));
static_assert(vlist::elem<'a'>(vlist::List<'a','b','c'> ()));
static_assert(!(vlist::elem<'a'>(vlist::List<4,3,2,1> ())));
static_assert(!(vlist::elem<5>((vlist::List<4,3,2,1> ()))));
}

void and_test() {
//base case for and, empty list is true
static_assert(vlist::and_list(vlist::List<> ()));
static_assert(vlist::and_list(vlist::List<true,true,true> ()));
static_assert(!(vlist::and_list(vlist::List<true,true,false> ())));
}

void or_test() {
static_assert(vlist::or_list(vlist::List<true,false,false> ()));
//base case for or, empty list is false
static_assert(!(vlist::or_list(vlist::List<> ())));
static_assert(!(vlist::or_list(vlist::List<false,false,false> ())));
}

void all_test() {
//anything holds for the empty list, as a consequence of the definition
static_assert(vlist::all(([](auto b){return b % 2 == 0;}),vlist::List<>()));
//
static_assert(vlist::all(([](auto b){return b % 2 == 0;}),vlist::List<2,4,6>()));
static_assert(vlist::all(([](auto b){return b == 1;}),vlist::List<1,1,1>()));
//character tests
static_assert(!(vlist::all(([](auto b){return b == 'a';}),vlist::List<'a','b','c'>())));
static_assert((vlist::all(([](auto b){return b == 'a';}),vlist::List<'a','a','a'>())));
}

void any_test() {
//numerical tests
static_assert(vlist::any(([](auto b){return b % 2 == 0;}),vlist::List<2,4,7>()));
static_assert(vlist::any(([](auto b){return b == 1;}),vlist::List<1,2,1>()));
//character tests
static_assert(!(vlist::any(([](auto b){return b == 'a';}),vlist::List<'b','b','c'>())));
static_assert((vlist::any(([](auto b){return b == 'a';}),vlist::List<'a','b','c'>())));
//Nothing holds for the empty list, as above.
static_assert(!(vlist::any(([](auto b){return b == 'a';}),vlist::List<>())));
}

void length_test() {
//length works for different types
static_assert(vlist::length(vlist::List<2,4,7,2,5>()) == 5);
static_assert(vlist::length(vlist::List<'a','b','c'>()) == 3);
//the length of an empty list is 0
static_assert(vlist::length(vlist::List<>()) == 0);
//a list of length 2 is not length 2
static_assert(!(vlist::length(vlist::List<1,2,1>()) == 2));
}
void sum_test() {
//The sum of an empty list is 0
static_assert((vlist::sum(vlist::List<>()) == 0));
//sum only typed for numbers
// static_assert(!(vlist::product(vlist::List<'a'>())));
static_assert(vlist::sum(vlist::List<2,4,7>()) == 13);
static_assert(vlist::sum(vlist::List<1,2,1>()) != 5);
}
void min_test() {
//empty list has no minimum, and caught
// static_assert(!(vlist::minimum(vlist::List<>()) == 2));
//The minimum of a list is the minimum, and in the list
static_assert(vlist::minimum(vlist::List<2,4,7>()) == 2);
static_assert(vlist::minimum(vlist::List<2,4,7>()) != 1);
static_assert(vlist::minimum(vlist::List<1,2,1>()) != 2);
static_assert(vlist::minimum(vlist::List<1,2,1>()) == 1);
}
void max_test() {
//empty list has no maximum
// static_assert(!(vlist::maximum(vlist::List<>()) == 2));
//obvious maximums
static_assert(vlist::maximum(vlist::List<2,4,7>()) == 7);
static_assert(vlist::maximum(vlist::List<1,2,1>()) == 2);
static_assert(vlist::maximum(vlist::List<1,2,1>()) != 3);
static_assert(vlist::maximum(vlist::List<1,2,1>()) != 1);
}
void product_test() {
//The empty list will return the multiplicative identity
static_assert((vlist::product(vlist::List<>()) == 1));

//A list of characters will not have a product
//(I don't know how to safely guard it)
// static_assert((vlist::product(vlist::List<'a'>())));

//Products act like products
static_assert(vlist::product(vlist::List<2,4,7>()) == 56);
static_assert(vlist::product(vlist::List<1,2,1>()) != 1);
}
void foldr_test() {
 auto l = vlist::List<4,8,16>();
 //Strange behaviour, happens with two definitions of foldr
 static_assert(vlist::foldr(([](auto a,auto b){return (a+b)*0.5;}), 4, l) == 6.5);
 static_assert(vlist::foldr(([](auto a,auto b){return (a+b)/2;}), 4, l) == 6);
 //normal behaviour
 static_assert(vlist::foldr(([](auto a,auto b){return (a+b);}), 4, l) == 32);
 //edge case, empty list returns only the accumulator
 static_assert(vlist::foldr(([](auto a,auto b){return (a+b)*0.5;}), 4, vlist::List<> ()) == 4);
}
void foldr1_test() {
 auto l = vlist::List<4,8,16,32>();
 static_assert(vlist::foldr1(([](auto a,auto b){return (a+b)/2;}), l) == 10);
 static_assert(vlist::foldr1(([](auto a,auto b){return (a+b);}), l) == 60);
 static_assert(vlist::foldr1(([](auto a,auto b){return (a+b);}), l) == 60);
}
void foldl_test() {
//similar casees to foldr above
 auto l = vlist::List<4,8,16,32>();
 auto f = ([](auto a,auto b){return (a+b)/2;});
 auto g = ([](auto a,auto b){return (a+b);});
 static_assert(vlist::foldl(f, 4, vlist::List<>()) == 4);
 static_assert(vlist::foldl(f, 4, l) == 21);
 static_assert(!(vlist::foldl(f, 4, l) == 20));
 static_assert(vlist::foldl(g,4, l) == 64);
 static_assert(!(vlist::foldl(g,4, l) == 32));
}

void foldl1_test() {
 auto l = vlist::List<4,4,8,16,32>();
 auto f = ([](auto a,auto b){return (a+b)/2;});
 auto g = ([](auto a,auto b){return (a+b);});
 static_assert(vlist::foldl1(f, l) == 21);
 static_assert(vlist::foldl1(g, l) == 64);
}

void replicate_test() {
//replicating 0 times yields the empty list
static_assert((vlist::replicate<0,1>()) == vlist::List<>());
//replicating other types works too
static_assert((vlist::replicate<5,1>()) == vlist::List<1,1,1,1,1>());
static_assert((vlist::replicate<3,'a'>()) == vlist::List<'a','a','a'>());
}
//does not make too long lists
static_assert(!(vlist::replicate<5,1>() == vlist::List<1,1,1,1,1,1>()));
void zip_test() {
//same length zips
using namespace vlist;
auto l = zip(List<1,2,3>(), List<2,4,6>());
static_assert((vlist::zip(vlist::List<1,2,3>(), vlist::List<2,4,6>()))
              == vlist::List<std::pair<int, int>{1, 2},
                             std::pair<int, int>{2, 4},
                             std::pair<int, int>{3, 6}> ());

static_assert((vlist::zip(vlist::List<1,2,3>(), vlist::List<2,4,6>()))
              == vlist::List<std::pair<int, int>{1, 2},
                             std::pair<int, int>{2, 4},
                             std::pair<int, int>{3, 6}> ());

static_assert((vlist::zip(vlist::List<1,2,3>(), vlist::List<'a','b','c'>()))
              == vlist::List<std::pair<int, char>{1, 'a'},
                             std::pair<int, char>{2, 'b'},
                             std::pair<int, char>{3, 'c'}> ());

//is_unityped catches different sized lists, still don't know how to escape
//a compile time error
//
// try{
// static_assert((vlist::zip(vlist::List<1,2>(), vlist::List<2,4,6>()))
//               == vlist::List<std::pair<int, int>{1, 2},
//                              std::pair<int, int>{2, 4}> ());
// }
// catch(...) {std::cout << "Catching first zip test";}

// try{
//     static_assert((vlist::zip(vlist::List<1,2,3>(), vlist::List<2,4>()))
//                   == vlist::List<std::pair<int, int>{1, 2},
//                                  std::pair<int, int>{2, 4}> ());
// }
// catch(...) {std::cout << "Catching first zip test";}
}
void iterateN_test() {
static_assert((vlist::iterateN<0,1>(([](auto b){return b+1;})))
              == vlist::List<>());
static_assert((vlist::iterateN<3,1>(([](auto b){return 2*b;})))
              == vlist::List<1,2,4>());
static_assert((vlist::iterateN<3,'a'>(([](auto b){return b;})))
              == vlist::List<'a','a','a'>());
}
void concat_test() {
   using namespace vlist;
auto s1 = List<1,2>();
auto c1 = List<'a','b'>();
// auto s2 = List<3,4>();
// auto s3 = List<5,6>();
auto l = List<s1,s1,s1> ();
auto l1 = List<c1,c1,c1> ();
//is_unityped catches this as well
// auto l1 = List<s1,s2> ();
static_assert(vlist::concat(l)
                   == vlist::List<1,2,1,2,1,2>());
static_assert(!(vlist::concat(l)
                   == vlist::List<1,2,3,4,5,6>()));
static_assert(vlist::concat(l1)
                   == vlist::List<'a','b','a','b','a','b'>());
static_assert(!(vlist::concat(l1)
                   == vlist::List<'a','a','a','a','a','a'>()));
// static_assert(vlist::concat(l1)
//                    == vlist::List<1,2,1,2,1,2>());
}


}
