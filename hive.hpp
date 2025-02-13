/***
 *
 * Hive is a minimilistic small and fast c++ replaccement to standard library like iostream and containers such as
 * STL and it uses c++ standard library with a pinch of salt very little only for the features like initializer_list
 * and move semantics etc it does not uses any kind of data structure even also it does not even uses allocation
 *
 *
 *
 *
 */
#pragma once
#include <stdio.h>
#include <initializer_list>
#include <cstring>
#include <cstdint>
#include "list.hpp"
#include "hive_traits.hpp"
#include <utility>

#define EXPERIMENTAL 0

#define INIT_LIST_CAPACITY 121

void panic(const char *msg__);

/** will introduce small string optimization (SSO) soon for small and usable strings  */
class String
{
public:
    String();
    ~String();
    String(int s_);
    String(const char *ch_);
    String(const String &other);
    String(String &&other);
    String &operator=(const String &other);
    String &operator=(String &&other);
    String &operator+=(const char *ch_);
    String &operator+=(String &&other);
    char &operator[](index_type idx);
    const char &operator[](index_type idx) const;
    void push(char ch_);
    char pop();
    bool operator==(const String &other);
    char *data();
    size_t size() { return m_data.size(); }

    List<char>::Iterator begin()
    {
        return List<char>::Iterator(m_data.data());
    }

    List<char>::Iterator end()
    {
        return List<char>::Iterator(m_data.data() + m_data.size());
    }

    char *buffer() { return m_data.data(); }

private:
    List<char> m_data{};
};
/** erroe and halt */

/** basic types printers */
void hive_print1(int __a)
{
    printf("%d", __a);
}

void hive_print1(char _ch)
{
    printf("%c", _ch);
}

void hive_print1(float __a)
{
    printf("%f", __a);
}

void hive_print1(double __a)
{
    printf("%lf", __a);
}

void hive_print1(const char *__a)
{
    printf("%s", __a);
}

void hive_print1(char *__a)
{
    printf("%s", __a);
}

void hive_print1(bool __a)
{
    if (__a)
    {
        printf("True");
    }
    else
    {
        printf("False");
    }
}

void hive_print1(size_t __a)
{
    printf("%ld", __a);
}

void panic(const char *msg__)
{
    fprintf(stderr, "PANIC: %s\n", msg__);
    abort();
}
/** lots of bug fixing in here ok  */
void hive_print1(String &v)
{
    hive_print1(v.data());
}

// soon we will try to introduce the concepts so that we can use the specially compile time messages in here
template <Printable T>
void hive_print1(const T &__v)
{

    hive_print1(__v.repr());
}

template <RawPrintable T>
void hive_print1(const T &__v)
{
    printf("%s", __v.repr());
}

/** hive format funtion use specifiacally just like f string of python but a little lower level */

/** usable string printer basic printing for the types in here which is compatible for all funcitons compatible with repr function declared in a class
 * this implementation of IO strictly does not need implementing an daunting << operator only needs an repr function that needs to be checked by the hive
 * runtime.
 */
template <typename... Args>
void print(Args&&...args)
{
    ((hive_print1(args), printf(" ")), ...);
    printf("\n");
}

/*** String library in C++
 *
 * Name seems to be javaish but as i am going to also make String_view so i think StringBuilder Can be used but
 * it can change anytime
 */

/**
 *
 * Mostly String converter are here
 */

String to_string(const char *str)
{
    return String(str);
}

String to_string(char ch_)
{
    String a;
    a.push(ch_); // i know pretty inefficient in here but ok
    return a;
}

String to_string(int ch_)
{
    char buffer[INIT_LIST_CAPACITY - 92];
    snprintf(buffer, sizeof(buffer), "%d", ch_);
    return String(buffer);
}

String to_string(double _d)
{
    char buffer[INIT_LIST_CAPACITY - 92];
    snprintf(buffer, sizeof(buffer), "%.6f", _d);
    return String(buffer);
}

String to_string(unsigned int _d)
{
    char buffer[INIT_LIST_CAPACITY - 92];
    snprintf(buffer, sizeof(buffer), "%u", _d);
    return String(buffer);
}

String to_string(unsigned long long _d)
{
    char buffer[INIT_LIST_CAPACITY - 92];
    snprintf(buffer, sizeof(buffer), "%llu", _d);
    return String(buffer);
}

String to_string(long long _d)
{
    char buffer[INIT_LIST_CAPACITY - 92];
    snprintf(buffer, sizeof(buffer), "%lld", _d);
    return String(buffer);
}

String to_string(int64_t _d)
{
    return to_string(static_cast<long long>(_d));
}

String to_string(size_t _d)
{
    char buffer[INIT_LIST_CAPACITY - 92];
    snprintf(buffer, sizeof(buffer), "%zu", _d);
    return String(buffer);
}

template <typename... Args>
String format(Args... args)
{
    String fa[] = {to_string(args)...}; // very very unsafe bro

    String result;
    for (auto s : fa)
    {
        result += s.buffer(); // fuck very inefficient shit
    }
    return result;
}

template <typename T>
List<T>::List() : sz(0), capacity(0), items(nullptr)
{
}

template <typename T>
List<T>::List(std::initializer_list<T> init)
{
    sz = 0;
    capacity = init.size();
    items = (T *)calloc(capacity, sizeof(T));

    if (items == nullptr)
        panic("failed to allocate the memory");

    for (auto v : init)
    {
        items[sz] = v;
        sz++;
    }
}

template <typename T>
void List<T>::extend()
{

    int new_capacity = capacity * 2;
    T *new_items = (T *)realloc(items, new_capacity * sizeof(T)); // what the hell ??
    if (new_items == nullptr)
        panic("Failed to allocate the memory");
    capacity = new_capacity;
    items = new_items;
}

template <typename T>
List<T>::List(int _s)
{
    sz = 0;
    capacity = _s;
    items = (T *)calloc(_s, sizeof(T));
    if (items == nullptr)
        panic("Failed to allocate the memory");
}

template <typename T>
List<T>::List(const List<T> &other)
{
    capacity = other.capacity;
    sz = other.sz;
    items = (T *)calloc(capacity, sizeof(T));
    for (index_type i = 0; i < sz; ++i)
    {
        items[i] = other.items[i];
    }
}

template <typename T>
List<T>::List(List<T> &&other)
{
    items = other.items;
    sz = other.sz;
    capacity = other.capacity;

    other.items = nullptr;
    other.sz = 0;
    other.capacity = 0;
}

template <typename T>
List<T>::~List()
{

    for (index_type i = 0; i < sz; i++)
    {
        items[i].~T();
    }

    free(items);
    sz = 0;
    capacity = 0;
    items = nullptr;
}

template <typename T>
T *List<T>::data()
{
    return items;
}

template <typename T>
const T *List<T>::data() const
{
    return items;
}

template <typename T>
void List<T>::push(T _v)
{
    if (capacity == 0)
    {
        capacity = INIT_LIST_CAPACITY;
        sz = 0;
        items = (T *)calloc(capacity, sizeof(T));
    }
    else if (sz == capacity)
    {
        extend();
    }
    items[sz] = _v;
    sz++;
}

template <typename T>
bool List<T>::operator==(const List<T> &other)
{
    if (sz != other.sz)
        return false;
    for (int i = 0; i < sz; i++)
    {
        if (!(items[i] == other.items[i]))
            return false;
    }
    return true;
}

template <typename T>
index_type List<T>::indexOf(T _v)
{
    for (int i = 0; i < sz; i++)
    {
        if (items[i] == _v)
        {
            return i;
        }
    }
    return -1;
}

template <typename T>
void List<T>::reserve(int _s)
{
    if (_s <= capacity)
        return;

    T *new_items = (T *)calloc(_s, sizeof(T));
    if (!new_items)
        panic("Memory allocation failed");

    for (index_type i = 0; i < sz; i++)
    {
        new_items[i] = items[i];
    }

    for (int i = 0; i < sz; ++i)
    {
        items[i].~T(); // Call destructor
    }
    free(items);
    items = nullptr;
    items = new_items;
    capacity = _s;
}

template <typename T>
index_type List<T>::size()
{
    return sz;
}

template <typename T>
T List<T>::pop()
{
    if (sz == 0)
    {
        panic("can not pop from an empty List");
    }
    auto temp = items[sz];
    sz--;
    return temp;
}

template <typename T>
T &List<T>::operator[](index_type index)
{
    // support for negative index
    if (index >= sz or (-index) >= (sz + 1))
    {
        panic("index out of bounds");
    }
    if (index < 0)
    {
        return items[sz + index];
    }
    return items[index];
}

template <typename T>
const T &List<T>::operator[](index_type index) const
{
    /**
     *  0
     * {1,2,3}
     * index = -1  -2 -3 */
    if (index >= sz or (-index) > (sz + 1))
    {
        panic("index out of bounds");
    }
    if (index < 0)
    {
        return items[sz + index];
    }
    return items[index];
}

template <typename T>
List<T> &List<T>::operator=(const List<T> &other)
{
    if (this == &other)
    {
        return *this; // Handle self-assignment
    }

    for (index_type i = 0; i < sz; ++i)
    {
        items[i].~T();
    }

    free(items); // Free old memory

    capacity = other.capacity;
    sz = other.sz;
    items = (T *)calloc(capacity, sizeof(T));
    for (index_type i = 0; i < sz; ++i)
    {
        items[i] = other.items[i];
    }

    return *this;
}

template <typename T>
List<T> &List<T>::operator=(List<T> &&other)
{

    if (this == &other)
    {
        return *this;
    }

    for (index_type i = 0; i < sz; i++)
    {
        items[i].~T();
    }

    free(items);
    capacity = other.capacity;
    sz = other.sz;
    items = other.items;

    other.items = nullptr;
    other.capacity = 0;
    other.sz = 0;
    return *this;
}

/** String implementation */
String::String() : m_data() {}

String::String(int _s)
{
    m_data.reserve(_s);
}

String::String(const char *ch_)
{
    size_t len = strlen(ch_);

    m_data.reserve(len + 1);
    for (size_t i = 0; i < len; i++)
    {
        m_data.push(ch_[i]);
    }
}

String::String(const String &other)
{
    m_data = other.m_data;
}

String::String(String &&other)
{
    m_data = std::move(other.m_data);
}

String::~String()
{
    m_data.~List();
}

void String::push(char ch_)
{
    m_data.push(ch_);
}

char String::pop()
{
    if (m_data.size() == 0)
    {
        panic("Cannot pop out from the String");
    }
    auto t = m_data.pop();
    return t;
}

char *String::data()
{
    return m_data.data();
}

bool String::operator==(const String &other)
{
    return m_data == other.m_data;
}

char &String::operator[](index_type index)
{
    return m_data[index];
}

const char &String::operator[](index_type index) const
{
    return m_data[index];
}

String &String::operator=(String &&other)
{
    if (this == &other)
        return *this;
    m_data = std::move(other.m_data);
    return *this;
}

String &String::operator=(const String &other)
{
    if (this == &other)
        return *this;
    m_data = other.m_data;
    return *this;
}

String &String::operator+=(const char *ch_)
{
    index_type len = strlen(ch_);

    for (index_type i = 0; i < len; i++)
    {
        push(ch_[i]);
    }
    return *this;
}

template <typename T, typename U>
struct Pair
{
    T first;
    U second;
    Pair(T &&a, U &&b) : first(a), second(b) {}

    String repr() const
    {
        return format("a");
    }
};
