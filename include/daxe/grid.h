/*
 * DAXE - 2D GRID UTILITIES
 * D.A's Axe - Cut through C++ verbosity
 * 
 * Essential utilities for grid-based problems
 */

#ifndef DAXE_GRID_H
#define DAXE_GRID_H

#include "base.h"
#include "macros.h"
#include <array>
#include <vector>
#include <utility>

DAXE_NAMESPACE_BEGIN

// ==========================================
// DIRECTION ARRAYS
// ==========================================
// 4 directions: up, right, down, left
constexpr std::array<std::pair<i64, i64>, 4> DIR4 = {{{-1, 0}, {0, 1}, {1, 0}, {0, -1}}};
// 8 directions: including diagonals
constexpr std::array<std::pair<i64, i64>, 8> DIR8 = {{{-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}}};

// ==========================================
// BOUNDS CHECKING
// ==========================================
DAXE_NODISCARD constexpr bool inbounds(i64 r, i64 c, i64 rows, i64 cols) noexcept {
    return r >= 0 && r < rows && c >= 0 && c < cols;
}

// ==========================================
// NEIGHBOR GENERATION
// ==========================================
DAXE_NODISCARD inline std::vector<std::pair<i64, i64>> neighbors4(i64 r, i64 c, i64 rows, i64 cols) {
    std::vector<std::pair<i64, i64>> result;
    result.reserve(4);
    for (auto [dr, dc] : DIR4) {
        i64 nr = r + dr, nc = c + dc;
        if (inbounds(nr, nc, rows, cols)) result.push_back({nr, nc});
    }
    return result;
}

DAXE_NODISCARD inline std::vector<std::pair<i64, i64>> neighbors8(i64 r, i64 c, i64 rows, i64 cols) {
    std::vector<std::pair<i64, i64>> result;
    result.reserve(8);
    for (auto [dr, dc] : DIR8) {
        i64 nr = r + dr, nc = c + dc;
        if (inbounds(nr, nc, rows, cols)) result.push_back({nr, nc});
    }
    return result;
}

// ==========================================
// GRID INDEX CONVERSION
// ==========================================
DAXE_NODISCARD constexpr i64 toindex(i64 r, i64 c, i64 cols) noexcept { return r * cols + c; }
DAXE_NODISCARD constexpr std::pair<i64, i64> tocoord(i64 index, i64 cols) noexcept { return {index / cols, index % cols}; }

// ==========================================
// MANHATTAN DISTANCE
// ==========================================
DAXE_NODISCARD constexpr i64 manhattan(i64 r1, i64 c1, i64 r2, i64 c2) noexcept {
    return (r1 > r2 ? r1 - r2 : r2 - r1) + (c1 > c2 ? c1 - c2 : c2 - c1);
}

DAXE_NAMESPACE_END

#endif // DAXE_GRID_H
