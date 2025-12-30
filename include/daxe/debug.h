/*
 * DAXE - UNIVERSAL DEBUGGER
 * D.A's Axe - Cut through C++ verbosity
 * 
 * Naming: Flat style (no snake_case)
 */

#ifndef DAXE_DEBUG_H
#define DAXE_DEBUG_H

#include "containers.h"
#include "macros.h"

DAXE_NAMESPACE_BEGIN

// stringify conversions
DAXE_NODISCARD inline str stringify(const str& s) { return '"' + s + '"'; }
DAXE_NODISCARD inline str stringify(char c) { return "'" + str(1, c) + "'"; }
DAXE_NODISCARD inline str stringify(const char* s) { return stringify(str(s)); }
DAXE_NODISCARD inline str stringify(bool b) { return b ? "true" : "false"; }
template <typename T> DAXE_NODISCARD inline str stringify(T x) { return std::to_string(x); }

#if DAXE_HAS_INT128
DAXE_NODISCARD inline str stringify(i128 n) {
    if (n == 0) return "0";
    str s; 
    bool neg = n < 0;
    // Use unsigned to avoid overflow on MIN value
    u128 un = neg ? static_cast<u128>(-(n + 1)) + 1 : static_cast<u128>(n);
    while (un > 0) { s += static_cast<char>('0' + un % 10); un /= 10; }
    if (neg) s += '-';
    std::reverse(s.begin(), s.end());
    return s;
}

DAXE_NODISCARD inline str stringify(u128 n) {
    if (n == 0) return "0";
    str s;
    while (n > 0) { s += static_cast<char>('0' + n % 10); n /= 10; }
    std::reverse(s.begin(), s.end());
    return s;
}
#endif

template <typename A, typename B>
DAXE_NODISCARD inline str stringify(const std::pair<A, B>& p) { return "(" + stringify(p.first) + ", " + stringify(p.second) + ")"; }

template <typename T>
DAXE_NODISCARD inline str stringify(const std::vector<T>& v) {
    str r = "{";
    for (size_t i = 0; i < v.size(); ++i) { if (i) r += ", "; r += stringify(v[i]); }
    return r + "}";
}

template <typename T>
DAXE_NODISCARD inline str stringify(const std::set<T>& s) {
    str r = "{"; bool first = true;
    for (const auto& x : s) { if (!first) r += ", "; first = false; r += stringify(x); }
    return r + "}";
}

template <typename K, typename V>
DAXE_NODISCARD inline str stringify(const std::map<K, V>& m) {
    str r = "{"; bool first = true;
    for (const auto& [k, v] : m) { if (!first) r += ", "; first = false; r += stringify(k) + ": " + stringify(v); }
    return r + "}";
}

// Debug output
inline void debugout() { std::cerr << std::endl; }
template <typename Head, typename... Tail>
inline void debugout(const Head& h, const Tail&... t) {
    std::cerr << " " << stringify(h);
    if constexpr (sizeof...(t) > 0) { std::cerr << ","; debugout(t...); }
    else debugout();
}

DAXE_NAMESPACE_END

// Debug macros
#if DAXE_DEBUG
    #define debug(...) std::cerr << "[" << #__VA_ARGS__ << "]:", dax::debugout(__VA_ARGS__)
    #define dbg(x) std::cerr << "[" << #x << "]: " << dax::stringify(x) << std::endl
#else
    #define debug(...) ((void)0)
    #define dbg(x) ((void)0)
#endif

#define ensure(cond, msg) \
    do { if (!(cond)) DAXE_UNLIKELY { std::cerr << "Assertion failed: " << #cond << "\n" << "Message: " << msg << "\n" << "File: " << __FILE__ << ":" << __LINE__ << '\n'; std::exit(1); } } while (0)

// Expect: soft assertion that returns false on failure
#define expect(cond) \
    ([&]() -> bool { if (!(cond)) DAXE_UNLIKELY { std::cerr << "Expectation failed: " << #cond << " at " << __FILE__ << ":" << __LINE__ << '\n'; return false; } return true; }())

// Unreachable marker
#define unreachable() \
    do { std::cerr << "Unreachable code reached at " << __FILE__ << ":" << __LINE__ << '\n'; std::exit(1); } while (0)

// Timing macros
#define TIMED(name, ...) \
    do { auto _start = std::chrono::high_resolution_clock::now(); __VA_ARGS__; \
         auto _end = std::chrono::high_resolution_clock::now(); \
         std::cerr << "[TIMER] " << name << ": " << std::chrono::duration<double, std::milli>(_end - _start).count() << "ms\n"; } while (0)

// Log with file and line
#define LOG(...) std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] ", dax::debugout(__VA_ARGS__)

#endif // DAXE_DEBUG_H
