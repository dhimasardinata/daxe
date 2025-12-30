/*
 * DAXE - COMPETITIVE PROGRAMMING COMPATIBILITY
 * D.A's Axe - Cut through C++ verbosity
 * 
 * Provides short macros for competitive programming.
 * Include this file AFTER daxe.h for CP shortcuts.
 * 
 * Usage:
 *   #include <daxe.h>
 *   #include <daxe/compat.h>  // Adds CP shortcuts
 */

#ifndef DAXE_COMPAT_H
#define DAXE_COMPAT_H

#include "macros.h"

// ==========================================
// UPDATE FUNCTION SHORTCUTS (CP style)
// ==========================================
template<typename T>
constexpr bool ckmin(T& a, const T& b) noexcept { if (b < a) { a = b; return true; } return false; }
template<typename T>
constexpr bool ckmax(T& a, const T& b) noexcept { if (b > a) { a = b; return true; } return false; }

// ==========================================
// SIZE SHORTCUTS
// ==========================================
#define sz(x) len(x)
#define rall(x) reverseall(x)

// ==========================================
// CONTAINER METHOD SHORTCUTS
// ==========================================
#define pb push_back
#define eb emplace_back

// ==========================================
// PAIR/TUPLE SHORTCUTS
// ==========================================
#define mp std::make_pair
#define mt std::make_tuple
#define ft first
#define sd second

// ==========================================
// LOOP SHORTCUTS (CP style)
// ==========================================
#define rep(i, n) loop(i, n)
#define rep1(i, n) loop1(i, n)
#define rrep(i, n) loopdown(i, n)
#define FOR(i, a, b) loopfrom(i, a, b)
#define FORR(i, a, b) for (dax::i64 i = static_cast<dax::i64>(b) - 1; i >= static_cast<dax::i64>(a); --i)
#define each(x, c) foreach(x, c)
#define ceach(x, c) foreachconst(x, c)

// ==========================================
// ALGORITHM SHORTCUTS
// ==========================================
#define sor(x) sortasc(x)
#define rsor(x) sortdesc(x)
#define rev(x) flip(x)
#define uniq(x) uniquify(x)
#define lb(c, x) lowerbound(c, x)
#define ub(c, x) upperbound(c, x)

// ==========================================
// BIT SHORTCUTS
// ==========================================
#define bit(n, i) getbit(n, i)
#define popcount(n) countbits(n)
#define clz(n) leadingzeros(n)
#define ctz(n) trailingzeros(n)
#define lg2(n) log2floor(n)

// ==========================================
// MEMORY SHORTCUTS
// ==========================================
#define zeromem(a) memzero(a)
#define fillmem(a, v) memfill(a, v)

// ==========================================
// I/O SHORTCUTS
// ==========================================
// Note: We avoid defining 'endl' as it would break std::endl usage
// Use '\n' directly for performance (doesn't flush buffer)
#define readvec dax::readvector
#define printn dax::printspaced

// ==========================================
// COMMON CP TYPES (ultra-short)
// ==========================================
using ll = dax::i64;
using ull = dax::u64;
using ld = dax::f64;
using pii = std::pair<int, int>;
using pll = std::pair<dax::i64, dax::i64>;
using vll = std::vector<dax::i64>;
using vvll = std::vector<std::vector<dax::i64>>;
using vs = dax::vstr;
using vvs = dax::vvstr;

#endif // DAXE_COMPAT_H
