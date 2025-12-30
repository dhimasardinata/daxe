module;

// GLOBAL MODULE FRAGMENT
// Include ALL system headers here to avoid conflicts
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <stack>
#include <deque>
#include <utility>
#include <limits>
#include <fstream>
#include <sstream>
#include <cctype>
#include <cstdint>
#include <optional>
#include <variant>
#include <functional>
#include <stdexcept>
#include <array>
#include <random>
#include <chrono>
#include <thread>
#include <mutex>
#include <new>
#include <memory>
#include <type_traits>
#include <concepts>
#include <iterator>
#include <ranges>
#include <initializer_list>
#include <compare>
#include <bit>
#include <numbers>
#include <span>

export module daxe;

// Define export macro BEFORE including headers
#define DAXE_EXPORT export
#define DAXE_NO_GLOBAL

// Include the master header
// Since DAXE_EXPORT is strictly defined as 'export', 
// and DAXE_NAMESPACE_BEGIN uses it, all dax content will be exported.
#include "../include/daxe.h"

// Note: Macros (ensure, expect, etc.) are NOT exported.
// Users must include <daxe/debug.h> or similar if they need macros.

export using namespace dax;
