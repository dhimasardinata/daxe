#include <daxe.h>
#include <daxe/english.h>

// ==========================================
// HELPER: Section printer
// ==========================================
fx section(const str& title) {
    print("\n=== " + title + " ===");
}

// ==========================================
// MAIN SHOWCASE
// ==========================================
int main() {
    print("╔══════════════════════════════════════════════╗");
    print("║           DAXE FULL SHOWCASE                 ║");
    print("║  90+ utilities for modern C++ development    ║");
    print("╚══════════════════════════════════════════════╝");
    
    // ========================================
    section("1. TYPE ALIASES");
    i64 bignum = 1234567890123LL;
    f64 pi = 3.14159265358979;
    str greeting = "Hello, DAXE!";
    print("i64:", bignum, "f64:", pi);
    print("str:", greeting);
    
    // ========================================
    section("2. PYTHONIC CONTAINERS");
    List nums = {1, 2, 3, 4, 5};
    nums.append(6);
    print("List:", nums);
    print("  Sum:", nums.sum(), "Max:", nums.max(), "Min:", nums.min());
    print("  Safe access: nums.getat(10) =", nums.getat(10).value_or(-1));
    
    Dict<str, i64> scores;
    scores["alice"] = 100;
    scores["bob"] = 85;
    scores.setdefault("charlie", 90);
    print("Dict:", scores);
    print("  Get with default:", scores.get("unknown", -1));
    
    Set<i64> primes = {2, 3, 5, 7, 11};
    primes.add(13);
    print("Set:", primes.tolist());
    print("  Has 7?", primes.has(7));
    
    // ========================================
    section("3. FUNCTIONAL UTILITIES");
    vi64 data = {10, -5, 20, -15, 30};
    print("Data:", data);
    
    var sorted_data = sorted(data);
    print("  Sorted:", sorted_data);
    
    var filtered_data = filtered(data, [](i64 x) { return x > 0; });
    print("  Positive only:", filtered_data);
    
    var prefix = prefixsum(nums);
    print("  Prefix sum of nums:", prefix);
    
    print("  Argmax:", argmax(data), "Argmin:", argmin(data));
    
    // ========================================
    section("4. MATH UTILITIES");
    print("gcd(48, 18):", gcd(48, 18));
    print("lcm(12, 8):", lcm(12, 8));
    print("power(2, 10, MOD):", power(2, 10, MOD));
    print("isprime(1000003):", isprime(1000003));
    
    print("Mean:", mean(nums), "Median:", median(nums));
    print("combinations(10, 3):", combinations(10, 3));
    print("permutations(5, 2):", permutations(5, 2));
    
    // Safety math
    print("safediv(10, 0):", safediv(10, 0));
    print("satadd(MAX, 100):", satadd(std::numeric_limits<i64>::max() - 10, 100));
    print("clamp(150, 0, 100):", clamp(150LL, 0LL, 100LL));
    print("inrange(5, 0, 10):", inrange(5, 0, 10));
    
    // ========================================
    section("5. BIT MANIPULATION");
    u64 bits = 0b10110100;
    print("Value:", bits, "(binary: 10110100)");
    print("  bitcount:", bitcount(bits));
    print("  highestbit:", highestbit(bits));
    print("  lowestbit:", lowestbit(bits));
    print("  ispow2(64):", ispow2(64ULL));
    print("  nextpow2(100):", nextpow2(100ULL));
    
    // ========================================
    section("6. MODULAR ARITHMETIC");
    Mint a = 1000000006;
    Mint b = 2;
    print("Mint a = 1000000006 (mod 1e9+7):", a);
    print("  a + b =", a + b);
    print("  a * b =", a * b);
    print("  a.pow(2) =", a.pow(2));
    print("  b.inv() =", b.inv());
    
    // ========================================
    section("7. STRING UTILITIES");
    str text = "  hello world  ";
    print("Original:", "'" + text + "'");
    print("  strip:", "'" + strip(text) + "'");
    print("  lstrip:", "'" + lstrip(text) + "'");
    print("  capitalize:", capitalize(strip(text)));
    print("  title:", title(strip(text)));
    print("  split:", split("a,b,c", ","));
    
    // ========================================
    section("8. DATA STRUCTURES");
    
    // Fenwick Tree
    FenwickTree ft(10);
    ft.update(3, 5);
    ft.update(5, 10);
    ft.update(7, 3);
    print("FenwickTree:");
    print("  Query [0..7]:", ft.query(7));
    print("  Range [3..5]:", ft.rangequery(3, 5));
    
    // DSU (Union-Find)
    DSU dsu(5);
    dsu.unite(0, 1);
    dsu.unite(2, 3);
    dsu.unite(1, 2);
    print("DSU:");
    print("  connected(0, 3):", dsu.connected(0, 3));
    print("  connected(0, 4):", dsu.connected(0, 4));
    print("  components:", dsu.components());
    
    // Graph
    Graph g = makegraph(4);
    addedge(g, 0, 1);
    addedge(g, 1, 2);
    adddirected(g, 2, 3);
    print("Graph:");
    print("  Nodes:", nodecount(g), "Degree[1]:", degree(g, 1));
    
    // ========================================
    section("9. GRID UTILITIES");
    i64 rows = 5, cols = 5;
    print("Grid 5x5:");
    print("  inbounds(2, 3):", inbounds(2, 3, rows, cols));
    print("  inbounds(5, 5):", inbounds(5, 5, rows, cols));
    print("  manhattan(0,0 -> 3,4):", manhattan(0, 0, 3, 4));
    print("  neighbors4(2,2):", neighbors4(2, 2, rows, cols));
    print("  toindex(2,3):", toindex(2, 3, cols));
    print("  tocoord(13):", tocoord(13, cols));
    
    // ========================================
    section("10. SAFETY TYPES");
    Option<i64> maybe = Some(42LL);
    print("Option Some(42):", maybe.value_or(-1));
    
    Option<i64> nothing = None;
    print("Option None:", nothing.value_or(-1));
    
    // Safe access chaining
    List data2 = {100, 200, 300};
    print("Safe chained access:");
    print("  data2.getat(1):", data2.getat(1).value_or(-1));
    print("  data2.getat(99):", data2.getat(99).value_or(-1));
    
    // ========================================
    section("11. DEBUGGING");
    ensure(2 + 2 == 4, "Math is broken!");
    
    if (expect(nums.size() > 0)) {
        print("Expectation passed!");
    }
    
    // ========================================
    print("\n╔══════════════════════════════════════════════╗");
    print("║           SHOWCASE COMPLETE                  ║");
    print("╚══════════════════════════════════════════════╝");
    
    return 0;
}
