#pragma once 
#include <cstdio>
#include <cstdlib>
#include "hive.hpp"

typedef int64_t index_type;


template <typename T>
class ListIterator {
public:
    using Valuetype = typename T::ValueType;
    using reference = Valuetype&;
    using pointer = Valuetype*;

    explicit ListIterator(pointer start): m_ptr(start) { }

    ListIterator& operator++() {
        ++m_ptr;
        return *this;
    }

    ListIterator operator++(int){
        auto c = *this;
        ++m_ptr;
        return c;
    }

    reference operator*() {return *m_ptr;}
    reference operator -> () {return m_ptr;}

    bool operator != (const ListIterator<T>& other) const { return m_ptr != other.m_ptr; }
    bool operator == (const ListIterator<T>& other) const {return m_ptr == other.m_ptr; }

private:
    pointer m_ptr;
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


    T *data();
    iterator begin()
    {
        return iterator(items);
    }
    const T *data() const;
    iterator end()
    {
        return iterator(items + sz);
    }

private:
    void extend();
    T *items{};
    size_t capacity{};
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
    };
private:
    
    size_t sz {};
    Node* head;
    Node* tail;
};