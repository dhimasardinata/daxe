/*
 * DAXE - SAFETY TESTS
 * 
 * Build: cx run safety_test
 */

#include <daxe.h>
#include <daxe/safe.h>
#include <cassert>

using namespace dax;

#define TEST(name, expr) \
    do { \
        bool passed = (expr); \
        std::cout << (passed ? "✅" : "❌") << " " << name << "\n"; \
        if (!passed) failures++; \
    } while(0)

int failures = 0;

void test_option() {
    std::cout << "\n=== Option<T> Tests ===\n";
    
    Option<i64> some = Some(42);
    Option<i64> none = None;
    
    TEST("Some has value", issome(some));
    TEST("None has no value", isnone(none));
    TEST("unwrap returns value", unwrap(some) == 42);
    TEST("valueor with Some", valueor(some, 0LL) == 42);
    TEST("valueor with None", valueor(none, 99LL) == 99);
}

void test_safe_access() {
    std::cout << "\n=== Safe Access Tests ===\n";
    
    vi64 v = {1, 2, 3, 4, 5};
    
    TEST("getat(0) works", valueor(getat(v, 0LL), -1LL) == 1);
    TEST("getat(-1) works (negative index)", valueor(getat(v, -1LL), -1LL) == 5);
    TEST("getat(100) returns None", isnone(getat(v, 100LL)));
    TEST("getor with valid index", getor(v, 2LL, -1LL) == 3);
    TEST("getor with invalid index", getor(v, 100LL, -1LL) == -1);
    TEST("first returns first", valueor(first(v), -1LL) == 1);
    TEST("last returns last", valueor(last(v), -1LL) == 5);
    
    vi64 empty_v;
    TEST("first of empty is None", isnone(first(empty_v)));
    TEST("last of empty is None", isnone(last(empty_v)));
}

void test_bounds() {
    std::cout << "\n=== Bounds Tests ===\n";
    
    TEST("inbounds(5, 0, 10) = true", inbounds(5, 0, 10));
    TEST("inbounds(10, 0, 10) = false", !inbounds(10, 0, 10));
    TEST("inbounds(-1, 0, 10) = false", !inbounds(-1, 0, 10));
    
    TEST("ingrid(0,0,5,5) = true", ingrid(0, 0, 5, 5));
    TEST("ingrid(4,4,5,5) = true", ingrid(4, 4, 5, 5));
    TEST("ingrid(5,0,5,5) = false", !ingrid(5, 0, 5, 5));
    TEST("ingrid(-1,0,5,5) = false", !ingrid(-1, 0, 5, 5));
    
    TEST("clamp(5, 0, 10) = 5", clamp(5, 0, 10) == 5);
    TEST("clamp(-5, 0, 10) = 0", clamp(-5, 0, 10) == 0);
    TEST("clamp(15, 0, 10) = 10", clamp(15, 0, 10) == 10);
}

void test_safe_math() {
    std::cout << "\n=== Safe Math Tests ===\n";
    
    auto div_ok = trydiv(10, 2);
    auto div_err = trydiv(10, 0);
    
    TEST("trydiv(10, 2) is ok", div_ok.isok());
    TEST("trydiv(10, 2) = 5", div_ok.unwrap() == 5);
    TEST("trydiv(10, 0) is err", div_err.iserr());
    
    auto sqrt_ok = trysqrt(16.0);
    auto sqrt_err = trysqrt(-1.0);
    
    TEST("trysqrt(16) is ok", sqrt_ok.isok());
    TEST("trysqrt(-1) is err", sqrt_err.iserr());
}

void test_universal_functions() {
    std::cout << "\n=== Universal Functions Tests ===\n";
    
    vi64 v = {3, 1, 4, 1, 5, 9, 2, 6};
    si64 s = {3, 1, 4, 1, 5, 9, 2, 6};
    
    TEST("max(vector)", max(v) == 9);
    TEST("max(set)", max(s) == 9);
    TEST("min(vector)", min(v) == 1);
    TEST("min(set)", min(s) == 1);
    TEST("sum(vector)", sum(v) == 31);
    TEST("sum(set)", sum(s) == 30); // set deduplicates
    TEST("has(vector, 5)", has(v, 5LL));
    TEST("has(set, 5)", has(s, 5LL));
    TEST("!has(vector, 99)", !has(v, 99LL));
    TEST("len(vector)", len(v) == 8);
    TEST("len(set)", len(s) == 7); // unique elements
}

void test_math() {
    std::cout << "\n=== Math Tests ===\n";
    
    TEST("gcd(12, 8) = 4", gcd(12, 8) == 4);
    TEST("lcm(12, 8) = 24", lcm(12, 8) == 24);
    TEST("isprime(2) = true", isprime(2));
    TEST("isprime(17) = true", isprime(17));
    TEST("isprime(18) = false", !isprime(18));
    TEST("power(2, 10) = 1024", power(2, 10, 1000000007) == 1024);
    TEST("mod(-3, 5) = 2", mod(-3, 5) == 2);
}

int main() {
    std::cout << "╔═══════════════════════════════════════╗\n";
    std::cout << "║        DAXE SAFETY TEST SUITE         ║\n";
    std::cout << "╚═══════════════════════════════════════╝\n";
    
    test_option();
    test_safe_access();
    test_bounds();
    test_safe_math();
    test_universal_functions();
    test_math();
    
    std::cout << "\n" << std::string(40, '=') << "\n";
    if (failures == 0) {
        std::cout << "✅ All tests passed!\n\n";
        return 0;
    } else {
        std::cout << "❌ " << failures << " tests failed!\n\n";
        return 1;
    }
}
