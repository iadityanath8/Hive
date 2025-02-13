#pragma once 
#include <cstdio>
#include <cstdlib>
#include "hive.hpp"

typedef int64_t index_type;
template <typename T>
class List
{
public:
    List();
    List(int _s);
    List(std::initializer_list<T> init);
    List(const List<T> &other);
    List(List<T> &&other);

    ~List();
    /** copy assignment */
    void push(T _v);
    bool find(T _v);
    index_type indexOf(T _v);
    List<T> &operator=(const List<T> &other);
    List<T> &operator=(List<T> &&other);
    bool operator==(const List<T> &other);
    void reserve(int _s);
    index_type size(); // experimental i will intriduce a global len funtion to identify sz
    T pop();
    T &operator[](index_type index);
    const T &operator[](index_type index) const;

    // iterator
    class Iterator
    {
    private:
        T *mptr;

    public:
        Iterator(T *ptr) : mptr(ptr) {}
        T &operator*() { return *mptr; }

        Iterator &operator++()
        {
            ++mptr;
            return *this;
        }
        bool operator!=(const Iterator &other) { return mptr != other.mptr; }
    };

    T *data();
    Iterator begin()
    {
        return Iterator(items);
    }
    const T *data() const;
    Iterator end()
    {
        return Iterator(items + sz);
    }

private:
    void extend();
    T *items{};
    size_t capacity{};
    index_type sz{};
};
