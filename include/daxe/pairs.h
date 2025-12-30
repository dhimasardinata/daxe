/*
 * DAXE - PAIRS
 * D.A's Axe - Cut through C++ verbosity
 */

#ifndef DAXE_PAIRS_H
#define DAXE_PAIRS_H

#include "base.h"
#include <utility>

DAXE_NAMESPACE_BEGIN

// Signed pairs
using pi8 = std::pair<i8, i8>;
using pi16 = std::pair<i16, i16>;
using pi32 = std::pair<i32, i32>;
using pi64 = std::pair<i64, i64>;
#if DAXE_HAS_INT128
using pi128 = std::pair<i128, i128>;
#endif

// Unsigned pairs
using pu8 = std::pair<u8, u8>;
using pu16 = std::pair<u16, u16>;
using pu32 = std::pair<u32, u32>;
using pu64 = std::pair<u64, u64>;
#if DAXE_HAS_INT128
using pu128 = std::pair<u128, u128>;
#endif

// Float pairs
using pf32 = std::pair<f32, f32>;
using pf64 = std::pair<f64, f64>;
using pf80 = std::pair<f80, f80>;

// Other pairs
using pstr = std::pair<str, str>;
using pbool = std::pair<bool, bool>;
using pchar = std::pair<char, char>;

// Mixed pairs
using pi32i64 = std::pair<i32, i64>;
using pi64i32 = std::pair<i64, i32>;
using pi32str = std::pair<i32, str>;
using pi64str = std::pair<i64, str>;
using pstri32 = std::pair<str, i32>;
using pstri64 = std::pair<str, i64>;

DAXE_NAMESPACE_END

#endif // DAXE_PAIRS_H
