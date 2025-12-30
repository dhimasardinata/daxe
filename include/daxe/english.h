/*
 * DAXE - ENGLISH ALIASES
 * D.A's Axe - Cut through C++ verbosity
 * 
 * Pythonic, Rust-like, English-readable type aliases
 */

#ifndef DAXE_ENGLISH_H
#define DAXE_ENGLISH_H

#include "base.h"

DAXE_NAMESPACE_BEGIN

// ==========================================
// KEYWORD ALIASES (Type Inference)
// Note: `var` conflicts with Windows SDK headers on Windows
// ==========================================
// ==========================================
// KEYWORD ALIASES (Type Inference)
// ==========================================
// 'var' is not a C++ keyword. We define it as auto universally.
// If any Windows header defines 'var', we undefine it first or just rely on this macro.
#ifdef var
    #undef var
#endif
#define var auto
#define let const auto
// These are always safe:
#define fx auto              // fx square(i64 x) { return x*x; }
#define fn auto              // fn add(i64 a, i64 b) { return a+b; }
#define proc void            // proc log(str msg) { print(msg); }

// ==========================================
// ENGLISH TYPE ALIASES (Primary Types)
// ==========================================
using Int = i64;
using Float = f64;
using String = str;
using Bool = bool;
using Char = char;

// Lowercase alternatives
using number = i64;
using decimal = f64;
using text = str;
using byte = u8;
using letter = char;

// ==========================================
// SIZE VARIANTS
// ==========================================
using int8 = i8;
using int16 = i16;
using int32 = i32;
using int64 = i64;
#if DAXE_HAS_INT128
using int128 = i128;
#endif

using uint8 = u8;
using uint16 = u16;
using uint32 = u32;
using uint64 = u64;
#if DAXE_HAS_INT128
using uint128 = u128;
#endif

// ==========================================
// SEMANTIC ALIASES
// ==========================================
using Index = i64;
using Count = i64;
using Size = i64;
using Length = i64;
using Position = i64;
using Offset = i64;
using Distance = i64;
using Weight = i64;
using Cost = i64;
using Timestamp = i64;
using Id = i64;

using Real = f64;
using Ratio = f64;
using Probability = f64;
using Coordinate = f64;
using Angle = f64;

// ==========================================
// PAIR ALIASES
// ==========================================
using Point = std::pair<Int, Int>;
using Span = std::pair<Int, Int>;
using Interval = std::pair<Int, Int>;
using Edge = std::pair<Int, Int>;
using Bounds = std::pair<Int, Int>;

using Point2D = std::pair<Float, Float>;
using Point3D = std::tuple<Float, Float, Float>;

using Entry = std::pair<String, Int>;
using Mapping = std::pair<Int, Int>;

DAXE_NAMESPACE_END

#endif // DAXE_ENGLISH_H
