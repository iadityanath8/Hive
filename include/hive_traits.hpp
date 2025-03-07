#pragma once 
#include <concepts>
#include <cstdlib>
#include "hive.hpp"

namespace hive {

template <typename T>
struct make_rvalue
{
    make_rvalue() = delete;
    static T &&instance() { return static_cast<T &&>(T{}); }
};

/** Need a little bit fixation TODO! */
template <typename T, typename = void>
struct has_repr
{
    static constexpr bool value = false;
};

template <typename T>
struct has_repr<T, decltype((void)make_rvalue<T>::instance().repr(), void())>
{
    static constexpr bool value = true;
};


/** specialized concepts soon we will try to add more traits in here normally like this in here */
class String;
using Buffer = char*;



template<typename T>
concept Printable = requires(T t){
      { make_rvalue<T>::instance().repr() } -> std::same_as<String>;
};

template <typename T>
concept RawPrintable = requires(T t) {
    { make_rvalue<T>::instance().repr() } -> std::same_as<const char*>;
};

template <typename T>
concept BufferPrinatable = requires(T t) {
    { make_rvalue<T>::instance().repr() } -> std::same_as<Buffer>;
};

template <typename T>
concept Comparable = requires(T a, T b) {
    { a > b } -> std::same_as<bool>;
    { a < b } -> std::same_as<bool>;
    { a ==b } -> std::same_as<bool>;
};


template <typename T> 
concept Iterable = requires(T t) { 
    {t.begin()} -> std::same_as<typename T::iterator>;
    {t.end()} -> std::same_as<typename T::iterator>;
};


// TODO: Implement hashable trait 
// template <typename T> 
// concept Hashable =

}