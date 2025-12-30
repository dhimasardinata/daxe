/*
 * DAXE - VECTORS
 * D.A's Axe - Cut through C++ verbosity
 */

#ifndef DAXE_VECTORS_H
#define DAXE_VECTORS_H

#include "pairs.h"
#include <vector>
#include <deque>

DAXE_NAMESPACE_BEGIN

// 1D Vectors - Signed
template<typename T = i64>
struct Vec : public std::vector<T> {
    using std::vector<T>::vector;
};
// Deduction guides for Vec
Vec() -> Vec<i64>;
template <typename T> Vec(std::initializer_list<T>) -> Vec<T>;

using vi8 = std::vector<i8>;
using vi16 = std::vector<i16>;
using vi32 = std::vector<i32>;
using vi64 = std::vector<i64>;
#if DAXE_HAS_INT128
using vi128 = std::vector<i128>;
#endif

// 1D Vectors - Unsigned
using vu8 = std::vector<u8>;
using vu16 = std::vector<u16>;
using vu32 = std::vector<u32>;
using vu64 = std::vector<u64>;
#if DAXE_HAS_INT128
using vu128 = std::vector<u128>;
#endif

// 1D Vectors - Float
using vf32 = std::vector<f32>;
using vf64 = std::vector<f64>;
using vf80 = std::vector<f80>;

// 1D Vectors - Other
using vstr = std::vector<str>;
using vbool = std::vector<bool>;
using vchar = std::vector<char>;

// Vector of Pairs
using vpi32 = std::vector<pi32>;
using vpi64 = std::vector<pi64>;
#if DAXE_HAS_INT128
using vpi128 = std::vector<pi128>;
#endif
using vpf64 = std::vector<pf64>;
using vpstr = std::vector<pstr>;

// 2D Vectors
using vvi32 = std::vector<vi32>;
using vvi64 = std::vector<vi64>;
#if DAXE_HAS_INT128
using vvi128 = std::vector<vi128>;
#endif
using vvf64 = std::vector<vf64>;
using vvstr = std::vector<vstr>;
using vvbool = std::vector<vbool>;
using vvchar = std::vector<vchar>;
using vvpi32 = std::vector<vpi32>;
using vvpi64 = std::vector<vpi64>;

// 3D Vectors
using vvvi32 = std::vector<vvi32>;
using vvvi64 = std::vector<vvi64>;
using vvvbool = std::vector<vvbool>;

// 4D Vectors
using vvvvi32 = std::vector<vvvi32>;
using vvvvi64 = std::vector<vvvi64>;

DAXE_NAMESPACE_END

#endif // DAXE_VECTORS_H
