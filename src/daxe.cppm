module;

// ==========================================
// GLOBAL MODULE FRAGMENT
// ==========================================
// System headers MUST be included here to avoid
// "block-scope extern declaration" errors in C++20 modules.

// Core
#include <algorithm>
#include <bit>
#include <cctype>
#include <cmath>
#include <compare>
#include <concepts>
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <limits>
#include <memory>
#include <new>
#include <numbers>
#include <numeric>
#include <ranges>
#include <span>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

// Containers
#include <array>
#include <deque>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>

// IO & Filesystem
#include <charconv>  // Needed for daxe/convert.h
#include <fstream>
#include <sstream>

// Safety & Functional
#include <functional>
#include <optional>
#include <stdexcept>
#include <variant>

// Threading & Time
#include <chrono>
#include <mutex>
#include <random>
#include <thread>

// C++23 Features (Feature detection)
// This fixes the specific error with std::print
#if defined(__has_include)
#if __has_include(<print>)
#include <print>
#endif
#if __has_include(<expected>)
#include <expected>
#endif
#if __has_include(<mdspan>)
#include <mdspan>
#endif
#endif

export module daxe;

// ==========================================
// MODULE PURVIEW
// ==========================================

// Disable auto-importing to global scope inside the module itself
// Users will use "using namespace dax;" if they want that.
#define DAXE_NO_GLOBAL

// Include the master header
// Since system headers were included in the Global Fragment,
// headers inside daxe.h will use those definitions without
// attaching them to this named module.
export {
#include "../include/daxe.h"
}

// Explicitly export the namespace
export using namespace dax;