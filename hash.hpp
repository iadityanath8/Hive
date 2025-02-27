#pragma once 
#include <cstdlib>
#include "hive.hpp"

unsigned long CXX_HASH_UTILITY(String a, int mod = 20) {
    int res = 0;
    int mask = 0x95329;

    for (auto i : a) {
        res += (mask ^ i) >> 5; // 5
        mask = mask >> 31; // 21
    }
    return res % mod;    
}

unsigned long hash__(String str) {
    unsigned long hash = 5381;
    for (size_t i = 0; i < str.size(); i++) {
        hash = ((hash << 5) + hash) + str[i]; 
    }
    return hash;
}
