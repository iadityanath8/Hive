#pragma once
#include <cstdlib>
#include "hive.hpp"

unsigned long CXX_HASH_UTILITY(String a, int mod = 20)
{
    unsigned long hash = 0xa5c;
    unsigned long prime = 611113;

    for (auto i : a)
    {
        hash ^= i;
        hash *= prime;
    }
    return hash % mod;
}


/***   DJB 2  src -> http://www.cse.yorku.ca/~oz/hash.html */
unsigned long djbhash__(String str, int mode = 20)
{
    unsigned long hash = 5381;
    for (size_t i = 0; i < str.size(); i++)
    {
        hash = ((hash << 5) + hash) + str[i];
    }
    return hash;
}

/**** loose loose src -> http://www.cse.yorku.ca/~oz/hash.html */
unsigned long llhash(String str, int mod = 20)
{
    unsigned int hash = 0;

    for (size_t i = 0;i < str.size();i++) {
        hash += str[i];
    }

    return hash;
}


/**** Primitive types  */
constexpr uint64_t hash_int(uint64_t x) {
    x ^= x >> 33;
    x *= 0xff51afd7ed558ccdULL;
    x ^= x >> 33;
    x *= 0xc4ceb9fe1a85ec53ULL;
    x ^= x >> 33;
    return x;
}

constexpr uint64_t hash_ptr(const void* ptr) {
    return hash_int(reinterpret_cast<uint64_t>(ptr));
}


constexpr uint64_t hash_float(double x) {
    uint64_t bits;
    static_assert(sizeof(double) == sizeof(uint64_t), "Double must be 64-bit");
    memcpy(&bits, &x, sizeof(x));  // Get raw IEEE-754 representation
    return hash_int(bits);  // Use integer hash
}




// #include <immintrin.h>
// #include <vector>
// #include <cstdint>

// template <typename K, typename V>
// int simd_probe(const std::vector<bucket<K, V>>& buckets, uint64_t hash, const K& key) {
//     if (buckets.empty()) return -1;

//     const __m256i hash_vec = _mm256_set1_epi64x(static_cast<int64_t>(hash));
//     int num_buckets = buckets.size();

//     for (int i = 0; i < num_buckets; i += 4) {
//         // Load 4 hashes from buckets
//         __m256i bucket_hashes = _mm256_set_epi64x(
//             static_cast<int64_t>(buckets[i+3].hash),
//             static_cast<int64_t>(buckets[i+2].hash),
//             static_cast<int64_t>(buckets[i+1].hash),
//             static_cast<int64_t>(buckets[i].hash)
//         );

//         // Compare hashes
//         __m256i cmp = _mm256_cmpeq_epi64(bucket_hashes, hash_vec);
//         int mask = _mm256_movemask_pd(_mm256_castsi256_pd(cmp));

//         if (mask) {
//             int match_idx = i + (__builtin_ctz(mask) / 2);  // Find first match
//             if (buckets[match_idx].key == key) {
//                 return match_idx;  // Verify key to resolve collisions
//             }
//         }
//     }
//     return -1; // Not found
// }
