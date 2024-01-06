#pragma once

#include <string>//string
#include <cstddef>//size_t
#include <type_traits>//is_same_v
#include <tuple>//make_tuple, tuple

namespace vlist
{

//Helper Function**************************************************************

consteval bool is_unityped()                   {return true;}
consteval bool is_unityped(auto)               {return true;}
consteval bool is_unityped(auto X, auto... Xs) {return (std::is_same_v<decltype(X), decltype(Xs)> && ...);}
// template (typename F, typename G)
// consteval bool is_unityped(F<G<xs...>...>)
//Main Declarations************************************************************

//List : LiteralType ty => {n : _} -> Meta (List n ty)
template<auto... Xs> requires (is_unityped(Xs...)) struct List
{
  //postpend : LiteralType ty => Meta (List n ty -> ty -> List (n + 1) ty)
  //postpend (List {xs...}) x = List {xs..., x}
  template<auto X> constexpr auto postpend() const
  requires (is_unityped(X, Xs...)) {return List<Xs..., X>();}

  //bb : LiteralType ty => Meta (List n ty -> Nat -> ty)
  template<std::size_t N>
  requires (sizeof...(Xs) > 0) && (N <= sizeof...(Xs))
  constexpr auto bb() const;
};
//*****************************************************************************

// template <auto... xs, auto... ys>
// consteval bool is_unityped(List<xs...> s, List<ys...> l)
// {return (std::is_same_v<decltype(xs), decltype(ys)> && ...);}

//prepend : LiteralType ty => Meta (ty -> List n ty -> List (n + 1) ty)
//prepend x (List {xs...}) = List {x, xs...}
template<auto X, auto... Xs> constexpr auto prepend(List<Xs...> l)
requires (is_unityped(X, Xs...)) {return List<X, Xs...>();}

//Forward Declarations*********************************************************

template<auto x, auto... xs>
consteval bool is_unityped(List<x, xs...> l) {return (std::is_same_v<decltype(x), decltype(xs)> && ...);}

  namespace app
{

template<auto X> struct prepend
{
  template<auto... Xs> constexpr auto operator () (List<Xs...> l) const
  requires (is_unityped(Xs...)) {return vlist::prepend<X>(l);}
};

template<auto X> struct postpend
{
  template<auto... Xs> constexpr auto operator () (List<Xs...> l) const
  requires (is_unityped(Xs...)) {return l.template postpend<X>();}
};

}//namespace app

//Utilities********************************************************************

//to_string********************************************************************
template<auto x>
constexpr auto last(List<x>)
{
 return x;
}

template<auto x, auto... xs>
constexpr auto last(List<x,xs...> l)
{
 return last(List<xs...>());
}

template<auto x>
constexpr auto init(List<x>)
{
  return List<>();
}
template<auto x, auto... xs>
constexpr auto init(List<x,xs...>)
{
  return prepend<x>(init(List<xs...>()));
}

// std::string to_string(std::pair<int,int> x) {
//   // std::string output;
//   // output.append();
//   // output.append(' ');
//   // output.append(std::to_string(std::get<1>(x)));
//     return std::to_string(std::get<0>(x)) + ";" + std::to_string(std::get<1>(x));
//   }

  template<auto... Xs> constexpr std::string convert_init(List<Xs...> l)
{
  if constexpr (sizeof... (Xs) > 0)
    return std::to_string(head(l)) + ", " + convert_init(tail(l));
  else
    return "";
}

template<auto... Xs> constexpr std::string convert_last(List<Xs...> l)
{
  if constexpr (sizeof... (Xs) > 0)
    return std::to_string(last(l));
  else
    return "";
}

template<auto... Xs> constexpr std::string to_string(List<Xs...> l)
{
  return "List{" + convert_init(init(l)) + convert_last(l) + "}";
}



//Operators********************************************************************

template<auto... Xs, auto... Ys> constexpr bool operator == (List<Xs...>, List<Ys...>) {return false;}
template<auto... Xs> constexpr bool operator == (List<Xs...>, List<Xs...>) {return true;}

template<auto... Xs, auto... Ys>
constexpr bool operator != (List<Xs...> lx, List<Ys...> ly) {return !(lx == ly);}

//*****************************************************************************

template<auto... Xs> constexpr auto operator >> (List<Xs...> l, auto f) {return f(l);}

template<auto... Xs, auto... Ys>
constexpr auto operator >> (std::tuple<List<Xs...>, List<Ys...>> t, auto f) {return f(t);}

//*****************************************************************************

template<auto... Xs, auto... Ys>
constexpr auto operator , (List<Xs...> xs, List<Ys...> ys) {return std::make_tuple(xs, ys);}

//*****************************************************************************

}//namespace vlist

namespace std
{

template<auto... Xs>
std::ostream& operator << (std::ostream& out, vlist::List<Xs...> l)
{
  out << to_string(l);
  return out;
}

}//namespace std
