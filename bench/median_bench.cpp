#include <daxe.h>
#include <daxe/compat.h>
#include <chrono>
#include <random>

using namespace dax;

template<typename Func>
f64 benchmark(const str& name, i64 iterations, Func&& f) {
    auto start = std::chrono::high_resolution_clock::now();
    for (i64 i = 0; i < iterations; ++i) {
        f();
    }
    auto end = std::chrono::high_resolution_clock::now();
    f64 us = std::chrono::duration<f64, std::micro>(end - start).count() / iterations;
    print(name, ":", us, "us/op");
    return us;
}

int main() {
    print("=== Median Optimization Benchmark ===\n");
    
    // Generate random data
    std::mt19937 gen(42);
    std::uniform_int_distribution<i64> dist(1, 10000);
    
    vi64 data(10000);
    for (auto& x : data) x = dist(gen);
    
    const i64 N = 1000;
    
    print("--- median() on 10K elements ---");
    f64 time = benchmark("dax::median (nth_element)", N, [&]() {
        volatile f64 r = median(data);
        (void)r;
    });
    
    // Reference: full sort version
    f64 sorttime = benchmark("std::sort + access", N, [&]() {
        auto copy = data;
        std::sort(copy.begin(), copy.end());
        volatile f64 r = static_cast<f64>(copy[copy.size()/2]);
        (void)r;
    });
    
    print("\nSpeedup:", sorttime / time, "x faster");
    
    return 0;
}
