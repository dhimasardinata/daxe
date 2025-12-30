/*
 * DAXE - EXAMPLE USAGE
 * D.A's Axe - Cut through C++ verbosity
 * 
 * Build: cx run example
 */

#include <daxe.h>

int main() {
    // Read input
    Int n;
    input(n);
    vi64 v = readvec<i64>(n);
    
    // Universal functions work on any container
    print("Max:", max(v));
    print("Min:", min(v));
    print("Sum:", sum(v));
    
    // Safe access with Option
    if (auto x = first(v); issome(x)) {
        print("First element:", unwrap(x));
    }
    
    // Python-like range
    for (Int i : range(n)) {
        if (isprime(v[i])) {
            print("Prime at index", i, ":", v[i]);
        }
    }
    
    // Pythonic containers
    List<Int> primes = List<Int>(v).filter([](Int x) { return isprime(x); });
    print("Primes:", primes);
    
    // Math
    print("GCD(12, 8):", gcd(12, 8));
    print("LCM(12, 8):", lcm(12, 8));
    
    return 0;
}
