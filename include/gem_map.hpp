#pragma once 
#include "hash.hpp"
// #include "hive.hpp"
#include "list.hpp"

namespace hive {
template <typename Key, typename Value>
class GemMap{
    GemMap(Iterable it);   
    auto put(Key k, Value v) -> void;
    auto get(Key k) -> Value;
    auto size() -> size_t;
private:
    struct Bucket {
        Key k;
        Value v;
        uint64_t hash;
    };
    List<Bucket> buckets {};
};
}
