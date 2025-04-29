#include "hive_traits.hpp"

namespace hive
{
    template <typename T> /** TODO: use concept to generalize more **/
    struct HIVE__CXX_Range_iterator
    {
        using ValueType = typename T::ValueType;
        using referece = ValueType &;
        using pointer = ValueType *;

        ValueType s;
        ValueType st;
        ValueType ed;
        explicit HIVE__CXX_Range_iterator(ValueType s_, ValueType st_, ValueType ed_) : s(s_), st(st_), ed(ed_) {}

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

    template <typename T> /** TODO: use Concept to genralize more **/
    struct Range
    {
        using ValueType = T;
        using iterator = HIVE__CXX_Range_iterator<Range<T>>;

        T current;
        T step;
        T en;
        Range(T _C) : current(0), step(1), en(_C) {}
        Range(T s, T ee, T se) : current(s), step(se), en(ee) {}
        Range(T s, T ee) : current(s), en(ee), step(1) {}

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
    struct ZipIterator
    {
        Iterator1 iter1;
        Iterator2 iter2;

        using ValueType1 = decltype(*iter1);
        using ValueType2 = decltype(*iter2);

        auto operator*()
        {
            if constexpr (is_tuple_v<ValueType2>)
            {
                return std::tuple_cat(std::make_tuple(*iter1), *iter2);
            }
            else
            {
                return std::tuple_cat(std::make_tuple(*iter1), std::make_tuple(*iter2));
            }
        }

        ZipIterator &operator++()
        {
            ++iter1;
            ++iter2;
            return *this;
        }

        bool operator!=(const ZipIterator &other)
        {
            return iter1 != other.iter1 and iter2 != other.iter2;
        }
    };

    // takes 2 container and returns a pair of values of these 2 containers
    template <Iterable Container1, Iterable Container2>
    struct Zip
    {
        Container1 m_d1;
        Container2 m_d2;

        Zip(Container1 c1, Container2 c2) : m_d1(c1), m_d2(c2)
        {
        }

        using Iterator1 = decltype(make_rvalue<Container1 &>::instance().begin());
        using Iterator2 = decltype(make_rvalue<Container2 &>::instance().begin());
        using iterator = ZipIterator<Iterator1, Iterator2>;

        iterator begin()
        {
            return {m_d1.begin(), m_d2.begin()};
        }

        iterator end()
        {
            return {m_d1.end(), m_d2.end()};
        }
    };


    /** Map **/
    template <class T>
    struct MapIterator {
        using Iterator = typename T::Iterator;
        using Function = typename T::Function;
        Iterator m_iter;
        Function m_f;

        explicit MapIterator(Iterator&& i, Function&& f): m_iter(i), m_f(f) {}
        auto operator*() {
            return m_f(*m_iter);
        }

        MapIterator &operator++()
        {
            ++m_iter;
            return *this;
        }

        bool operator !=(const MapIterator& other) {
            return m_iter != other.m_iter;
        }
    };
    
    template <class Func, Iterable Container>
    struct Map {
        Func fn;
        Container c;

        template <typename F, typename C>
        Map(F &&f, C &&c1) : fn(std::forward<F>(f)), c(std::forward<C>(c1)) {}

        using Iterator  = decltype(make_rvalue<Container&>::instance().begin());
        using Function  = Func;
        using iterator  = MapIterator<Map<Func, Container>>;

        /** usefull to identify other containers as taking the Iterator type and deducing the type using CTAD **/
        using ValueType = typename Container::ValueType;

        iterator begin() {
            return iterator(c.begin(), std::move(fn));
        }

        iterator end()   {
            return iterator(c.end(), std::move(fn));
        }
    };

    template <typename Func, Iterable Container>
    Map(Func, Container) -> Map<Func, Container>;

    template <typename T>
    struct ZipWithIterator {
        using Function  = typename T::Function;
        using Iterator1 = typename T::Iterator1;
        using Iterator2 = typename T::Iterator2;
        Function m_f;
        Iterator1 m_iter1;
        Iterator2 m_iter2;

        explicit ZipWithIterator(Function&& f, Iterator1&& i,Iterator2&& i2): m_f(f), m_iter1(i), m_iter2(i2) {}
        auto operator*() {
            return m_f(*m_iter1, *m_iter2);
        }

        ZipWithIterator &operator++()
        {
            ++m_iter1;
            ++m_iter2;
            return *this;
        }

        bool operator!=(const ZipWithIterator &other)
        {
            return m_iter1 != other.m_iter1 and m_iter2 != other.m_iter2;
        }
    };

    template <typename Func, Iterable Container1, Iterable Container2>
    struct ZipWith {
        Func       f;
        Container1 c1;
        Container2 c2;
        using Iterator1 = decltype(make_rvalue<Container1&>::instance().begin());
        using Iterator2 = decltype(make_rvalue<Container2&>::instance().begin());
        using Function  = Func;
        using iterator  = ZipWithIterator<ZipWith<Func,Container1,Container2>>;


        /** usefull to identify other containers as taking the Iterator type and deducing the type using CTAD **/
        using ValueType = typename Container1::ValueType;

        iterator begin() {
            return iterator(std::move(f),c1.begin(),c2.begin());
        }

        iterator end()   {
            return iterator(std::move(f),c1.end(),c2.end());
        }
    };

    template <typename T>
    struct FilterIterator { 
        using Iterator = typename T::Iterator;
        using Function = typename T::Function; 
        Iterator m_iter;
        Iterator m_end;
        Function m_f;

        explicit FilterIterator(Function&& f, Iterator&& i, Iterator&& e): m_iter(i), m_f(f), m_end(e) {
            advance_to_valid();
        }

        auto operator*() {
            return *m_iter;
        }

        FilterIterator &operator++()
        {
            ++m_iter;
            advance_to_valid();
            return *this;
        }

        bool operator!=(const FilterIterator &other)
        {
            return m_iter != other.m_iter;
        }

        void advance_to_valid() {
            while (m_iter != m_end && !m_f(*m_iter)) {
                ++m_iter;
            }
        }
    };  

    template <typename Func, Iterable Container>
    struct Filter {
        Func f;
        Container c;
        using iterator = FilterIterator<Filter<Func, Container>>;
        using Iterator = decltype(make_rvalue<Container&>::instance().begin());
        using Function = Func;


        /** usefull to identify other containers as taking the Iterator type and deducing the type using CTAD **/
        using ValueType = typename Container::ValueType;

        iterator begin() {
            return iterator(std::move(f), c.begin(),c.end());
        }

        iterator end()  {
            return iterator(std::move(f), c.end(),c.end());
        }
    };  

    template <Iterable A, Iterable B>
    auto zipn(A &&a, B &&b)
    {
        return Zip((A &&)a, (B &&)b);
    }

    template <Iterable A, Iterable B, Iterable... Rest>
    auto zipn(A &&a, B &&b, Rest &&...rest)
    {
        return Zip((A &&)a, zipn((B &&)b, (Rest &&)rest...));
    }

    template<typename Func, Iterable A, Iterable B>
    auto zipWith(Func f, A&& a, B&& b) {
        return ZipWith(f, (A&&)a, (B&&)b);
    }
    
    template <typename Func, Iterable A, Iterable B, Iterable... Rest>
    auto zipWith(Func f, A&& a, B&& b, Rest&& ...rest) {
        return ZipWith(f,(A&&)a,zipWith(f,(B&&)b,(Rest&&)rest...));
    }
}