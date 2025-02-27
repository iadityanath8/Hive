#pragma once 

#include <cstdio>
#include <cstdlib>
#include "hive.hpp"
#include <utility>

typedef int64_t index_type;

template <typename T>
class ListIterator {
public:
    using ValueType = typename T::ValueType;
    using reference = ValueType&;
    using pointer = ValueType*;

    explicit __attribute__((always_inline)) inline ListIterator(pointer __restrict__ start) 
        : m_ptr(start) { }

    __attribute__((always_inline)) inline ListIterator& operator++() {
        ++m_ptr;
        return *this;
    }

    __attribute__((always_inline)) inline ListIterator operator++(int) {
        ListIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    __attribute__((always_inline)) inline reference operator*() const { return *m_ptr; }
    
    __attribute__((always_inline)) inline pointer operator->() const { return m_ptr; }

    __attribute__((always_inline)) inline bool operator!=(const ListIterator<T>& other) const { 
        return m_ptr != other.m_ptr; 
    }

    __attribute__((always_inline)) inline bool operator==(const ListIterator<T>& other) const { 
        return m_ptr == other.m_ptr; 
    }

private:
    pointer __restrict__ m_ptr;
};


template <typename T>
class List
{
public:

    using ValueType = T;
    using iterator = ListIterator<List<T>>;

    List();
    List(int _s);
    List(std::initializer_list<T> init);
    List(const List<T> &other);
    List(List<T> &&other);

    ~List();
    /** copy assignment */
    inline void push(T _v);
    bool find(T _v);
    __attribute__((always_inline)) inline index_type indexOf(T _v);
    __attribute__((always_inline)) inline List<T> &operator=(const List<T> &other);
    List<T> &operator=(List<T> &&other);
    bool operator==(const List<T> &other);
    void reserve(int _s);
    
    __attribute__((always_inline)) inline constexpr index_type size(); // experimental i will intriduce a global len funtion to identify sz
    __attribute__((always_inline)) inline constexpr T pop();
    __attribute__((always_inline)) inline T &operator[](index_type index);
    __attribute__((always_inline)) inline const T &operator[](index_type index) const;


    __attribute__((always_inline)) inline T *data();

    __attribute__((always_inline)) inline const T *data() const;
    
    __attribute__((always_inline)) inline iterator begin()
    {
        return iterator(items);
    }
    __attribute__((always_inline)) inline iterator end()
    {
        return iterator(items + sz);
    }

private:
    size_t capacity{};
    inline void extend(size_t s = 0);
    T *items{};
    index_type sz{};
};


template <typename T>
class LinkedListIterator {
public:
    using ValueType = typename T::ValueType;
    using reference = ValueType&;
    using pointer   = ValueType*;
    using NodeType = typename T::Node;

    explicit LinkedListIterator(NodeType* node): m_node(node) {}

    LinkedListIterator& operator++(){
        if (m_node) m_node = m_node->next;
        return *this;
    }

    LinkedListIterator& operator++(int) {
        auto c = *this;
        if (m_node) {m_node = m_node->next;}
        return c;
    }

    reference operator*() {
        return m_node->data;
    }

    pointer operator->() {
        return &(m_node->data);
    }

    bool operator!=(const LinkedListIterator& other) const { return m_node != other.m_node; }
    bool operator ==(const LinkedListIterator& other) const {return m_node == other.m_node; }

private:
    NodeType* m_node;
};

template <typename T>
class LinkedList {
public:
    using ValueType = T;
    using iterator = LinkedListIterator<LinkedList<T>>;

    LinkedList ();
    LinkedList(std::initializer_list<T> init);
    ~LinkedList();

    LinkedList(const LinkedList& other);
    LinkedList(LinkedList&& other);
    LinkedList& operator=(LinkedList&& other);

    LinkedList& operator=(const LinkedList& other); 
    auto push(T item) -> void;
    auto pop() -> T;
    auto insert(T item,size_t pos) -> void;
    auto remove(size_t pos) -> void;
    inline auto back() -> T;
    inline auto front() -> T;
    inline auto size()-> size_t {return sz;}
    inline auto empty() -> bool;

    iterator begin() {
        return iterator(head);
    }

    iterator end() {
        return iterator(nullptr);
    }

    /** debuggable */
    auto print__();

    struct Node {
        T data;
        Node* prev;
        Node* next;
        Node(T _d):data(_d),prev(nullptr),next(nullptr) { }
        Node(T _d, Node* prev_,Node* next_):data(_d),prev(prev_), next(next_) {}
    };
private:
    
    size_t sz {};
    Node* head;
    Node* tail;     
    Node* allocate_node(const T& value, Node* prev, Node* next);
    void clear();
};


/**** Static Containers */

template <typename T> 
class StatIterator {
public:
    using ValueType = typename T::ValueType;
    using reference = ValueType&;
    using pointer = ValueType*;

    reference operator*() {
        return *curr;
    }

    pointer operator->() {
        return curr;
    }

    StatIterator& operator++() {
        curr++;
        return *this;
    }

    StatIterator operator++(int) {
        auto t = *this;
        curr++;
        return t;
    }

    bool operator==(const StatIterator& other) const {
        return other.curr == curr;
    }

    bool operator!=(const StatIterator& other) const {
        return other.curr != curr;
    }

    explicit StatIterator(pointer a): curr(a) {}

private:
    pointer curr;
};


template<typename T, int N> 
class StatArray {   // considering a fking name change later but as of now its just an implementation num 
public:
    using ValueType =  T;
    using referece  =  ValueType&;
    using pointer   =  ValueType*;
    using iterator  =  StatIterator<StatArray<T,N>>;
    
    inline auto max_size() -> size_t;
    inline auto back() -> T;
    inline auto front() -> T;
    
    T& operator[](index_type index); 
    const T& operator[](index_type index) const; 

    iterator begin() {
        return iterator(M_arr);
    }

    iterator end() {
        return iterator(M_arr + M_size);
    }

private:
    T M_arr[N] {};
    size_t M_size {N};
};



/* ------------------------------------------- Iterator Tools i am thinking to make an another file for ir as i build more itertools ----------------------------------*/




/***** TODO: Try to implement a modern and powerfull iterator just like rust please rust like using templates and vrey powerfull and fgaster then traditional 1990s iterator ok ok ok  */