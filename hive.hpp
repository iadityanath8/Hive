/***
 *
 * Hive is a minimilistic small and fast c++ replaccement to standard library like iostream and containers such as
 * STL and it uses c++ standard library with a pinch of salt very little only for the features like initializer_list
 * and move semantics etc it does not uses any kind of data structure even also it does not even uses allocation of the standard
 * library as it uses some c++ standard library such as the type_traits and new although they can also be implemented by hive
 *
 *
 * It has some python like features like python like print and repr function and rust like format function although not
 * a macro but a function so
 *
 *
 *
 *
 *
 */

#pragma once
#include <cstdio>
#include <initializer_list>
#include <cstring>
#include <cstdint>
#include "list.hpp"
#include "hive_traits.hpp"
#include <utility>
#include <new>
#include <type_traits>

#define EXPERIMENTAL 1

#define INIT_LIST_CAPACITY 121
#define INIT_VEC_SIZE 2

#define CXX_GMCR_HIVE /// we'll see about naming convention and we will change the name in here  

[[noreturn]] inline void panic(const char *msg__);

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
    char &operator[](index_type idx);
    const char &operator[](index_type idx) const;
    void push(char ch_);
    char pop();
    bool operator==(const String &other);
    char *data();
    size_t size()
    {
        if (is_sso)
        {
            return sso_index;
        }
        else
        {
            return m_data.size();
        }
    }

    List<char>::iterator begin()
    {
        if (is_sso)
        {
            return List<char>::iterator(sso_buffer);
        }
        return List<char>::iterator(m_data.data());
    }

    List<char>::iterator end()
    {
        if (is_sso)
        {
            return List<char>::iterator(sso_buffer + sso_index);
        }
        return List<char>::iterator(m_data.data() + m_data.size());
    }

private:
    static constexpr size_t SSO_SIZE = 23;
    char sso_buffer[SSO_SIZE + 1] = {0};
    List<char> m_data{};
    bool is_sso{true};
    index_type sso_index;
};
/** erroe and halt */

/** basic types printers */
void hive_print1(int __a)
{
    printf("%d", __a);
}

void hive_print1(unsigned long long __a) {
    printf("%lld", __a);
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

void hive_print1(int64_t __a)
{
    printf("%lld", __a);
}

[[noreturn]] inline void panic(const char *msg__)
{
    fprintf(stderr, "PANIC: %s\n", msg__);
    abort();
}
/** lots of bug fixing in here ok  */
void print_up_to(const char *str, int num_chars)
{
    printf("%.*s", (int)num_chars, str);
}

void hive_print1(String &v)
{
    // hive_print1(v.data());
    print_up_to(v.data(), v.size());
}

// soon we will try to introduce the concepts so that we can use the specially compile time messages in here
template <Printable T>
void hive_print1(const T &__v)
{
    auto b = __v.repr();
    hive_print1(b);
}

// template <RawPrintable T>
// void hive_print1(const T &__v)
// {
//     printf("%s", __v.repr());
// }

/** hive format funtion use specifiacally just like f string of python but a little lower level */

/** usable string printer basic printing for the types in here which is compatible for all funcitons compatible with repr function declared in a class
 * this implementation of IO strictly does not need implementing an daunting << operator only needs an repr function that needs to be checked by the hive
 * runtime.
 */
template <typename... Args>
void print(Args &&...args)
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

// String to_string(int64_t _d)
// {
//     return to_string(static_cast<long long>(_d));
// }

String to_string(size_t _d)
{
    char buffer[INIT_LIST_CAPACITY - 92];
    snprintf(buffer, sizeof(buffer), "%zu", _d);
    return String(buffer);
}

template <typename... Args>
String format(Args... args)
{
    String result;
    ((result += to_string(args).data()), ...); // Fold expression for efficient concatenation
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
inline void List<T>::extend(size_t s) // request for being inline 
{
    if (s == 0)
        s = capacity * 2;

    int new_capacity = s;
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
    items = (T *)malloc(_s*sizeof(T));
    if (items == nullptr)
        panic("Failed to allocate the memory");
}

template <typename T>
List<T>::List(const List<T> &other)
{
    capacity = other.capacity;
    sz = other.sz;
    items = static_cast<T*>(malloc(capacity * sizeof(T)));

    if constexpr (std::is_trivially_copyable_v<T>) {
        memcpy(items, other.items, sz * sizeof(T)); // Fastest path
    } else {
        for (index_type i = 0; i < sz; ++i) {
            items[i] = other.items[i];
        }
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
    if constexpr (!std::is_trivially_destructible_v<T>)
    {
        for (index_type i = 0; i < sz; i++)
        {
            items[i].~T();
        }
    }
    free(items);
    sz = 0;
    capacity = 0;
    items = nullptr;
}

template <typename T>
inline T *List<T>::data()
{
    return items;
}

template <typename T>
inline const T *List<T>::data() const
{
    return items;
}

template <typename T>
inline void List<T>::push(T _v)
{
    if (capacity == 0)
    {
        capacity = INIT_VEC_SIZE;
        items = static_cast<T*>(malloc(sizeof(T) * capacity));
    }
    else if (sz == capacity)
    {
        extend();
    }
    items[sz++] = _v;
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
inline index_type List<T>::indexOf(T _v)
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
    if (_s+1 <= capacity)  // TODO: LOOK here
        return;

    T *new_items = (T *)malloc(sizeof(T) * _s);

    for (index_type i = 0; i < sz; i++)
    {
        new_items[i] = items[i];
    }

    if constexpr (!std::is_trivially_destructible_v<T>)
    {
        for (int i = 0; i < sz; ++i)
        {
            items[i].~T(); // Call destructor
        }
    }

    free(items);
    items = nullptr;
    items = new_items;
    capacity = _s;
}

template <typename T>
__attribute__((always_inline)) inline constexpr index_type List<T>::size()
{
    return sz;
}

template <typename T>
__attribute__((always_inline)) inline constexpr  T List<T>::pop()
{
    if (sz != 0)
    {
        auto temp = items[sz];
        sz--;
        return temp;
    }
    return T{};
}

template <typename T>
__attribute__((always_inline))inline T &List<T>::operator[](index_type index)
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
__attribute__((always_inline)) inline const T &List<T>::operator[](index_type index) const
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
        return *this;

    if (capacity < other.sz) {
        extend(other.sz);
    }
    sz = other.sz;

    if constexpr (std::is_trivially_copyable_v<T>) {
        memcpy(items, other.items, sz * sizeof(T));
    }else {
        for (index_type i = 0;i < sz;i++) {
            items[i] = other.items[i];
        }
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

    if constexpr (!std::is_trivially_destructible_v<T>)
    {
        for (index_type i = 0; i < sz; i++)
        {
            items[i].~T();
        }
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

/** -------------------------------------------------------------------------------------String implementation ----------------------------------------------------------------------------------------------*/
String::String() : m_data(), is_sso(true), sso_index(0) {}

String::String(int _s)
{
    if (_s >= SSO_SIZE)
    {
        m_data.reserve(_s);
        is_sso = false;
    }
}

String::String(const char *ch_) : sso_index(0), is_sso(true)
{
    size_t len = strlen(ch_);
    if (is_sso and len < SSO_SIZE)
    {
        strncpy(sso_buffer, ch_, len);
        sso_index = len;
        sso_buffer[len] = '\0';
    }
    else
    {
        is_sso = false;
        m_data.reserve(len + 1);
        for (size_t i = 0; i < len; i++)
        {
            m_data.push(ch_[i]);
        }
    }
}

String::String(const String &other)
{
    if (is_sso and other.is_sso)
    {
        strncpy(sso_buffer, other.sso_buffer, SSO_SIZE + 1);
        sso_index = other.sso_index;
    }
    else
    {
        m_data = other.m_data;
    }
}

String::String(String &&other) noexcept
{
    // check SSO
    if (other.is_sso)
    {
        strncpy(sso_buffer, other.sso_buffer, SSO_SIZE + 1);
        sso_index = other.sso_index;
        other.sso_index = 0;
        other.sso_buffer[0] = '\0';
        other.is_sso = true;
    }
    else
    {
        m_data = std::move(other.m_data);
    }
}

String::~String() {
    // if (is_sso) {
    //     printf("Destroying SSO string: %s\n", sso_buffer);
    // } else {
    //     printf("Destroying heap string: %s\n", m_data.data());
    //     // m_data.~List();
    // }
}


void String::push(char ch_)
{
    if (is_sso and size() + 1 >= SSO_SIZE)
    {
        for (int i = 0; i < sso_index; i++)
        {
            m_data.push(sso_buffer[i]);
        }
        m_data.push(ch_);
        is_sso = false;
    }
    else if (is_sso)
    {
        sso_buffer[sso_index] = ch_;
        sso_index++;
        sso_buffer[sso_index] = '\0';
    }
    else
    {
        m_data.push(ch_);
    }
}

char String::pop()
{
    if (m_data.size() == 0 or sso_index == 0)
    {
        panic("Cannot pop out from the String");
    }
    if (is_sso)
    {
        auto t = sso_buffer[sso_index];
        sso_index--;
        return t;
    }
    auto t = m_data.pop();
    return t;
}

char *String::data()
{
    if (is_sso)
        return sso_buffer;
    return m_data.data();
}

bool String::operator==(const String &other)
{
    if (sso_index != other.sso_index)
        return false;
    if (is_sso)
        return strncmp(sso_buffer, other.sso_buffer, sso_index) == 0;
    return m_data == other.m_data;
}

char &String::operator[](index_type index)
{
    if (is_sso)
    {
        auto cmp = (index < static_cast<index_type>(SSO_SIZE) and (-index + 1) < sso_index + 1); // TODO: check please if false
        if (!cmp)
        {
            panic("Index out of bound for the String");
        }
        index_type in = index < 0 ? sso_index + index : index;
        return sso_buffer[in];
    }
    return m_data[index];
}

const char &String::operator[](index_type index) const
{
    if (is_sso)
    {
        auto cmp = (index < static_cast<index_type>(SSO_SIZE) and (-index + 1) < sso_index + 1); // TODO: check please of false
        if (!cmp)
        {
            panic("Index out of bound for the String");
        }
        auto in = index < 0 ? sso_index + index : index;
        return sso_buffer[in];
    }
    return m_data[index];
}

String &String::operator=(String &&other)
{
    if (this == &other)
        return *this;
    if (is_sso and other.is_sso)
    {
        strncpy(sso_buffer, other.sso_buffer, SSO_SIZE + 1);
        sso_index = other.sso_index;
        other.sso_index = 0;
        other.sso_buffer[0] = '\0';
        other.is_sso = true;
        return *this;
    }
    if (is_sso)
    {
        m_data = std::move(other.m_data);
        is_sso = false;
    }
    else
    {
        m_data = std::move(other.m_data);
    }
    other.is_sso = true;
    other.sso_buffer[0] = '\0';
    other.sso_index = 0;
    return *this;
}

String &String::operator=(const String &other)
{
    if (this == &other)
        return *this;
    if (is_sso && other.is_sso)
    {
        strncpy(sso_buffer, other.sso_buffer, SSO_SIZE); // wwwwooooo
        sso_index = other.sso_index;
    }
    else if (is_sso)
    {
        is_sso = false;
        m_data = other.m_data;
    }
    else
    {
        m_data = other.m_data;
    }
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

/**  LinkedList implementation  **/
template <typename T>
LinkedList<T>::LinkedList() : head(nullptr), tail(nullptr), sz(0) {}

template <typename T>
LinkedList<T>::~LinkedList()
{
    /** Look for dynamic freeing  */
    auto temp = head;
    while (temp != nullptr)
    {
        auto t = temp;
        temp = temp->next;
        t->data.~T();
        free(t);
        t = nullptr;
    }
}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList &other)
{
    if (!other.head)
        return;
    head = allocate_node(other.head->data, nullptr, nullptr);
    tail = head;

    Node *other_curr = other.head->next;

    while (other_curr != nullptr)
    {
        Node *newnode = allocate_node(other_curr->data, tail, nullptr);
        tail->next = newnode;
        tail = tail->next;
        // tail = newnode;
        other_curr = other_curr->next;
    }
}

template <typename T>
LinkedList<T>::LinkedList(LinkedList &&other)
{
    sz = other.sz;
    head = other.head;
    tail = other.tail;

    other.head = nullptr;
    other.tail = nullptr;
    sz = 0;
}

template <typename T>
LinkedList<T> &LinkedList<T>::operator=(const LinkedList &other)
{
    if (this == &other)
        return *this;

    clear();
    if (!other.head)
    {
        head = nullptr;
        tail = nullptr;
        return *this;
    }

    head = allocate_node(other.head->data, nullptr, nullptr);
    tail = head;

    Node *other_curr = other.head->next;
    while (other_curr)
    {
        Node *newnode = allocate_node(other_curr->data, tail, nullptr);
        tail->next = newnode;
        tail = newnode;
        other_curr = other_curr->next;
    }

    return *this;
}

template <typename T>
LinkedList<T> &LinkedList<T>::operator=(LinkedList &&other)
{
    if (this == &other)
        return *this;
    sz = other.sz;
    head = other.head;
    tail = other.tail;

    other.head = nullptr;
    other.tail = nullptr;
    other.sz = 0;
    return *this;
}

template <typename T>
LinkedList<T>::LinkedList(std::initializer_list<T> init)
{
    head = nullptr;
    tail = nullptr;
    for (auto i : init)
    {
        push(i);
    }
}

template <typename T>
void LinkedList<T>::push(T item)
{
    void *mem = malloc(sizeof(Node));
    if (!mem)
        panic("Memory allocation failed");

    Node *newnode = new (mem) Node(item, tail, nullptr);

    if (!head)
    {
        head = newnode;
    }
    else
    {
        tail->next = newnode;
    }

    tail = newnode;
    sz++;
}

template <typename T>
auto LinkedList<T>::pop() -> T
{
    if (head == nullptr and tail == nullptr)
    {
        panic("Pop from an empty LinkedList");
    }

    tail = tail->prev;
    auto temp = tail->next;
    auto d = temp->data;
    tail->next = nullptr;
    free(temp);
    return d;
}

template <typename T>
auto LinkedList<T>::remove(size_t pos) -> void
{
    if (sz == 0)
    {
        panic("ERROR: cannot pop from an Empty Linked List");
    }
    if (pos == sz)
    {
        panic("ERROR: cannot pop Invalid position");
    }

    if (pos == 0)
    {
        auto del = head;
        head = head->next;
        head->prev = nullptr;
        free(del);
    }

    else if (pos == sz - 1)
    {
        auto del = tail;
        tail = tail->prev;
        tail->next = nullptr;
        free(del);
    }
    // 1 - 2 - 3 - 4
    else
    {
        auto temp = head;
        for (int i = 0; i < pos - 1; i++)
        {
            temp = temp->next;
        }
        auto del = temp->next;
        auto after = temp->next->next;
        temp->next = after;
        after->prev = temp;
        free(del);
    }
}

template <typename T>
auto LinkedList<T>::insert(T m_d, size_t idx) -> void
{
    if (idx >= sz + 1)
    {
        panic("ERROR: Cannot insert data this far");
    }
    if (idx == 0)
    {
        void *mem = malloc(sizeof(Node));
        auto newnode = new (mem) Node(m_d);
        newnode->next = head;
        head->prev = newnode;
        head = newnode;
    }
    else if (idx == sz)
    {
        push(m_d);
        // 1 - 2 - 3 - 4
    }
    else
    {
        auto temp = head;
        void *mem = malloc(sizeof(Node));
        for (size_t i = 0; i < idx - 1; i++)
        {
            temp = temp->next;
        }
        auto newnode = new (mem) Node(m_d);
        auto after = temp->next;
        temp->next = newnode;
        newnode->prev = temp;
        newnode->next = after;
        after->prev = newnode;
    }
}

template <typename T>
auto LinkedList<T>::back() -> T
{
    if (tail == nullptr)
    {
        panic("Error: LinkedList empty cannot pop from the linkedlist");
    }
    return tail->data;
}

template <typename T>
auto LinkedList<T>::front() -> T
{
    if (head == nullptr)
    {
        panic("Error: LinkedList is empty cannot pop from the front");
    }
    return head->data;
}

template <typename T>
LinkedList<T>::Node *LinkedList<T>::allocate_node(const T &value, Node *prev, Node *next)
{
    void *mem = malloc(sizeof(Node));
    if (!mem)
        panic("ERROR: Allocation failed");
    return new (mem) Node(value, prev, next);
}

template <typename T>
auto LinkedList<T>::clear() -> void
{
    auto temp = head;
    while (temp != nullptr)
    {
        auto t = temp;
        temp = temp->next;
        free(t);
        t = nullptr;
    }
}

template <typename T, int N>
auto StatArray<T, N>::operator[](index_type index) const -> const T &
{
    auto cmp = (index < static_cast<index_type>(M_size)) and (-index + 1 < M_size + 1);
    if (!cmp)
    {
        panic("ERROR: index out of bounds of StatArray");
    }
    auto in = index < 0 ? index + M_size : index;
    return M_arr[in];
}

template <typename T, int N>
auto StatArray<T, N>::operator[](index_type index) -> T &
{
    auto cmp = (index < static_cast<index_type>(M_size)) and (-index + 1 < M_size + 1);
    if (!cmp)
    {
        panic("ERROR: index out of bounds of StatArray");
    }
    auto in = index < 0 ? index + M_size : index;
    return M_arr[in];
}

template <typename T, int N>
auto StatArray<T, N>::max_size() -> size_t
{
    return M_size;
}

template <typename T, int N>
auto StatArray<T, N>::back() -> T
{
    if (M_size == 0)
    {
        panic("ERROR: empty Array with no size");
    }
    return M_arr[N - 1];
}

template <typename T, int N>
auto StatArray<T, N>::front() -> T
{
    if (M_size == 0)
    {
        panic("ERROR: empty Array with no size");
    }
    return M_arr[0];
}

struct HIVE__CXX_Range_iterator
{
    int s;
    int st;
    int ed;
    explicit HIVE__CXX_Range_iterator(int s_, int st_, int ed_) : s(s_), st(st_), ed(ed_) {}

    HIVE__CXX_Range_iterator &operator++()
    {
        s += st;
        return *this;
    }

    HIVE__CXX_Range_iterator operator++(int)
    {
        auto c = *this;
        s += st;
        return c;
    }

    bool operator!=(const HIVE__CXX_Range_iterator &other) const
    {
        return (st > 0) ? (s < other.s) : (s > other.s);
    }
    int operator*() const { return s; }
};

struct Range
{
    int current;
    int step;
    int en;
    Range(int _C) : current(0), step(1), en(_C) {}
    Range(int s, int se, int ee) : current(s), step(se), en(ee) {}

    HIVE__CXX_Range_iterator begin()
    {
        return HIVE__CXX_Range_iterator(current, step, en);
    }

    HIVE__CXX_Range_iterator end()
    {
        return HIVE__CXX_Range_iterator(en, step, en);
    }
};

/**   Can work with any custom iterable datatypes which has begin and end functions and with proper operator it automatically deduces the types of underline container and its
 *    iterators !!!  i got it finally yeah yeah yeah
 */
template <typename Iterator>
struct HIVE__CXX_Enumerate_iterator
{
    Iterator m_iter{};
    size_t idx{};

    bool operator!=(const HIVE__CXX_Enumerate_iterator &other)
    {
        return m_iter != other.m_iter;
    }

    auto operator*()
    {
        return std::pair<size_t, decltype(*m_iter)>{idx, *m_iter};
    }

    HIVE__CXX_Enumerate_iterator &operator++()
    {
        ++m_iter;
        idx++;
        return *this;
    }

    HIVE__CXX_Enumerate_iterator(const Iterator &iter, size_t index = 0)
        : m_iter(iter), idx(index) {}
};

/*** OK OK OK  */
template <typename Container>
struct Enumerate
{
    using Iterator = decltype(make_rvalue<Container &>::instance().begin());

    Container data;

    HIVE__CXX_Enumerate_iterator<Iterator> begin()
    {
        return {data.begin(), 0};
    }

    HIVE__CXX_Enumerate_iterator<Iterator> end()
    {
        return {data.end(), static_cast<size_t>(data.size())};
    }
};
