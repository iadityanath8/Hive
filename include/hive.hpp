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

namespace hive
{

    [[noreturn]] inline void panic(const char *msg__);

    /** will introduce small string optimization (SSO) soon for small and usable strings  */
    class String
    {
    public:
        using iterator = List<char>::iterator;
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

        iterator begin()
        {
            if (is_sso)
            {
                return List<char>::iterator(sso_buffer);
            }
            return List<char>::iterator(m_data.data());
        }

        iterator end()
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

    void hive_print1(unsigned long long __a)
    {
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
    }

    template <typename... Args>
    void println(Args &&...args)
    {
        ((hive_print1(args), printf(" ")), ...);
        printf("\n");
        fflush(stdout);
    }

    // we'll see about the string_view and  all the stuff in here
    void printf(const char *_f)
    {
        print(_f);
    }

    template <typename T, typename... Args>
    inline void printf(const char *_f, T&& value, Args&&... args) // only requesting bro 
    {
        while (*_f)
        {
            if (*_f == '{' && *(_f + 1) == ':' && *(_f + 2) == 'N')
            {
                if constexpr (std::is_floating_point_v<T>)
                {
                    int precision = 6;
                    const char *ptr = _f + 3;

                    if (*ptr >= '0' && *ptr <= '9')
                    {
                        precision = 0;
                        while (*ptr >= '0' && *ptr <= '9')
                        {
                            precision = precision * 10 + (*ptr - '0');
                            ++ptr;
                        }
                    }

                    char format_specifier[10];
                    snprintf(format_specifier, sizeof(format_specifier), "%%.%df", precision);

                    char buffer[64];
                    snprintf(buffer, sizeof(buffer), format_specifier, value);

                    hive_print1(buffer);

                    printf(ptr + 1, args...);
                    return;
                }
            }

            if (*_f == '{' and *(_f + 1) == '}')
            {
                hive_print1(value);
                printf(_f + 2, args...);
                return;
            }
            else
            {
                putchar(*_f);
                _f++;
            }
        }
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
    template <Iterable ia>
    List<T>::List(ia&& iter) {
        for (auto i : iter) {
            push(i);
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
        items = (T *)malloc(_s * sizeof(T));
        if (items == nullptr)
            panic("Failed to allocate the memory");
    }

    template <typename T>
    List<T>::List(const List<T> &other)
    {
        capacity = other.capacity;
        sz = other.sz;
        items = static_cast<T *>(malloc(capacity * sizeof(T)));

        if constexpr (std::is_trivially_copyable_v<T>)
        {
            memcpy(items, other.items, sz * sizeof(T)); // Fastest path
        }
        else
        {
            for (index_type i = 0; i < sz; ++i)
            {
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
            items = static_cast<T *>(malloc(sizeof(T) * capacity));
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
        if (_s + 1 <= capacity) // TODO: LOOK here
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
    __attribute__((always_inline)) inline constexpr T List<T>::pop()
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
    __attribute__((always_inline)) inline T &List<T>::operator[](index_type index)
    {

#ifdef HIVE_BOUND_CHECK
        // support for negative index
        if (index >= sz or (-index) >= (sz + 1))
        {
            panic("index out of bounds");
        }
#endif

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

#ifdef HIVE_BOUND_CHECK

        if (index >= sz or (-index) > (sz + 1))
        {
            panic("index out of bounds");
        }

#endif

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

        if (capacity < other.sz)
        {
            extend(other.sz);
        }
        sz = other.sz;

        if constexpr (std::is_trivially_copyable_v<T>)
        {
            memcpy(items, other.items, sz * sizeof(T));
        }
        else
        {
            for (index_type i = 0; i < sz; i++)
            {
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
            is_sso = false;
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

    String::~String()
    {
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

#ifdef HIVE_BOUND_CHECK
        if (m_data.size() == 0 or sso_index == 0)
        {
            panic("Cannot pop out from the String");
        }
#endif

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
#ifdef HIVE_BOUND_CHECK
            auto cmp = (index < static_cast<index_type>(SSO_SIZE) and (-index + 1) < sso_index + 1); // TODO: check please if false
            if (!cmp)
            {
                panic("Index out of bound for the String");
            }
#endif
            index_type in = index < 0 ? sso_index + index : index;
            return sso_buffer[in];
        }
        return m_data[index];
    }

    const char &String::operator[](index_type index) const
    {
        if (is_sso)
        {

#ifdef HIVE_BOUND_CHECK
            auto cmp = (index < static_cast<index_type>(SSO_SIZE) and (-index + 1) < sso_index + 1); // TODO: check please of false
            if (!cmp)
            {
                panic("Index out of bound for the String");
            }
#endif

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

#ifdef HIVE_BOUND_CHECK
        if (head == nullptr and tail == nullptr)
        {
            panic("Pop from an empty LinkedList");
        }
#endif

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

#ifdef HIVE_BOUND_CHECK
        if (sz == 0)
        {
            panic("ERROR: cannot pop from an Empty Linked List");
        }
        if (pos == sz)
        {
            panic("ERROR: cannot pop Invalid position");
        }
#endif

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

#ifdef HIVE_BOUND_CHECK
        if (idx >= sz + 1)
        {
            panic("ERROR: Cannot insert data this far");
        }
#endif

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
#ifdef HIVE_BOUND_CHECK
        if (tail == nullptr)
        {
            panic("Error: LinkedList empty cannot pop from the linkedlist");
        }
        return tail->data;
#endif
    }

    template <typename T>
    auto LinkedList<T>::front() -> T
    {

#ifdef HIVE_BOUND_CHECK
        if (head == nullptr)
        {
            panic("Error: LinkedList is empty cannot pop from the front");
        }
#endif

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

#ifdef HIVE_BOUND_CHECK
        auto cmp = (index < static_cast<index_type>(M_size)) and (-index + 1 < M_size + 1);
        if (!cmp)
        {
            panic("ERROR: index out of bounds of StatArray");
        }
#endif

        auto in = index < 0 ? index + M_size : index;
        return M_arr[in];
    }

    template <typename T, int N>
    auto StatArray<T, N>::operator[](index_type index) -> T &
    {

#ifdef HIVE_BOUND_CHECK
        auto cmp = (index < static_cast<index_type>(M_size)) and (-index + 1 < M_size + 1);
        if (!cmp)
        {
            panic("ERROR: index out of bounds of StatArray");
        }
#endif

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

#ifdef HIVE_BOUND_CHECK
        if (M_size == 0)
        {
            panic("ERROR: empty Array with no size");
        }
#endif

        return M_arr[N - 1];
    }

    template <typename T, int N>
    auto StatArray<T, N>::front() -> T
    {

#ifdef HIVE_BOUND_CHECK
        if (M_size == 0)
        {
            panic("ERROR: empty Array with no size");
        }
#endif
        return M_arr[0];
    }

    template <typename T> /** TODO: use concept to generalize more **/
    struct HIVE__CXX_Range_iterator
    {
        using ValueType = typename T::ValueType;
        using referece =  ValueType&;
        using pointer  = ValueType*;

        ValueType s;
        ValueType st;
        ValueType ed;
        explicit HIVE__CXX_Range_iterator(ValueType s_,ValueType st_,ValueType ed_) : s(s_), st(st_), ed(ed_) {}

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
        ValueType operator*() const { return s; }
    };

    template <typename T>  /** TODO: use Concept to genralize more **/
    struct Range
    {
        using ValueType = T;
        using iterator = HIVE__CXX_Range_iterator<Range<T>>;

        T current;
        T step;
        T en;        
        Range(T _C) : current(0), step(1), en(_C) {}
        Range(T s, T ee, T se) : current(s), step(se), en(ee) {}
        Range(T s, T ee): current(s), en(ee), step(1) {}

        iterator begin()
        {
            return iterator(current, step, en);
        }

        iterator end()
        {
            return iterator(en, step, en);
        }
    };

    /**   Can work with any custom iterable datatypes which has begin and end functions and with proper operator it automatically deduces the types of underline container and its
     *    iterators !!!  i got it finally yeah yeah yeah
     */
    
    template <class Iterator>
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

    template <Iterable Container>
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


    template <class Iterator1, class Iterator2> 
    struct ZipIterator {
        Iterator1 iter1;
        Iterator2 iter2;

        using ValueType1 = decltype(*iter1);
        using ValueType2 = decltype(*iter2);

        auto operator*() {
            if constexpr (is_tuple_v<ValueType2>) {
                return std::tuple_cat(std::make_tuple(*iter1), *iter2);
            } else {
                return std::tuple_cat(std::make_tuple(*iter1), std::make_tuple(*iter2));
            }
        }

        ZipIterator &operator++() {
            ++iter1;
            ++iter2;
            return *this;
        }
        
        bool operator!=(const ZipIterator &other)
        {
            return iter1 != other.iter1 and iter2!= other.iter2; 
        }
    };


    // takes 2 container and returns a pair of values of these 2 containers 
    template <Iterable Container1, Iterable Container2>
    struct Zip {
        Container1 m_d1;
        Container2 m_d2;

        Zip(Container1 c1, Container2 c2): m_d1(c1), m_d2(c2) {

        }

        using Iterator1 = decltype(make_rvalue<Container1 &>::instance().begin());
        using Iterator2 = decltype(make_rvalue<Container2 &>::instance().begin());
        using iterator  = ZipIterator<Iterator1, Iterator2>;

        iterator begin() {
            return {m_d1.begin(), m_d2.begin()};
        }

        iterator end () {
            return {m_d1.end(), m_d2.end()};
        }
    };

    template <Iterable A, Iterable B>
    auto zipn(A&& a, B&& b) {
        return Zip((A&&)a, (B&&)b);
    }

    template <Iterable A, Iterable B, Iterable... Rest>
    auto zipn(A&& a, B&& b, Rest&&... rest) {
        return Zip((A&&)a, zipn((B&&)b, (Rest&&)rest...));
    }
    
}