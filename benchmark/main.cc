#include <iostream>
#include <vector>
#include <chrono>
#include <atomic>
#include "../include/hive.hpp"  

constexpr size_t N = 10'000'0000;  

template <typename Func>
double benchmark(Func f) {
    auto start = std::chrono::high_resolution_clock::now();
    f();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::micro>(end - start).count(); // µs
}

int main() {
    std::vector<int> vec(N);
    List<int> lst(N);
    
    // Fill both containers
    for (size_t i = 0; i < N; ++i) {
        vec[i] = i;
        lst.push(i);
    }

    // Vector Iteration (Range-based for loop)
    double vec_time_range = benchmark([&]() {
        int sum = 0;
        std::atomic_thread_fence(std::memory_order_relaxed);
        for (int x : vec) sum += x;
        std::atomic_thread_fence(std::memory_order_relaxed);
    });

    // List Iteration (Range-based for loop)
    double lst_time_range = benchmark([&]() {
        int sum = 0;
        std::atomic_thread_fence(std::memory_order_relaxed);
        for (int x : lst) sum += x;
        std::atomic_thread_fence(std::memory_order_relaxed);
    });

    // Vector Iteration (Iterator)
    double vec_time_iter = benchmark([&]() {
        int sum = 0;
        std::atomic_thread_fence(std::memory_order_relaxed);
        for (auto it = vec.begin(); it != vec.end(); ++it) sum += *it;
        std::atomic_thread_fence(std::memory_order_relaxed);
    });

    // List Iteration (Custom Iterator)
    double lst_time_iter = benchmark([&]() {
        int sum = 0;
        std::atomic_thread_fence(std::memory_order_relaxed);
        for (auto it = lst.begin(); it != lst.end(); ++it) sum += *it;
        std::atomic_thread_fence(std::memory_order_relaxed);
    });

    std::cout << "Benchmark Results (in µs):\n";
    std::cout << "Vector (Range-based)  : " << vec_time_range << " µs\n";
    std::cout << "List   (Range-based)  : " << lst_time_range << " µs\n";
    std::cout << "Vector (Iterator)     : " << vec_time_iter << " µs\n";
    std::cout << "List   (Iterator)     : " << lst_time_iter << " µs\n";

    return 0;
}
