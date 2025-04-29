// iter_example.cc
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <initializer_list>
#include <type_traits>
#include <utility>
#include <new>

// A minimal C‐style string class for testing
class string {
    char* data_;
public:
    string(): data_(nullptr) {}

    string(const char* s) {
        if (s) {
            size_t n = std::strlen(s);
            data_ = (char*)std::malloc(n+1);
            std::memcpy(data_, s, n+1);
        } else {
            data_ = nullptr;
        }
    }

    // copy
    string(const string& o) {
        if (o.data_) {
            size_t n = std::strlen(o.data_);
            data_ = (char*)std::malloc(n+1);
            std::memcpy(data_, o.data_, n+1);
        } else {
            data_ = nullptr;
        }
    }
    // move
    string(string&& o) noexcept : data_(o.data_) {
        o.data_ = nullptr;
    }
    string& operator=(const string& o) {
        if (this != &o) {
            std::free(data_);
            if (o.data_) {
                size_t n = std::strlen(o.data_);
                data_ = (char*)std::malloc(n+1);
                std::memcpy(data_, o.data_, n+1);
            } else {
                data_ = nullptr;
            }
        }
        return *this;
    }
    string& operator=(string&& o) noexcept {
        if (this != &o) {
            std::free(data_);
            data_ = o.data_;
            o.data_ = nullptr;
        }
        return *this;
    }

    bool operator==(const string& o) const {
        if (!data_ && !o.data_) return true;
        if (!data_ || !o.data_) return false;
        return std::strcmp(data_, o.data_) == 0;
    }

    const char* c_str() const { return data_ ? data_ : ""; }

    ~string() { std::free(data_); }
};

// Generic List<T>
template<typename T>
class List {
    T*    items;
    size_t sz, cap;

  public:
    List(): items(nullptr), sz(0), cap(0) {}

    // initializer_list constructor
    List(std::initializer_list<T> init)
      : items(nullptr), sz(0), cap(init.size())
    {
        items = (T*)std::malloc(sizeof(T) * cap);
        if (!items) std::abort();
        for (auto const& v : init) {
            new (&items[sz]) T(v);
            ++sz;
        }
    }

    // copy
    List(const List& o)
      : items(nullptr), sz(o.sz), cap(o.cap)
    {
        items = (T*)std::malloc(sizeof(T) * cap);
        if (!items) std::abort();
        if constexpr (std::is_trivially_copyable_v<T>) {
            std::memcpy(items, o.items, sz * sizeof(T));
        } else {
            for (size_t i = 0; i < sz; ++i)
                new (&items[i]) T(o.items[i]);
        }
    }
    // move
    List(List&& o) noexcept
      : items(o.items), sz(o.sz), cap(o.cap)
    {
        o.items = nullptr;
        o.sz = o.cap = 0;
    }

    // push
    void push(T v) {
        if (sz == cap) {
            size_t newc = cap ? cap * 2 : 4;
            items = (T*)std::realloc(items, newc * sizeof(T));
            if (!items) std::abort();
            cap = newc;
        }
        new (&items[sz]) T(std::move(v));
        ++sz;
    }

    // indexed access
    T&       operator[](size_t i)       { return items[i]; }
    T const& operator[](size_t i) const { return items[i]; }
    size_t   size()     const { return sz; }

    // destructor
    ~List() {
        if constexpr (!std::is_trivially_destructible_v<T>) {
            for (size_t i = 0; i < sz; ++i)
                items[i].~T();
        }
        std::free(items);
    }
};

// Test in main
int main() {
    // Nested list of strings
    List<List<int>> a = {
        { 1, 2, 3 },
        { 4 }
    };

    // Print them to confirm behavior
    // for (size_t i = 0; i < a.size(); ++i) {
    //     for (size_t j = 0; j < a[i].size(); ++j) {
    //         std::printf("a[%zu][%zu] = %s\n",
    //                     i, j, a[i][j].c_str());
    //     }
    // }

    return 0;
}
