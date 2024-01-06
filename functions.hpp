#pragma once
#include<iostream>
#include "VListSU.hpp"
//functions.hpp
//Includes all functions required, ordered by higher order functions

//defining a check for a "Num" typeclass,
//Might want to add all of the std:: number types, but there's a lot of them
                                   // || std::is_same_v<std::int_32,decltype(x)>
                                   // || std::is_same_v<std::int_64,decltype(x)>
                                   // || std::is_same_v<float,decltype(x)>
                                   // || std::is_same_v<float,decltype(x)>
                                   // || std::is_same_v<float,decltype(x)>
consteval bool is_num() {return true;}
consteval bool is_num(auto x) {return std::is_same_v<int,decltype(x)>
                                   || std::is_same_v<float,decltype(x)>;}
consteval bool is_num(auto... xs) {return (std::is_same_v<int,decltype(xs)> && ...)
                                   ||     (std::is_same_v<float,decltype(xs)> && ...);}


namespace vlist {


//prepend and postpend
template <auto X> constexpr auto prepend(List<> l) {return List<X>();}
template<auto X, auto... Xs>
constexpr auto prepend(List<Xs...> l) {return List<X, Xs...>();}

template<auto X>
constexpr auto postpend(List<> l) {return List<X>();}
template<auto X,auto... Xs>
constexpr auto postpend(List<Xs...> l) {return List<Xs..., X>();}

//head and tail
constexpr auto head(List<> l) {return l;};
template<auto x, auto... xs>
constexpr auto head(List<x,xs...> l) {return x;};

template<auto X, auto... Xs>
constexpr auto tail(List<X,Xs...> l) {return List<Xs...>();};

// Pattern-matching approach
template<auto... Xs> constexpr auto null(List<Xs...> l) {return false;}
constexpr auto null(List<> l) {return true;}

//Append : append the list Ys to Xs

// template<auto... Ys> constexpr auto append(List<>, List<Ys...>)
//   {return List<Ys...>();}
// template<auto... Xs, auto... Ys> constexpr auto append(List<Xs...> l, List<Ys...> s)
//   {return List<Xs...,Ys...>();}

template<auto... Ys> constexpr auto append(List<> l, List<Ys...> s)
  {return s;}
template<auto x, auto... Xs, auto... Ys> constexpr auto append(List<x,Xs...> l, List<Ys...> s)
  {return prepend<x>(append(List<Xs...>(), s));}

template<auto... xs>
constexpr auto map(List<xs...> l, auto f)
requires (std::is_invocable_v<decltype(f), decltype(xs)> && ...)
{
  return List<f(xs)...>();
}

// is_invocable_v<decltype(f), decltype(Xs)>, check if f can be invoked on Xs
// the types of Xs

//Filter
constexpr auto filter(auto f, List <> l) {return l;}
template<auto x, auto... xs>
constexpr auto filter(auto f, List<x, xs...> l)
requires (std::is_invocable_r_v<bool, decltype(f), decltype(x)>) &&
         (std::is_invocable_r_v<bool, decltype(f), decltype(xs)> && ...)
{
if constexpr (f(x))
   {return prepend<x>(filter(f, tail(l)));}
else return filter(f, tail(l));
}

// requires std::is_invocable_r_v<bool, decltype(p), decltype(x)>
// - requires bool to be the return type of p invoked on x

template<std::size_t n, auto... xs>
  requires (n <= (sizeof...(xs)))
constexpr auto take(List<xs...> l)
{if constexpr (n==0) return List<>();
 else return prepend<head(l)>(take<n-1>(l));}

template<std::size_t n, auto x, auto... xs>
  requires (n < (sizeof...(xs)))
constexpr auto drop(List<x, xs...> l)
{if constexpr (n==0)
  {return List<>();}
  else return drop<n-1>(List<xs...>());}

template <std::size_t n, auto... xs>
  requires (n < sizeof...(xs))
constexpr auto splitAt (List<xs...> l)
{ return std::make_tuple(take<n>(l), drop<n>(l));}


constexpr auto drop_while(List<> l, auto f) {return l;}
template<auto x, auto... xs>
constexpr auto drop_while(List<x, xs...> l, auto f)
  requires (std::is_invocable_r_v<bool,decltype(f), decltype(x)>)
        && (std::is_invocable_r_v<bool,decltype(f), decltype(xs)> && ...)
{
  if constexpr (!f (x)) {return l;}
//neat bug, take away the else. To fix bug, add else.
  else return drop_while(List<xs...> (), f);
}

constexpr auto take_while(List<> l, auto f) {return l;}
template<auto x, auto... xs>
constexpr auto take_while(List<x, xs...> l, auto f)
  requires (std::is_invocable_r_v<bool,decltype(f), decltype(x)>)
        && (std::is_invocable_r_v<bool,decltype(f), decltype(xs)> && ...)
{
  if constexpr (!f (x)) {return List<>();}
  else return prepend<x>(take_while(List<xs...> (), f));
}

constexpr auto span(List<> l, auto f)
{
    return l;
}
template<auto x, auto... xs>
constexpr auto span(List<x, xs...> l, auto f)
  requires (std::is_invocable_r_v<bool,decltype(f), decltype(x)>)
        && (std::is_invocable_r_v<bool,decltype(f), decltype(xs)> && ...)
{
  return std::make_tuple(take_while(l, f), drop_while(l, f));
}

//replicates a single element
template<auto n, auto x>
constexpr auto replicate()
    requires (std::is_same_v<int,decltype(n)>)
{
    if constexpr (n > 0)
       {return prepend<x>(replicate<n-1,x>());}
    else return List<>();
}
//iterateN, iterate a function f on an element x, n times.
template<auto n, auto x>
constexpr auto iterateN(auto f)
  requires (std::is_invocable_r_v<decltype(x), decltype(f), decltype(x)>)
        && (std::is_same_v<decltype(x), decltype(f(x))>)
        && (std::is_same_v<int, decltype(n)>)
{
    if constexpr (n > 0)
    {return prepend<x>(iterateN<n-1,f(x)>(f));}
    else return List<>();
}

//      Zip, defined using pattern matching and make_pair.

constexpr auto zip(List<> l, List<> s) {
    return l;
    }
template<auto x, auto... xs>
constexpr auto zip(List<x, xs...> l, List<> s)
{
    return s;
}
template<auto y, auto... ys>
constexpr auto zip(List<> l, List<y, ys...> s)
{
    return l;
}
template<auto... xs, auto... ys>
constexpr auto zip(List<xs...> l, List<ys...> s)
{
    return append(List<std::make_pair(head(l),head(s))>(),zip(tail(l),tail(s)));
}

//      Folds, all defined based on the notes using case splits

//is_invocable_r_v is used variadically to encode the signatures of the
//accumulators.
constexpr auto foldr (auto f, auto y, List<> l)
{
    return y;
}
template<auto x, auto... xs>
constexpr auto foldr (auto f, auto y ,List<x, xs...> l)
    requires (std::is_invocable_r_v<decltype(y), decltype(f), decltype(x), decltype(y)>)
{
    return f(x, foldr(f, y, List<xs...> ()));
}
//alternative foldr definition
// constexpr auto foldr (auto f, auto y, List<> l)
// {
//     return y;
// }
// template<auto... xs>
// constexpr auto foldr (auto f, auto y, List<xs...> l)
//     // requires (std::is_invocable_r_v<decltype(y), decltype(f), decltype(last(l)), decltype(y)>)
// {
//     return foldr(f, f(last(l),y), init(l));
// }

template<auto x>
constexpr auto foldr1 (auto f, List<x> l)
{
    return x;
}
template<auto x, auto... xs>
constexpr auto foldr1 (auto f, List<x, xs...> l)
    requires (std::is_invocable_r_v<decltype(x), decltype(f), decltype(x), decltype(x)>)
{
    return f(x, foldr1(f, List<xs...> ()));
}

constexpr auto foldl (auto f, auto y, List<> l)
{
    return y;
}
template<auto x, auto... xs>
constexpr auto foldl (auto f, auto y,List<x, xs...> l)
    requires (std::is_invocable_r_v<decltype(y), decltype(f), decltype(y), decltype(x)>)
          && (is_unityped(x))
{
    return foldl(f, f(y,x), List<xs...> ());
}

template<auto x>
constexpr auto foldl1 (auto f, List<x> l)
{
    return x;
}
template<auto x, auto... xs>
constexpr auto foldl1 (auto f, List<x, xs...> l)
    // requires (std::is_invocable_r_v<decltype(y), decltype(f), decltype(y), decltype(x)>)
{
    return foldl(f, x, List<xs...> ());
}

//Concat,
//not definable using a fold
//not callable due to unityping

constexpr auto concat(List<> l)
{
    return l;
}

template<typename T, T... ys, List<ys...> x, List... xs>
constexpr auto concat(List<x,xs...> l)
{
    return append(x, concat(List<xs...> ()));
}

//      Fold-based, booleans on lists

//or and and, defined as foldrs with converse accumulators
template<auto... xs>
constexpr auto or_list(List<xs...> l )
requires (std::is_same_v<bool, decltype(xs)> && ...)
{
   return foldr(([](auto a, auto b){return a || b;}), false, l);
}

template<auto... xs>
constexpr auto and_list(List<xs...> l )
{
   return foldr(([](auto a, auto b){return a && b;}), true, l);
}

//all and any, definitions based on map and and/or_list respectively,
//with requirements asserting boolean valued functions

template<auto... xs>
constexpr auto all(auto f, List<xs...> l )
requires (std::is_invocable_r_v<bool, decltype(f), decltype(xs) > && ...)
{
   return and_list(map(l,f));
}

template<auto... xs>
constexpr auto any(auto f, List<xs...> l )
requires (std::is_invocable_r_v<bool, decltype(f), decltype(xs)> && ...)
{
   return or_list(map(l,f));
}

template<auto y, auto... xs>
constexpr auto elem(List<xs...> l ){
   auto f = [](auto b){return y == b;};
   return any(f, l);
}

constexpr auto length(List<> l)
{
  return 0;
}

template<auto... xs>
constexpr auto length(List<xs...> l){
  return sizeof...(xs);
}

//Fold-based, numbers

//Sum and Product, both defined using a right fold and using the
//test for being a "Num" (integer or float, the standard library defines
//far too many for me to include any more).
template<auto... xs>
constexpr auto sum(List<xs...> l )
    requires (is_num(xs...))
     // &&      (sizeof...(xs) > 0)
{
   return foldr(([](auto a, auto b){return a + b;}), 0, l);
}

template<auto... xs>
constexpr auto product(List<xs...> l )
    requires (is_num(xs...))
    // &&       (sizeof...(xs) > 0)

{
   return foldr(([](auto a, auto b){return a * b;}), 1, l);
}
//minimum and maximum
//size requirements added to comply with
//the use of foldr1
template<auto... xs>
constexpr auto minimum(List<xs...> l )
    requires (sizeof...(xs) > 0)
{
   return foldr1(([](auto a, auto b){return std::min(a,b);}), l);
}

template<auto... xs>
constexpr auto maximum(List<xs...> l )
    requires (sizeof...(xs) > 0)
{
   return foldr1(([](auto a, auto b){return std::max(a,b);}), l);
}
}


