/*
 * DAXE - BASE PRIMITIVES
 * D.A's Axe - Cut through C++ verbosity
 * 
 * Minimal includes for fast compilation
 */

#ifndef DAXE_BASE_H
#define DAXE_BASE_H

#include "config.h"
#include <cstdint>
#include <string>

DAXE_NAMESPACE_BEGIN

// ==========================================
// SIGNED INTEGERS
// ==========================================
using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

// ==========================================
// UNSIGNED INTEGERS
// ==========================================
using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

// ==========================================
// 128-BIT INTEGERS (GCC/Clang only)
// ==========================================
#if defined(__GNUC__) || defined(__clang__)
    using i128 = __int128_t;
    using u128 = __uint128_t;
    #define DAXE_HAS_INT128 1
#else
    // MSVC doesn't support __int128.
    // We STRICTLY do not alias i128 to i64 anymore to prevent silent overflow bugs.
    // Users code relying on i128 on Windows will correctly fail to compile.
    #define DAXE_HAS_INT128 0
#endif

// ==========================================
// FLOATING POINT
// ==========================================
using f32 = float;
using f64 = double;
using f80 = long double;

// ==========================================
// STRING
// ==========================================
using str = std::string;
using String = std::string;

// ==========================================
// BOOLEAN
// ==========================================
using boolean = bool;

DAXE_NAMESPACE_END

// Bring types into global scope
#ifndef DAXE_NO_GLOBAL
using namespace dax;
#endif

#endif // DAXE_BASE_H
