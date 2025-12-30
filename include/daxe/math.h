/*
 * DAXE - MATH UTILITIES
 * D.A's Axe - Cut through C++ verbosity
 * 
 * Naming: Flat style (no snake_case)
 */

#ifndef DAXE_MATH_H
#define DAXE_MATH_H

#include "base.h"
#include "macros.h"
#include <cmath>
#include <numeric>
#include <algorithm>
#include <limits>
#include <vector>
#include <map>
#include <tuple>
#include "safe.h"

DAXE_NAMESPACE_BEGIN

// ==========================================
// BASIC MATH
// ==========================================
template <typename T> DAXE_NODISCARD constexpr bool iseven(T n) noexcept { return n % 2 == 0; }
template <typename T> DAXE_NODISCARD constexpr bool isodd(T n) noexcept { return n % 2 != 0; }
template <typename T> DAXE_NODISCARD constexpr bool ispositive(T n) noexcept { return n > 0; }
template <typename T> DAXE_NODISCARD constexpr bool isnegative(T n) noexcept { return n < 0; }
template <typename T> DAXE_NODISCARD constexpr bool iszero(T n) noexcept { return n == 0; }

DAXE_NODISCARD constexpr i64 mod(i64 x, i64 m) noexcept { return ((x % m) + m) % m; }

// Core utilities (clamp is in safe.h)
template <typename T>
DAXE_NODISCARD constexpr T lerp(T a, T b, f64 t) noexcept {
    return static_cast<T>(a + t * (b - a));
}

template <typename T>
DAXE_NODISCARD constexpr int sign(T n) noexcept {
    return (n > T{}) - (n < T{});
}

template <typename T>
DAXE_NODISCARD constexpr T absval(T n) noexcept {
    if constexpr (std::is_same_v<T, i64>) {
        if (n == std::numeric_limits<i64>::min()) return std::numeric_limits<i64>::max();
    }
    return n < T{} ? -n : n;
}

DAXE_NODISCARD constexpr i64 gcd(i64 a, i64 b) noexcept {
    while (b) { a %= b; std::swap(a, b); }
    return a < 0 ? -a : a;
}

DAXE_NODISCARD constexpr i64 lcm(i64 a, i64 b) noexcept { 
    if (a == 0 || b == 0) return 0;
    i64 temp = (a / gcd(a, b)) * b;
    return temp < 0 ? -temp : temp;
}

DAXE_NODISCARD inline i64 power(i64 base, i64 exp, i64 m = MOD) noexcept {
    i64 res = 1;
    base %= m; if (base < 0) base += m;
    while (exp > 0) {
        if (exp & 1) DAXE_LIKELY res = res * base % m;
        base = base * base % m;
        exp >>= 1;
    }
    return res;
}

// binarypower - binary exponentiation without modulo (with overflow saturation)
DAXE_NODISCARD inline i64 binarypower(i64 base, i64 exp) noexcept {
    if (exp < 0) return 0;  // Negative exponent gives 0 for integers
    if (exp == 0) return 1;
    if (base == 0) return 0;
    if (base == 1) return 1;
    if (base == -1) return (exp & 1) ? -1 : 1;
    
    i64 res = 1;
    constexpr i64 MAX = std::numeric_limits<i64>::max();
    bool overflow = false;
    
    while (exp > 0) {
        if (exp & 1) {
            if (!overflow && absval(base) != 0 && absval(res) > MAX / absval(base)) {
                overflow = true;
            }
            if (!overflow) res *= base;
        }
        exp >>= 1;
        if (exp > 0 && !overflow) {
            if (absval(base) > MAX / absval(base)) {
                overflow = true;
            } else {
                base *= base;
            }
        }
    }
    return overflow ? MAX : res;
}

// Extended Euclidean Algorithm
// Returns {gcd, x, y} such that ax + by = gcd
DAXE_NODISCARD constexpr std::tuple<i64, i64, i64> extendedgcd(i64 a, i64 b) noexcept {
    i64 s = 0, old_s = 1;
    i64 r = b, old_r = a;
    while (r != 0) {
        i64 quotient = old_r / r;
        i64 temp = old_r; old_r = r; r = temp - quotient * r;
        temp = old_s; old_s = s; s = temp - quotient * s;
    }
    i64 y = (b == 0) ? 0 : (old_r - old_s * a) / b;
    return {old_r, old_s, y};
}

// Modular Inverse using Extended Euclidean Algorithm
// Works for any a, m (coprime). Returns 0 if inverse does not exist.
DAXE_NODISCARD inline i64 modinv(i64 a, i64 m = MOD) noexcept {
    auto [g, x, y] = extendedgcd(a, m);
    if (g != 1) return 0; // No inverse exists
    return (x % m + m) % m;
}

// ==========================================
// NUMBER PROPERTIES (Forward Declared for Compile-Time Checks)
// ==========================================
DAXE_NODISCARD constexpr bool isprime(i64 n) noexcept {
    if (n < 2) DAXE_UNLIKELY return false;
    if (n == 2 || n == 3) DAXE_LIKELY return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (i64 i = 5; i <= n / i; i += 6)
        if (n % i == 0 || n % (i + 2) == 0) return false;
    return true;
}

// ==========================================
// MODINT CLASS
// ==========================================
template <i64 M = MOD>
class Modint {
    i64 val;
public:
    constexpr Modint() noexcept : val(0) {}
    constexpr Modint(i64 v) noexcept : val((v % M + M) % M) {}
    
    constexpr i64 value() const noexcept { return val; }
    
    constexpr Modint operator+(const Modint& o) const noexcept { return Modint((val + o.val) % M); }
    constexpr Modint operator-(const Modint& o) const noexcept { return Modint((val - o.val + M) % M); }
    constexpr Modint operator*(const Modint& o) const noexcept { return Modint((val * o.val) % M); }
    constexpr Modint operator/(const Modint& o) const noexcept { return *this * o.inv(); }
    
    constexpr Modint& operator+=(const Modint& o) noexcept { return *this = *this + o; }
    constexpr Modint& operator-=(const Modint& o) noexcept { return *this = *this - o; }
    constexpr Modint& operator*=(const Modint& o) noexcept { return *this = *this * o; }
    constexpr Modint& operator/=(const Modint& o) noexcept { return *this = *this / o; }
    
    constexpr Modint pow(i64 exp) const noexcept {
        Modint res(1), base = *this;
        while (exp > 0) {
            if (exp & 1) res *= base;
            base *= base;
            exp >>= 1;
        }
        return res;
    }
    
    constexpr Modint inv() const noexcept {
        if constexpr (isprime(M)) {
            return pow(M - 2);
        } else {
            return Modint(dax::modinv(val, M));
        }
    }
    
    constexpr bool operator==(const Modint& o) const noexcept { return val == o.val; }
    constexpr bool operator!=(const Modint& o) const noexcept { return val != o.val; }
    
    friend std::ostream& operator<<(std::ostream& os, const Modint& m) { return os << m.val; }
};

using Mint = Modint<MOD>;

// ==========================================
// COMBINATORICS
// ==========================================

// Factorial with memoization
DAXE_NODISCARD inline Mint factorial(i64 n) {
    if (n < 0) return Mint(0);  // Guard against negative input
    if (n > 10'000'000) panic("factorial cache limit exceeded (max 10,000,000)");
    static std::vector<Mint> cache = {Mint(1)};
    while (static_cast<i64>(cache.size()) <= n) {
        cache.push_back(cache.back() * static_cast<i64>(cache.size()));
    }
    return cache[static_cast<size_t>(n)];
}

// Inverse factorial with memoization
DAXE_NODISCARD inline Mint invfactorial(i64 n) {
    if (n < 0) return Mint(0);  // Guard against negative input
    static std::vector<Mint> cache;
    if (cache.empty()) cache.push_back(Mint(1));
    while (static_cast<i64>(cache.size()) <= n) {
        cache.push_back(factorial(static_cast<i64>(cache.size())).inv());
    }
    return cache[static_cast<size_t>(n)];
}

// combinations (n choose r)
DAXE_NODISCARD inline Mint combinations(i64 n, i64 r) {
    if (r < 0 || r > n) return Mint(0);
    return factorial(n) * invfactorial(r) * invfactorial(n - r);
}

// permutations (n permute r)
DAXE_NODISCARD inline Mint permutations(i64 n, i64 r) {
    if (r < 0 || r > n) return Mint(0);
    return factorial(n) * invfactorial(n - r);
}

// ==========================================
// DIVISION
// ==========================================
DAXE_NODISCARD constexpr i64 ceildiv(i64 a, i64 b) noexcept {
    if (b == 0) return 0;  // Avoid division by zero
    i64 d = a / b;
    i64 r = a % b;
    // Round up if there's a remainder and signs are the same (positive result)
    if (r != 0 && ((a > 0) == (b > 0))) d++;
    return d;
}

DAXE_NODISCARD constexpr i64 floordiv(i64 a, i64 b) noexcept {
    return a / b;
}

// Safe division - returns 0 if divisor is 0
DAXE_NODISCARD constexpr i64 safediv(i64 a, i64 b) noexcept {
    return b == 0 ? 0 : a / b;
}

// Safe modulo - returns 0 if divisor is 0
DAXE_NODISCARD constexpr i64 safemod(i64 a, i64 b) noexcept {
    return b == 0 ? 0 : a % b;
}

// Saturating addition - clamps to max/min on overflow
DAXE_NODISCARD constexpr i64 satadd(i64 a, i64 b) noexcept {
    if (b > 0 && a > std::numeric_limits<i64>::max() - b) return std::numeric_limits<i64>::max();
    if (b < 0 && a < std::numeric_limits<i64>::min() - b) return std::numeric_limits<i64>::min();
    return a + b;
}

// Saturating subtraction
DAXE_NODISCARD constexpr i64 satsub(i64 a, i64 b) noexcept {
    if (b < 0 && a > std::numeric_limits<i64>::max() + b) return std::numeric_limits<i64>::max();
    if (b > 0 && a < std::numeric_limits<i64>::min() + b) return std::numeric_limits<i64>::min();
    return a - b;
}

// Check if value is in range [lo, hi)
template <typename T>
DAXE_NODISCARD constexpr bool inrange(T val, T lo, T hi) noexcept {
    return val >= lo && val < hi;
}

// Map value from one range to another
DAXE_NODISCARD constexpr f64 mapvalue(f64 val, f64 fromlo, f64 fromhi, f64 tolo, f64 tohi) noexcept {
    f64 range = fromhi - fromlo;
    if (range == 0.0) return tolo;  // Avoid division by zero
    return tolo + (val - fromlo) * (tohi - tolo) / range;
}

// Wrap value to range [lo, hi)
template <typename T>
DAXE_NODISCARD constexpr T wrapvalue(T val, T lo, T hi) noexcept {
    T range = hi - lo;
    return lo + ((val - lo) % range + range) % range;
}

// ==========================================
// NUMBER PROPERTIES
// ==========================================

DAXE_NODISCARD inline bool issquare(i64 n) noexcept {
    if (n < 0) return false;
    if (n == 0) return true;
    i64 root = static_cast<i64>(std::sqrt(static_cast<double>(n)));
    // Check root and root+1 due to floating-point imprecision
    if (root * root == n) return true;
    if ((root + 1) * (root + 1) == n) return true;
    return false;
}

DAXE_NODISCARD inline i64 digits(i64 n) noexcept {
    if (n == 0) return 1;
    return static_cast<i64>(std::log10(std::abs(n))) + 1;
}

// ==========================================
// FACTORIZATION
// ==========================================
DAXE_NODISCARD inline std::vector<i64> factors(i64 n) {
    std::vector<i64> f; f.reserve(64);
    if (n <= 1) return f;  // Handle 0, 1, and negative numbers
    for (i64 i = 2; i * i <= n; ++i)
        while (n % i == 0) { f.push_back(i); n /= i; }
    if (n > 1) f.push_back(n);
    return f;
}

DAXE_NODISCARD inline std::vector<i64> divisors(i64 n) {
    std::vector<i64> r; r.reserve(128);
    if (n <= 0) return r;  // Guard against 0 and negative numbers
    for (i64 i = 1; i * i <= n; ++i) {
        if (n % i == 0) { r.push_back(i); if (i != n / i) r.push_back(n / i); }
    }
    std::sort(r.begin(), r.end());
    return r;
}

// Bit manipulation - portable implementations
#if defined(_MSC_VER)
    #include <intrin.h>
    DAXE_NODISCARD inline i64 bitcount(u64 n) noexcept { return static_cast<i64>(__popcnt64(n)); }
    DAXE_NODISCARD inline i64 trailingzeros(u64 n) noexcept {
        if (n == 0) return 64;
        unsigned long index;
        _BitScanForward64(&index, n);
        return static_cast<i64>(index);
    }
    DAXE_NODISCARD inline i64 leadingzeros(u64 n) noexcept {
        if (n == 0) return 64;
        unsigned long index;
        _BitScanReverse64(&index, n);
        return static_cast<i64>(63 - index);
    }
#else
    DAXE_NODISCARD inline i64 bitcount(u64 n) noexcept { return static_cast<i64>(__builtin_popcountll(n)); }
    DAXE_NODISCARD inline i64 trailingzeros(u64 n) noexcept { return n == 0 ? 64 : static_cast<i64>(__builtin_ctzll(n)); }
    DAXE_NODISCARD inline i64 leadingzeros(u64 n) noexcept { return n == 0 ? 64 : static_cast<i64>(__builtin_clzll(n)); }
#endif
DAXE_NODISCARD inline bool ispow2(u64 n) noexcept { return n && !(n & (n - 1)); }
DAXE_NODISCARD inline u64 nextpow2(u64 n) noexcept { if (n == 0) return 1; return 1ULL << (64 - leadingzeros(n - 1)); }
DAXE_NODISCARD inline i64 highestbit(u64 n) noexcept { return n == 0 ? -1 : 63 - leadingzeros(n); }
DAXE_NODISCARD inline i64 lowestbit(u64 n) noexcept { return n == 0 ? -1 : trailingzeros(n); }

// ==========================================
// STATISTICS
// ==========================================
template <typename Container>
DAXE_NODISCARD inline f64 mean(const Container& c) {
    if (std::empty(c)) return 0.0;
    f64 sum = 0.0;
    for (const auto& x : c) sum += static_cast<f64>(x);
    return sum / static_cast<f64>(std::size(c));
}

template <typename Container>
DAXE_NODISCARD inline f64 median(Container c) { // Pass by value to modify copy
    if (std::empty(c)) return 0.0;
    size_t n = std::size(c);
    size_t mid = n / 2;
    
    // O(n) partial sort using nth_element
    std::nth_element(std::begin(c), std::begin(c) + mid, std::end(c));
    
    if (n % 2 == 1) {
        return static_cast<f64>(c[mid]);
    } else {
        // For even count, need max of lower half
        auto maxlower = *std::max_element(std::begin(c), std::begin(c) + mid);
        return (static_cast<f64>(maxlower) + static_cast<f64>(c[mid])) / 2.0;
    }
}

template <typename Container>
DAXE_NODISCARD inline auto mode(const Container& c) -> typename Container::value_type {
    if (std::empty(c)) return {};
    std::map<typename Container::value_type, i64> counts;
    for (const auto& x : c) counts[x]++;
    
    i64 max_count = -1;
    typename Container::value_type result = {};
    
    for (const auto& [val, count] : counts) {
        if (count > max_count) {
            max_count = count;
            result = val;
        }
    }
    return result;
}

template <typename Container>
DAXE_NODISCARD inline f64 variance(const Container& c) {
    if (c.size() < 2) return 0.0;
    f64 m = mean(c);
    f64 sum_sq_diff = 0.0;
    for (const auto& x : c) {
        f64 diff = static_cast<f64>(x) - m;
        sum_sq_diff += diff * diff;
    }
    // Sample variance (n-1) - matches Python's statistics.variance()
    return sum_sq_diff / static_cast<f64>(c.size() - 1);
}

template <typename Container>
DAXE_NODISCARD inline f64 stddev(const Container& c) {
    return std::sqrt(variance(c));
}

DAXE_NAMESPACE_END

#endif // DAXE_MATH_H
