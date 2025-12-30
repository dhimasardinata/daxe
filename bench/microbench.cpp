#include <daxe.h>
#include <daxe/compat.h>

#include <chrono>


using namespace dax;

template <typename Func>
f64 benchmark(const str& name, i64 iterations, Func&& f) {
    auto start = std::chrono::high_resolution_clock::now();
    for (i64 i = 0; i < iterations; ++i) {
        f();
    }
    auto end = std::chrono::high_resolution_clock::now();
    f64 ns = std::chrono::duration<f64, std::nano>(end - start).count() / iterations;
    print(name, ":", ns, "ns/op");
    return ns;
}

int main() {
    print("=== DAXE Micro-Benchmarks ===\n");

    const i64 N = 1000000;
    vi64 data(1000);
    for (i64 i = 0; i < 1000; ++i)
        data[i] = i;

    print("--- max() ---");
    f64 daxe_max = benchmark("dax::max(v)", N, [&]() {
        volatile i64 r = max(data);
        (void)r;
    });
    f64 std_max = benchmark("std::max_element", N, [&]() {
        volatile i64 r = *std::max_element(data.begin(), data.end());
        (void)r;
    });
    print("Overhead:", (daxe_max - std_max), "ns\n");

    print("--- sum() ---");
    f64 daxe_sum = benchmark("dax::sum(v)", N, [&]() {
        volatile i64 r = sum(data);
        (void)r;
    });
    f64 std_sum = benchmark("std::accumulate", N, [&]() {
        volatile i64 r = std::accumulate(data.begin(), data.end(), 0LL);
        (void)r;
    });
    print("Overhead:", (daxe_sum - std_sum), "ns\n");

    print("--- has() ---");
    f64 daxe_has = benchmark("dax::has(v, x)", N, [&]() {
        volatile bool r = has(data, 500LL);
        (void)r;
    });
    f64 std_find = benchmark("std::find", N, [&]() {
        volatile bool r = std::find(data.begin(), data.end(), 500LL) != data.end();
        (void)r;
    });
    print("Overhead:", (daxe_has - std_find), "ns\n");

    print("=== Benchmark Complete ===");
    return 0;
}
