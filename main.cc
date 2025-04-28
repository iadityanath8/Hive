#include "./include/hive.hpp"

using namespace hive;

template <class Func, class T>
List<typename T::ValueType> map(Func f, T& v) {
    List<typename T::ValueType> p;
    for (auto i : v) {
        p.push(f(i));
    }
    return p;
}

int main() {
    List<int> a = {1,2,3,5};
    List<int> b = {4,5,6,7};
    List<int> c = {8,8,8,8};
    List<int> d = {11,12,31,41};

    for (auto [x,y,z,w]: zipn(a,b,c,d)) {
        println(x,y,z,w);
    }

    return 0;
}
