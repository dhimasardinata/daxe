/*
 * DAXE - I/O OVERLOADS
 * D.A's Axe - Cut through C++ verbosity
 * 
 * Naming: Flat style (no snake_case)
 */

#ifndef DAXE_IO_H
#define DAXE_IO_H

#include "containers.h"
#include "safe.h"
#include <iostream>

#if DAXE_HAS_PRINT
#include <print>
#endif

#include <fstream>
#include <sstream>

DAXE_NAMESPACE_BEGIN

// Fast I/O setup using Meyers Singleton pattern
// This approach is compatible with C++20 modules across all compilers (MSVC, Clang, GCC)
namespace detail {
    struct FastIO {
        FastIO() noexcept {
            std::ios::sync_with_stdio(false);
            std::cin.tie(nullptr);
            std::cout.tie(nullptr);
        }
    };
    
    // Function-local static ensures single instance and proper initialization
    inline FastIO& fastio() {
        static FastIO instance{};
        return instance;
    }
    
    // Force initialization at startup
    namespace { [[maybe_unused]] auto& init_ = fastio(); }
}

// 128-bit I/O with input validation
#if DAXE_HAS_INT128
inline std::istream& operator>>(std::istream& is, i128& n) {
    str s; is >> s;
    n = 0;
    bool neg = false;
    size_t i = 0;
    if (s.empty()) return is;
    if (s[0] == '-') { neg = true; i = 1; }
    
    // For simplicity with i128, working with positive u128 is easier but conversion is tricky for MIN 
    // Let's use u128 for parsing then check range
    u128 val = 0;
    // Max abs value of i128 is 2^127 for negative (unsigned view), or 2^127-1 for positive
    // 2^127 = 170141183460469231731687303715884105728
    
    for (; i < s.size(); ++i) {
        char c = s[i];
        if (c < '0' || c > '9') {
            is.setstate(std::ios::failbit);
            return is;
        }
        int digit = c - '0';
        
        // Overflow check (conservative u128 check)
        // u128 max is ~3.4e38. i128 max is ~1.7e38
        if (val > (static_cast<u128>(-1) - digit) / 10) {
             is.setstate(std::ios::failbit);
             return is;
        }
        val = val * 10 + digit;
    }
    
    // Check if val fits in i128
    // i128 min abs: 2^127
    // i128 max abs: 2^127 - 1
    u128 abs_limit = static_cast<u128>(1) << 127;
    if (!neg) abs_limit -= 1;
    
    if (val > abs_limit) {
        is.setstate(std::ios::failbit);
        return is;
    }
    
    n = neg ? -static_cast<i128>(val) : static_cast<i128>(val);
    return is;
}

inline std::ostream& operator<<(std::ostream& os, i128 n) {
    if (n == 0) return os << '0';
    
    u128 un;
    if (n < 0) {
        os << '-';
        // Handle INT128_MIN specially or cast carefully
        // -(INT128_MIN) is UB in signed arithmetic if it stays signed
        // logical negation of 100...00 is same in 2s complement but casting 100..0 is safe for u128
        un = static_cast<u128>(0) - static_cast<u128>(n);
    } else {
        un = static_cast<u128>(n);
    }
    
    char buf[40]; int i = 0;
    while (un > 0) { buf[i++] = static_cast<char>('0' + un % 10); un /= 10; }
    while (i--) os << buf[i];
    return os;
}

inline std::istream& operator>>(std::istream& is, u128& n) {
    str s; is >> s;
    n = 0;
    for (char c : s) {
        if (c < '0' || c > '9') {
            is.setstate(std::ios::failbit);
            return is;
        }
        n = n * 10 + (c - '0');
    }
    return is;
}

inline std::ostream& operator<<(std::ostream& os, u128 n) {
    if (n == 0) return os << '0';
    char buf[40]; int i = 0;
    while (n > 0) { buf[i++] = static_cast<char>('0' + n % 10); n /= 10; }
    while (i--) os << buf[i];
    return os;
}
#endif

// Pair I/O
template <typename T, typename U>
inline std::istream& operator>>(std::istream& is, std::pair<T, U>& p) {
    return is >> p.first >> p.second;
}

template <typename T, typename U>
inline std::ostream& operator<<(std::ostream& os, const std::pair<T, U>& p) {
    return os << p.first << ' ' << p.second;
}

// Vector I/O
template <typename T>
inline std::istream& operator>>(std::istream& is, std::vector<T>& v) {
    for (auto& x : v) is >> x;
    return is;
}

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    for (size_t i = 0; i < v.size(); ++i) {
        if (i) os << ' ';
        os << v[i];
    }
    return os;
}

// Set/Map I/O
template <typename T>
inline std::ostream& operator<<(std::ostream& os, const std::set<T>& s) {
    bool first = true;
    for (const auto& x : s) { if (!first) os << ' '; first = false; os << x; }
    return os;
}

template <typename K, typename V>
inline std::ostream& operator<<(std::ostream& os, const std::map<K, V>& m) {
    os << '{';
    bool first = true;
    for (const auto& [k, v] : m) { if (!first) os << ", "; first = false; os << k << ": " << v; }
    return os << '}';
}

// Deque I/O
template <typename T>
inline std::ostream& operator<<(std::ostream& os, const std::deque<T>& d) {
    for (size_t i = 0; i < d.size(); ++i) {
        if (i) os << ' ';
        os << d[i];
    }
    return os;
}

// Print functions
#if DAXE_HAS_PRINT
// Raw print (no newline)
inline void print() {}
template <typename T>
inline void print(const T& x) { std::print("{}", x); }
inline void print(char x) { std::print("{}", x); }
inline void print(const char* x) { std::print("{}", x); }
inline void print(char* x) { std::print("{}", x); }

template <typename T, typename... Args>
inline void print(const T& first, const Args&... rest) {
    std::print("{} ", first);
    print(rest...);
}

// PrintLn (newline)
inline void println() { std::println(""); }
template <typename... Args>
inline void println(const Args&... args) {
    print(args...);
    std::println("");
}
#else
// Raw print (no newline)
inline void print() {}
template <typename T>
inline void print(const T& x) { std::cout << x; }
inline void print(char x) { std::cout.put(x); }
inline void print(const char* x) { std::cout << std::string(x); }
inline void print(char* x) { std::cout << std::string(x); }

template <typename T, typename... Args>
inline void print(const T& first, const Args&... rest) {
    using Decayed = typename std::decay<T>::type;
    if constexpr (std::is_same<Decayed, const char*>::value || std::is_same<Decayed, char*>::value) {
        std::cout << std::string(first); 
    } else {
        std::cout << first;
    }
    std::cout.put(' ');
    print(rest...);
}

// PrintLn (newline)
template <typename... Args>
inline void println(const Args&... args) {
    print(args...);
    std::cout.put('\n');
}
#endif

template <typename... Args>
inline void printspaced(const Args&... args) { ((std::cout << args << ' '), ...); }

// Input functions
template <typename T>
inline void input(T& t) { std::cin >> t; }

template <typename T, typename... Args>
inline void input(T& first, Args&... rest) {
    std::cin >> first;
    input(rest...);
}

// Read functions - flat naming
template <typename T = i64>
DAXE_NODISCARD inline std::vector<T> readvector(size_t n) {
    std::vector<T> v(n);
    for (auto& x : v) std::cin >> x;
    return v;
}

template <typename T = i64>
DAXE_NODISCARD inline std::vector<std::vector<T>> readgrid(size_t rows, size_t cols) {
    std::vector<std::vector<T>> grid(rows, std::vector<T>(cols));
    for (auto& row : grid) for (auto& x : row) std::cin >> x;
    return grid;
}

DAXE_NODISCARD inline str readline() {
    str line;
    std::getline(std::cin, line);
    return line;
}

// ==========================================
// FILE I/O
// ==========================================

// Original readfile (returns empty string on error - for backward compatibility)
DAXE_NODISCARD inline str readfile(const str& path) {
    std::ifstream f(path, std::ios::in | std::ios::binary);
    if (!f) DAXE_UNLIKELY return "";
    f.seekg(0, std::ios::end);
    auto size = f.tellg();
    if (size == static_cast<std::streampos>(-1)) DAXE_UNLIKELY return "";
    std::string content;
    content.resize(static_cast<size_t>(size));
    f.seekg(0, std::ios::beg);
    f.read(&content[0], content.size());
    return content;
}

// Safe readfile - returns Result<str> with proper error handling
DAXE_NODISCARD inline Result<str> tryreadfile(const str& path) {
    std::ifstream f(path, std::ios::in | std::ios::binary);
    if (!f) DAXE_UNLIKELY return Result<str>::Err("Failed to open file: " + path);
    std::string content;
    f.seekg(0, std::ios::end);
    auto size = f.tellg();
    if (size == static_cast<std::streampos>(-1)) return Result<str>::Err("Failed to get file size (stream error): " + path);
    content.resize(static_cast<size_t>(size));
    f.seekg(0, std::ios::beg);
    f.read(&content[0], content.size());
    if (!f) return Result<str>::Err("Failed to read file: " + path);
    return Result<str>::Ok(content);
}

DAXE_NODISCARD inline std::vector<str> readlines(const str& path) {
    std::ifstream f(path);
    if (!f) DAXE_UNLIKELY return {};
    std::vector<str> lines;
    str line;
    while (std::getline(f, line)) {
        lines.push_back(line);
    }
    return lines;
}

// Safe readlines
DAXE_NODISCARD inline Result<std::vector<str>> tryreadlines(const str& path) {
    std::ifstream f(path);
    if (!f) DAXE_UNLIKELY return Result<std::vector<str>>::Err("Failed to open file: " + path);
    std::vector<str> lines;
    str line;
    while (std::getline(f, line)) {
        lines.push_back(line);
    }
    return Result<std::vector<str>>::Ok(lines);
}

inline bool writefile(const str& path, const str& content) {
    std::ofstream f(path);
    if (!f) DAXE_UNLIKELY return false;
    f << content;
    return true;
}

inline bool writefile(const str& path, const std::vector<str>& lines) {
    std::ofstream f(path);
    if (!f) DAXE_UNLIKELY return false;
    for (const auto& line : lines) f << line << '\n';
    return true;
}

inline bool appendfile(const str& path, const str& content) {
    std::ofstream f(path, std::ios::app);
    if (!f) DAXE_UNLIKELY return false;
    f << content;
    return true;
}

inline bool copyfile(const str& src, const str& dst) {
    std::ifstream s(src, std::ios::binary);
    std::ofstream d(dst, std::ios::binary);
    if (!s || !d) DAXE_UNLIKELY return false;
    d << s.rdbuf();
    return true;
}

inline bool removefile(const str& path) {
    return std::remove(path.c_str()) == 0;
}

DAXE_NODISCARD inline bool fileexists(const str& path) {
    std::ifstream f(path);
    return f.good();
}

DAXE_NAMESPACE_END

#endif // DAXE_IO_H
