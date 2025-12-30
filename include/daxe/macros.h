/*
 * DAXE - MACROS (Clean API)
 * D.A's Axe - Cut through C++ verbosity
 * 
 * Only contains clear, readable names.
 * For competitive programming shortcuts, include <daxe/compat.h>
 */

#ifndef DAXE_MACROS_H
#define DAXE_MACROS_H

#include "config.h"
#include "base.h"
#include <limits>
#include <cstring>
#include <algorithm>
#include <vector>
#include <memory>
#include <numeric>

DAXE_NAMESPACE_BEGIN

// ==========================================
// CONSTANTS - Clear names
// ==========================================
inline constexpr i64 INF = 4e18;
inline constexpr i32 MOD = 1e9 + 7;
inline constexpr i32 MOD2 = 998244353;
inline constexpr f64 EPS = 1e-9;
inline constexpr f64 PI = 3.14159265358979323846;

// ==========================================
// DIRECTION ARRAYS - Grid navigation
// ==========================================
inline constexpr int DX4[] = {0, 0, 1, -1};
inline constexpr int DY4[] = {1, -1, 0, 0};
inline constexpr int DX8[] = {0, 0, 1, -1, 1, -1, 1, -1};
inline constexpr int DY8[] = {1, -1, 0, 0, 1, -1, -1, 1};

// ==========================================
// UPDATE FUNCTIONS - Check and update min/max
// ==========================================
template<typename T>
constexpr bool updatemin(T& a, const T& b) noexcept { 
    if (b < a) DAXE_LIKELY { a = b; return true; }
    return false;
}

template<typename T>
constexpr bool updatemax(T& a, const T& b) noexcept { 
    if (b > a) DAXE_LIKELY { a = b; return true; }
    return false;
}

// Aliases for competitive programming moved to compat.h

// Loop macros - utilizing a helper to safely cast unsigned limits
// This avoids the issue where static_cast<i64>(UINT64_MAX) becomes -1
namespace detail {
    template <typename T>
    constexpr i64 safelimit(T n) noexcept {
        if constexpr (std::is_unsigned_v<T>) {
            if (n > static_cast<std::make_unsigned_t<i64>>(std::numeric_limits<i64>::max())) 
                return std::numeric_limits<i64>::max();
        }
        return static_cast<i64>(n);
    }
}

DAXE_NAMESPACE_END

// ==========================================
// ITERATOR MACROS - Removed 'all' to allow function overloading
// ==========================================
// #define all(x) (x).begin(), (x).end()      <-- Conflicts with all(container, pred)
// #define reverseall(x) (x).rbegin(), (x).rend()

// ==========================================
// SIZE MACRO - Returns i64 for safe comparisons
// ==========================================
#define len(x) static_cast<dax::i64>((x).size())

// ==========================================
// LOOP MACROS - Clear names
// ==========================================
// Loop macros - utilizing a helper to safely cast unsigned limits
// This avoids the issue where static_cast<i64>(UINT64_MAX) becomes -1
// (Moved to dax namespace above)
#define loop(i, n) for (dax::i64 i = 0; i < dax::detail::safelimit(n); ++i)
#define loop1(i, n) for (dax::i64 i = 1; i <= static_cast<dax::i64>(n); ++i)
#define loopdown(i, n) for (dax::i64 i = static_cast<dax::i64>(n) - 1; i >= 0; --i)
#define loopfrom(i, a, b) for (dax::i64 i = static_cast<dax::i64>(a); i < static_cast<dax::i64>(b); ++i)
#define foreach(x, c) for (auto& x : c)
#define foreachconst(x, c) for (const auto& x : c)

// ==========================================
// ALGORITHM MACROS - Clear names
// ==========================================
#define sortasc(x) std::sort((x).begin(), (x).end())
#define sortdesc(x) std::sort((x).rbegin(), (x).rend())
// 'flip' macro removed to avoid collision with std::vector<bool>::flip()
#define reverseall(x) std::reverse((x).begin(), (x).end())
#define uniquify(x) do { std::sort((x).begin(), (x).end()); (x).erase(std::unique((x).begin(), (x).end()), (x).end()); } while(0)
#define lowerbound(c, x) std::lower_bound((c).begin(), (c).end(), x)
#define upperbound(c, x) std::upper_bound((c).begin(), (c).end(), x)

// ==========================================
// BIT MANIPULATION - Clear names
// ==========================================
#define getbit(n, i) (((n) >> (i)) & 1)
#define setbit(n, i) ((n) | (1LL << (i)))
#define clearbit(n, i) ((n) & ~(1LL << (i)))
#define togglebit(n, i) ((n) ^ (1LL << (i)))

// ==========================================
// MEMORY - Clear names
// ==========================================
#define memzero(a) std::memset(a, 0, sizeof(a))
#define memfill(a, v) std::memset(a, v, sizeof(a))

#endif // DAXE_MACROS_H
