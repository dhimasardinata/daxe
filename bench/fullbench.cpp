#include <daxe.h>
#include <daxe/compat.h>
#include <chrono>
#include <random>
#include <iomanip>

using namespace dax;

// ==========================================
// BENCHMARK HARNESS
// ==========================================
class Benchmark {
    str name;
    i64 iterations;
    f64 totalns = 0;
    
public:
    Benchmark(const str& n, i64 iters = 100000) : name(n), iterations(iters) {}
    
    template<typename Func>
    f64 run(Func&& f) {
        auto start = std::chrono::high_resolution_clock::now();
        for (i64 i = 0; i < iterations; ++i) {
            f();
        }
        auto end = std::chrono::high_resolution_clock::now();
        totalns = std::chrono::duration<f64, std::nano>(end - start).count() / iterations;
        return totalns;
    }
    
    void report() const {
        std::cout << std::setw(30) << std::left << name << " : ";
        if (totalns < 1000) {
            std::cout << std::fixed << std::setprecision(2) << totalns << " ns/op\n";
        } else if (totalns < 1000000) {
            std::cout << std::fixed << std::setprecision(2) << totalns/1000 << " us/op\n";
        } else {
            std::cout << std::fixed << std::setprecision(2) << totalns/1000000 << " ms/op\n";
        }
    }
};

#define BENCH(name, iters, code) { Benchmark b(name, iters); b.run([&]() code); b.report(); }

// ==========================================
// MAIN
// ==========================================
int main() {
    print("╔══════════════════════════════════════════════╗");
    print("║          DAXE Comprehensive Benchmark        ║");
    print("╚══════════════════════════════════════════════╝\n");
    
    std::mt19937 gen(42);
    std::uniform_int_distribution<i64> dist(1, 10000);
    
    // Prepare test data
    vi64 data(10000);
    for (auto& x : data) x = dist(gen);
    vi64 sorted_data = sorted(data);
    
    // ========================================
    print("=== AGGREGATES ===");
    BENCH("max(10K)", 10000, { volatile auto r = max(data); (void)r; });
    BENCH("min(10K)", 10000, { volatile auto r = min(data); (void)r; });
    BENCH("sum(10K)", 10000, { volatile auto r = sum(data); (void)r; });
    BENCH("mean(10K)", 10000, { volatile auto r = mean(data); (void)r; });
    BENCH("median(10K)", 1000, { volatile auto r = median(data); (void)r; });
    
    // ========================================
    print("\n=== SEARCH ===");
    BENCH("has(10K)", 10000, { volatile auto r = has(data, 5000LL); (void)r; });
    BENCH("binsearch(10K sorted)", 100000, { volatile auto r = binsearch(sorted_data, 5000LL); (void)r; });
    BENCH("indexlower(10K sorted)", 100000, { volatile auto r = indexlower(sorted_data, 5000LL); (void)r; });
    
    // ========================================
    print("\n=== TRANSFORMATIONS ===");
    BENCH("sorted(10K)", 100, { volatile auto r = sorted(data); (void)r; });
    BENCH("reversed(10K)", 1000, { volatile auto r = reversed(data); (void)r; });
    BENCH("prefixsum(10K)", 1000, { volatile auto r = prefixsum(data); (void)r; });
    
    // ========================================
    print("\n=== DATA STRUCTURES ===");
    BENCH("FenwickTree update", 100000, { 
        FenwickTree ft(1000); 
        ft.update(500, 1); 
    });
    BENCH("FenwickTree query", 100000, { 
        static FenwickTree ft(1000);
        volatile auto r = ft.query(500);
        (void)r;
    });
    BENCH("DSU unite", 10000, { 
        DSU dsu(1000);
        for (i64 i = 0; i < 999; ++i) dsu.unite(i, i+1);
    });
    BENCH("DSU find", 100000, { 
        static DSU dsu(1000);
        volatile auto r = dsu.find(500);
        (void)r;
    });
    
    // ========================================
    print("\n=== MATH ===");
    BENCH("gcd(large)", 1000000, { volatile auto r = gcd(123456789LL, 987654321LL); (void)r; });
    BENCH("lcm(large)", 1000000, { volatile auto r = lcm(12345LL, 67890LL); (void)r; });
    BENCH("power(mod)", 100000, { volatile auto r = power(2LL, 60LL, MOD); (void)r; });
    BENCH("combinations(100,50)", 10000, { volatile auto r = combinations(100, 50); (void)r; });
    BENCH("isprime(1000003)", 100000, { volatile auto r = isprime(1000003LL); (void)r; });
    
    // ========================================
    print("\n=== SAFETY ===");
    BENCH("safediv", 1000000, { volatile auto r = safediv(100LL, 3LL); (void)r; });
    BENCH("satadd", 1000000, { volatile auto r = satadd(std::numeric_limits<i64>::max() - 1, 100LL); (void)r; });
    BENCH("clamp", 1000000, { volatile auto r = clamp(500LL, 0LL, 100LL); (void)r; });
    
    // ========================================
    print("\n=== BIT OPERATIONS ===");
    BENCH("bitcount", 1000000, { volatile auto r = bitcount(0xDEADBEEFULL); (void)r; });
    BENCH("highestbit", 1000000, { volatile auto r = highestbit(0xDEADBEEFULL); (void)r; });
    BENCH("ispow2", 1000000, { volatile auto r = ispow2(1024ULL); (void)r; });
    
    print("\n╔══════════════════════════════════════════════╗");
    print("║              Benchmark Complete              ║");
    print("╚══════════════════════════════════════════════╝");
    
    return 0;
}
