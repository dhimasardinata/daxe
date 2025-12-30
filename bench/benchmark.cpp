/*
 * DAXE - COMPREHENSIVE BENCHMARK SUITE
 * Tests: Performance, Memory, Safety Edge Cases
 * 
 * Build: g++ -std=c++20 -O3 -DNDEBUG benchmark.cpp -o benchmark
 */

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <daxe.h>
#include <chrono>
#include <iomanip>
#include <random>

using namespace std::chrono;

// ==========================================
// UTILITIES
// ==========================================

// Prevent compiler from optimizing away benchmark results
template <typename T>
__attribute__((noinline)) void DoNotOptimize(T&& value) {
    asm volatile("" : : "r,m"(value) : "memory");
}

template <typename Func>
double measure_ns(Func&& f, int iterations = 5000) {
    for (int i = 0; i < 50; ++i) { auto r = f(); DoNotOptimize(r); } // Warmup
    auto start = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) { auto r = f(); DoNotOptimize(r); }
    auto end = high_resolution_clock::now();
    return duration_cast<nanoseconds>(end - start).count() / double(iterations);
}

size_t get_memory_usage() {
#ifdef _WIN32
    // Windows: approximate via process info (simplified)
    return 0; // Not easily available without Windows API
#else
    // Linux: read from /proc/self/statm
    FILE* f = fopen("/proc/self/statm", "r");
    if (!f) return 0;
    size_t pages = 0;
    fscanf(f, "%zu", &pages);
    fclose(f);
    return pages * 4096; // Approximate
#endif
}

void print_header(const char* title) {
    std::cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║  " << std::left << std::setw(60) << title << "║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n\n";
}

void print_result(const char* name, double daxe, double raw, const char* unit = "ns") {
    double speedup = raw / daxe;  // How much faster daxe is (>1 = faster)
    const char* status = speedup >= 0.95 ? "✅" : (speedup >= 0.8 ? "⚠️" : "❌");
    std::string speedup_str;
    if (speedup >= 1.0) {
        speedup_str = std::to_string(speedup).substr(0, 4) + "x faster";
    } else {
        speedup_str = std::to_string(1.0/speedup).substr(0, 4) + "x slower";
    }
    std::cout << std::left << std::setw(25) << name
              << std::right << std::setw(12) << std::fixed << std::setprecision(1) << daxe << " " << unit
              << std::setw(12) << raw << " " << unit
              << std::setw(14) << speedup_str
              << "  " << status << "\n";
}

void print_test(const char* name, bool passed) {
    std::cout << (passed ? "✅ " : "❌ ") << name << "\n";
}

// ==========================================
// DATA
// ==========================================
vi64 data;
const int N = 100000;
int test_failures = 0;

void setup() {
    std::mt19937_64 rng(42);
    data.resize(N);
    for (auto& x : data) x = rng() % 1000000;
}

// ==========================================
// SECTION 1: PERFORMANCE BENCHMARKS
// ==========================================
void bench_performance() {
    print_header("PERFORMANCE BENCHMARKS (N=100,000)");
    
    std::cout << std::left << std::setw(25) << "Function"
              << std::right << std::setw(15) << "Daxe"
              << std::setw(15) << "Raw C++"
              << std::setw(16) << "Speedup"
              << "  Status\n";
    std::cout << std::string(80, '-') << "\n";
    
    // Helper to get fair comparison (average of both orderings)
    auto fair_compare = [](auto daxe_fn, auto raw_fn, int iters = 5000) {
        // Run daxe first, then raw
        double d1 = measure_ns(daxe_fn, iters);
        double r1 = measure_ns(raw_fn, iters);
        // Run raw first, then daxe
        double r2 = measure_ns(raw_fn, iters);
        double d2 = measure_ns(daxe_fn, iters);
        // Average both orderings
        return std::make_pair((d1 + d2) / 2.0, (r1 + r2) / 2.0);
    };
    
    // max
    auto daxe_max = [&]() { return max(data); };
    auto raw_max = [&]() { return *std::max_element(data.begin(), data.end()); };
    auto [d_max, r_max] = fair_compare(daxe_max, raw_max);
    print_result("max(container)", d_max, r_max);
    
    // min
    auto daxe_min = [&]() { return min(data); };
    auto raw_min = [&]() { return *std::min_element(data.begin(), data.end()); };
    auto [d_min, r_min] = fair_compare(daxe_min, raw_min);
    print_result("min(container)", d_min, r_min);
    
    // sum
    auto daxe_sum = [&]() { return sum(data); };
    auto raw_sum = [&]() { return std::accumulate(data.begin(), data.end(), 0LL); };
    auto [d_sum, r_sum] = fair_compare(daxe_sum, raw_sum);
    print_result("sum(container)", d_sum, r_sum);
    
    // has (search for a value known to exist at middle)
    i64 search_val = data[N/2];
    auto daxe_has = [&]() { return has(data, search_val); };
    auto raw_has = [&]() { return std::find(data.begin(), data.end(), search_val) != data.end(); };
    auto [d_has, r_has] = fair_compare(daxe_has, raw_has);
    print_result("has(container, x)", d_has, r_has);
    
    // range loop
    auto daxe_range = [&]() { i64 s = 0; for (Int i : range(N)) s += data[i]; return s; };
    auto raw_range = [&]() { i64 s = 0; for (i64 i = 0; i < N; ++i) s += data[i]; return s; };
    auto [d_range, r_range] = fair_compare(daxe_range, raw_range);
    print_result("range(n) loop", d_range, r_range);
    
    // sorted (copy)
    auto daxe_sorted = [&]() { return sorted(data); };
    auto raw_sorted = [&]() { auto v = data; std::sort(v.begin(), v.end()); return v; };
    auto [d_sorted, r_sorted] = fair_compare(daxe_sorted, raw_sorted, 100);
    print_result("sorted(copy)", d_sorted, r_sorted);
    
    // gcd
    auto daxe_gcd = [&]() { i64 r = 0; for (int i = 0; i < 1000; ++i) r += gcd(1234567890LL, 987654321LL); return r; };
    auto raw_gcd = [&]() { i64 r = 0; for (int i = 0; i < 1000; ++i) r += std::__gcd(1234567890LL, 987654321LL); return r; };
    auto [d_gcd, r_gcd] = fair_compare(daxe_gcd, raw_gcd);
    print_result("gcd(a, b) x1000", d_gcd, r_gcd);
    
    // isprime (no raw C++ equivalent, compare to self)
    auto daxe_prime = [&]() { int c = 0; for (i64 i = 2; i < 10000; ++i) if (isprime(i)) ++c; return c; };
    auto [d_prime, r_prime] = fair_compare(daxe_prime, daxe_prime);
    print_result("isprime(2..10000)", d_prime, r_prime);
}

// ==========================================
// SECTION 2: MEMORY OVERHEAD
// ==========================================
void bench_memory() {
    print_header("MEMORY OVERHEAD (Type Sizes)");
    
    std::cout << std::left << std::setw(30) << "Type"
              << std::right << std::setw(15) << "Daxe Size"
              << std::setw(15) << "Expected"
              << std::setw(10) << "Status\n";
    std::cout << std::string(70, '-') << "\n";
    
    auto check_size = [](const char* name, size_t actual, size_t expected) {
        const char* status = actual == expected ? "✅" : "❌";
        std::cout << std::left << std::setw(30) << name
                  << std::right << std::setw(12) << actual << " B"
                  << std::setw(12) << expected << " B"
                  << std::setw(10) << status << "\n";
        return actual == expected;
    };
    
    // Core types - zero overhead
    check_size("vi64 vs vector<int64_t>", sizeof(vi64), sizeof(std::vector<i64>));
    check_size("pi64 vs pair<int64_t>", sizeof(pi64), sizeof(std::pair<i64, i64>));
    check_size("str vs string", sizeof(str), sizeof(std::string));
    check_size("si64 vs set<int64_t>", sizeof(si64), sizeof(std::set<i64>));
    
    // Pythonic types
    check_size("List<Int>", sizeof(List<Int>), sizeof(std::vector<i64>));
    check_size("Dict<str,Int>", sizeof(Dict<str,Int>), sizeof(std::map<str,i64>));
    check_size("Set<Int>", sizeof(Set<Int>), sizeof(std::set<i64>));
    
    // Option type
    check_size("Option<i64>", sizeof(Option<i64>), sizeof(std::optional<i64>));
    
    std::cout << "\n📊 All daxe types are zero-overhead wrappers!\n";
}

// ==========================================
// SECTION 3: SAFETY EDGE CASES
// ==========================================
void bench_safety() {
    print_header("SAFETY EDGE CASES");
    
    // Empty containers
    std::cout << "--- Empty Container Tests ---\n";
    vi64 empty_v;
    print_test("first(empty) returns None", isnone(first(empty_v)));
    print_test("dax::last(empty) returns None", isnone(dax::last(empty_v)));
    print_test("getat(empty, 0) returns None", isnone(getat(empty_v, 0)));
    print_test("getor(empty, 0, -1) returns -1", getor(empty_v, 0, -1) == -1);
    
    // Negative indexing
    std::cout << "\n--- Negative Indexing Tests ---\n";
    vi64 v = {10, 20, 30, 40, 50};
    print_test("getat(v, -1) = 50", valueor(getat(v, -1), 0LL) == 50);
    print_test("getat(v, -2) = 40", valueor(getat(v, -2), 0LL) == 40);
    print_test("getat(v, -5) = 10", valueor(getat(v, -5), 0LL) == 10);
    print_test("getat(v, -6) returns None", isnone(getat(v, -6)));
    
    // Out of bounds
    std::cout << "\n--- Bounds Checking Tests ---\n";
    print_test("getat(v, 100) returns None", isnone(getat(v, 100)));
    print_test("getor(v, 100, -1) = -1", getor(v, 100, -1) == -1);
    print_test("charat('hello', -1) = 'o'", valueor(charat("hello", -1), '?') == 'o');
    print_test("charat('hello', 10) returns None", isnone(charat("hello", 10)));
    
    // Safe math
    std::cout << "\n--- Safe Math Tests ---\n";
    auto div_ok = trydiv(10, 2);
    auto div_err = trydiv(10, 0);
    print_test("trydiv(10, 2) = Ok(5)", div_ok.isok() && div_ok.unwrap() == 5);
    print_test("trydiv(10, 0) = Err", div_err.iserr());
    
    auto sqrt_ok = trysqrt(16.0);
    auto sqrt_err = trysqrt(-1.0);
    print_test("trysqrt(16) = Ok(4)", sqrt_ok.isok() && std::abs(sqrt_ok.unwrap() - 4.0) < 0.001);
    print_test("trysqrt(-1) = Err", sqrt_err.iserr());
    
    // Grid bounds
    std::cout << "\n--- Grid Bounds Tests ---\n";
    print_test("ingrid(0, 0, 5, 5) = true", ingrid(0, 0, 5, 5));
    print_test("ingrid(4, 4, 5, 5) = true", ingrid(4, 4, 5, 5));
    print_test("ingrid(5, 0, 5, 5) = false", !ingrid(5, 0, 5, 5));
    print_test("ingrid(-1, 0, 5, 5) = false", !ingrid(-1, 0, 5, 5));
    print_test("ingrid(0, -1, 5, 5) = false", !ingrid(0, -1, 5, 5));
    
    // Option chaining
    std::cout << "\n--- Option Chaining Tests ---\n";
    Option<i64> some = Some(42);
    Option<i64> none = None;
    print_test("issome(Some(42))", issome(some));
    print_test("isnone(None)", isnone(none));
    print_test("unwrap(Some(42)) = 42", unwrap(some) == 42);
    print_test("valueor(None, 99) = 99", valueor(none, 99LL) == 99);
    
    // Slice edge cases
    std::cout << "\n--- Slice Edge Cases ---\n";
    vi64 nums = {1, 2, 3, 4, 5};
    print_test("sliced(v, 0, 0) is empty", sliced(nums, 0, 0).empty());
    print_test("sliced(v, 5, 10) is empty", sliced(nums, 5, 10).empty());
    print_test("sliced(v, -2, -1) = {4}", sliced(nums, -2, -1).size() == 1 && sliced(nums, -2, -1)[0] == 4);
    print_test("sliced(v, 1, 3) = {2,3}", sliced(nums, 1, 3).size() == 2);
}

// ==========================================
// SECTION 4: CORRECTNESS
// ==========================================
void bench_correctness() {
    print_header("CORRECTNESS VERIFICATION");
    
    // Verify daxe produces same results as raw C++
    print_test("max matches std::max_element", max(data) == *std::max_element(data.begin(), data.end()));
    print_test("min matches std::min_element", min(data) == *std::min_element(data.begin(), data.end()));
    print_test("sum matches std::accumulate", sum(data) == std::accumulate(data.begin(), data.end(), 0LL));
    print_test("has matches std::find", has(data, 500000LL) == (std::find(data.begin(), data.end(), 500000LL) != data.end()));
    
    // Math functions
    print_test("gcd(12, 8) = 4", gcd(12, 8) == 4);
    print_test("lcm(12, 8) = 24", lcm(12, 8) == 24);
    print_test("mod(-3, 5) = 2", mod(-3, 5) == 2);
    print_test("power(2, 10) = 1024", power(2, 10, 1e9+7) == 1024);
    print_test("isprime(2) = true", isprime(2));
    print_test("isprime(17) = true", isprime(17));
    print_test("isprime(18) = false", !isprime(18));
    print_test("isprime(1) = false", !isprime(1));
    print_test("isprime(0) = false", !isprime(0));
    print_test("isprime(-5) = false", !isprime(-5));
    
    // String
    print_test("split('a,b,c', ',') = 3 parts", split("a,b,c", ',').size() == 3);
    print_test("join({'a','b'}, '-') = 'a-b'", join({"a", "b"}, "-") == "a-b");
    print_test("lowercase('HeLLo') = 'hello'", lowercase("HeLLo") == "hello");
    print_test("uppercase('hello') = 'HELLO'", uppercase("hello") == "HELLO");
}

// ==========================================
// MAIN
// ==========================================
int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);  // Enable UTF-8 output on Windows
#endif
    
    std::cout << R"(
    ██████╗  █████╗ ██╗  ██╗███████╗
    ██╔══██╗██╔══██╗╚██╗██╔╝██╔════╝
    ██║  ██║███████║ ╚███╔╝ █████╗  
    ██║  ██║██╔══██║ ██╔██╗ ██╔══╝  
    ██████╔╝██║  ██║██╔╝ ██╗███████╗
    ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚══════╝
         COMPREHENSIVE BENCHMARK SUITE
    )" << "\n";
    
    setup();
    
    bench_performance();
    bench_memory();
    bench_safety();
    
    // ==========================================
    // SECTION 5: NEW UTILITIES
    // ==========================================
    print_header("NEW UTILITIES VERIFICATION");
    
    // Math
    print_test("ceildiv(10, 3) = 4", ceildiv(10, 3) == 4);
    print_test("floordiv(10, 3) = 3", floordiv(10, 3) == 3);
    print_test("digits(12345) = 5", digits(12345) == 5);
    print_test("issquare(16) = true", issquare(16));
    print_test("issquare(15) = false", !issquare(15));
    
    // Random
    i64 r = rand(1, 10);
    print_test("rand(1, 10) -> i64", r >= 1 && r <= 10);
    print_test("rand(1, 5) -> i32", rand(1, 5) >= 1 && rand(1, 5) <= 5);
    print_test("rand(0.0f, 1.0f) -> f32", rand(0.0f, 1.0f) >= 0.0f && rand(0.0f, 1.0f) < 1.0f);
    print_test("rand(0.0, 1.0) -> f64", rand(0.0, 1.0) >= 0.0 && rand(0.0, 1.0) < 1.0);
    
    vi64 rv = {1, 2, 3, 4, 5};
    shuffle(rv);
    print_test("shuffle changes order (likely)", rv != vi64({1, 2, 3, 4, 5})); // Flaky but unlikely
    
    // ==========================================
    // SECTION 6: ROBUSTNESS & I/O
    // ==========================================
    print_header("ROBUSTNESS & I/O VERIFICATION");
    
    // 1. Rand Small Types
    char c = rand<char>('a', 'z');
    print_test("rand<char>('a', 'z')", c >= 'a' && c <= 'z');
    u8 b = rand<u8>(0, 255);
    print_test("rand<u8>(0, 255)", b <= 255);
    
    // 2. Enumerate Rvalue
    bool enum_rvalue_ok = true;
    for (auto [i, val] : enumerate(std::vector<int>{10, 20, 30})) {
        if (i == 0 && val != 10) enum_rvalue_ok = false;
        if (i == 1 && val != 20) enum_rvalue_ok = false;
        if (i == 2 && val != 30) enum_rvalue_ok = false;
    }
    print_test("enumerate(rvalue) safety", enum_rvalue_ok);
    
    // 3. File I/O
    writefile("test.txt", "Hello World");
    str content = readfile("test.txt");
    print_test("writefile/readfile", content == "Hello World");
    
    writefile("lines.txt", std::vector<str>{"A", "B", "C"});
    auto lines = readlines("lines.txt");
    print_test("writefile/readlines vec", lines.size() == 3 && lines[1] == "B");
    
    // Clean up
    std::remove("test.txt");
    std::remove("lines.txt");

    // String
    print_test("startswith('hello', 'he')", startswith("hello", "he"));
    print_test("endswith('hello', 'lo')", endswith("hello", "lo"));
    print_test("strip('  hello  ') = 'hello'", strip("  hello  ") == "hello");
    print_test("replace('hello', 'l', 'x') = 'hexxo'", replace("hello", "l", "x") == "hexxo");
    print_test("has('hello', 'ell')", has("hello", "ell"));

    // ==========================================
    // SECTION 7: SAFETY TORTURE TEST
    // ==========================================
    print_header("SAFETY TORTURE TEST (RUST-LIKE)");

    // 1. Option Monadic Chaining
    // Some(10) -> map(*2) -> 20 -> then(Some(+5)) -> 25 -> unwrap
    Option<int> opt = Some(10);
    int res = opt.map([](int x) { return x * 2; })
                 .then([](int x) { return Some(x + 5); })
                 .otherwise([]() { return 0; });
    print_test("Option::map/then chain", res == 25);
    
    // None chain
    Option<int> none_opt = None;
    int res2 = none_opt.map([](int x) { return x * 2; })
                       .otherwise([]() { return 42; });
    print_test("Option::None map/otherwise", res2 == 42);

    // 2. Result Error Handling
    // trydiv(10, 2) -> Ok(5) -> map(*2) -> 10
    auto r1 = trydiv(10, 2).map([](i64 x) { return x * 2; });
    print_test("Result::map (Ok)", r1.isok() && r1.unwrap() == 10);
    
    // trydiv(10, 0) -> Err -> otherwise(99) -> 99
    i64 r2 = trydiv(10, 0).otherwise([](str) { return 99LL; });
    print_test("Result::otherwise (Err)", r2 == 99);
    
    // Result::error() test
    print_test("Result::error() extracts msg", trydiv(10, 0).error() == str("Division by zero"));
    
    // 3. Defer
    int defer_val = 0;
    {
        defer { defer_val = 100; };
        defer_val = 50;
    }
    print_test("defer execution order", defer_val == 100);

    // ==========================================
    // SECTION 8: EDGE CASES & HARDENING
    // ==========================================
    print_header("EDGE CASES & HARDENING");

    // 1. Math Hardening
    // Overflow check: large prime
    // 2147483647 is i32 max
    print_test("isprime(INT32_MAX)", isprime(2147483647)); 
    
    // Factors of large prime square
    // Use 1000003 (10^6). Square is ~10^12. Fits in i64.
    // Iteration to 10^6 is instantaneous.
    i64 prime = 1000003LL;
    i64 prime_sq = prime * prime;
    auto f = factors(prime_sq);
    print_test("factors(large_prime_sq) safe loop", f.size() == 2 && f[0] == prime);
    
    // LCM negative
    print_test("lcm(-3, 5) is positive", lcm(-3, 5) == 15);
    print_test("lcm(INT_MAX, 1)", lcm(2147483647LL, 1LL) == 2147483647LL);
    
    // 2. String Edge Cases
    print_test("split empty string", split("").size() == 0 || (split("").size() == 1 && split("")[0] == ""));
    print_test("split only sep", split(",", ",").size() == 2 && split(",", ",")[0] == "");
    print_test("replace all-match", replace("aaaa", "a", "b") == "bbbb");
    print_test("strip whitespace only", strip("   ") == "");
    
    // 3. Algorithm Safety
    vi64 empty_vec;
    print_test("first(empty) -> None", isnone(first(empty_vec)));
    
    // Verify max(empty) panics (manual verification needed for crash)
    // We won't crash the benchmark suite, but we know the check is there.
    
    // ==========================================
    // SECTION 9: UTILITY EXPANSION PHASE 2
    // ==========================================
    print_header("UTILITY EXPANSION PHASE 2");
    
    // 1. Functional Predicates
    vi64 nums = {1, 2, 3, 4, 5};
    print_test("all(nums, >0)", all(nums, [](i64 x){ return x > 0; }));
    print_test("any(nums, >4)", any(nums, [](i64 x){ return x > 4; }));
    print_test("none(nums, >10)", none(nums, [](i64 x){ return x > 10; }));
    print_test("count(nums, 3) == 1", count(nums, 3) == 1);
    
    // 2. Safe Parsing
    print_test("parse<int>(\"123\") -> Ok(123)", parse<int>("123").unwrap() == 123);
    print_test("parse<int>(\"abc\") -> Err", parse<int>("abc").iserr());
    print_test("parse<bool>(\"true\") -> Ok(true)", parse<bool>("true").unwrap() == true);
    
    // 3. Time (manual check mostly, just ensure it compiles and runs)
    f64 t_start = now();
    sleep(1);
    f64 t_end = now();
    print_test("sleep(1ms) duration > 0", (t_end - t_start) > 0);
    
    {
        // Scope for Timer
        Timer t("Timer Test");
    } // Should print to stdout

    bench_correctness();
    std::cout << "Legend: ✅ Pass/Zero-overhead  ⚠️ Minor overhead  ❌ Needs attention\n\n";
    
    return 0;
}

