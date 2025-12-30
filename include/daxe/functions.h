/*
 * DAXE - UTILITY FUNCTIONS
 * D.A's Axe - Cut through C++ verbosity
 * 
 * Design: Universal functions work on ANY container
 * - Nouns for getters: max, min, sum, first, last
 * - Past participles: sorted, reversed, filtered
 * - Predicates: has, isprime
 */

#ifndef DAXE_FUNCTIONS_H
#define DAXE_FUNCTIONS_H

#include "containers.h"
#include "macros.h"
#include "safe.h"
#include <algorithm>
#include <numeric>
#include <cctype>

DAXE_NAMESPACE_BEGIN

// ==========================================
// TYPE TRAITS FOR UNIVERSAL FUNCTIONS
// ==========================================
namespace detail {
    template<typename T, typename = void>
    struct is_iterable : std::false_type {};
    
    template<typename T>
    struct is_iterable<T, std::void_t<
        decltype(std::begin(std::declval<T>())),
        decltype(std::end(std::declval<T>()))
    >> : std::true_type {};
    
    template<typename T>
    inline constexpr bool is_iterable_v = is_iterable<T>::value;
    
    template<typename T, typename = void>
    struct has_back : std::false_type {};
    
    template<typename T>
    struct has_back<T, std::void_t<decltype(std::declval<T>().back())>> : std::true_type {};
}

// ==========================================
// PIPE OPERATOR FOR COMPOSABILITY
// ==========================================
// Usage: value | transform | filter | collect
template <typename T, typename Func>
auto operator|(T&& val, Func&& f) -> decltype(f(std::forward<T>(val))) {
    return f(std::forward<T>(val));
}

// ==========================================
// UNIVERSAL GETTERS - Work on ANY container
// ==========================================

// max - two values
template <typename T>
DAXE_NODISCARD constexpr T max(const T& a, const T& b) noexcept {
    return a > b ? a : b;
}

// max - vector (optimized, no SFINAE overhead) - Safe by default
template <typename T>
DAXE_NODISCARD DAXE_ALWAYS_INLINE Option<T> max(const std::vector<T>& v) {
    if (v.empty()) return None;
    return Some(*std::max_element(v.begin(), v.end()));
}

// max - vector with fallback
template <typename T>
DAXE_NODISCARD DAXE_ALWAYS_INLINE T max(const std::vector<T>& v, const T& fallback) {
    if (v.empty()) return fallback;
    return *std::max_element(v.begin(), v.end());
}

// max - any other container (fallback with SFINAE) - Safe by default
template <typename Container, typename = std::enable_if_t<
    detail::is_iterable_v<Container> && 
    !std::is_same_v<Container, std::vector<typename Container::value_type>>>>
DAXE_NODISCARD inline auto max(const Container& c) -> Option<typename Container::value_type> {
    if (std::empty(c)) return None;
    return Some(*std::max_element(std::begin(c), std::end(c)));
}

// min - two values
template <typename T>
DAXE_NODISCARD constexpr T min(const T& a, const T& b) noexcept {
    return a < b ? a : b;
}

// min - vector (optimized, no SFINAE overhead) - Safe by default
template <typename T>
DAXE_NODISCARD DAXE_ALWAYS_INLINE Option<T> min(const std::vector<T>& v) {
    if (v.empty()) return None;
    return Some(*std::min_element(v.begin(), v.end()));
}

// min - vector with fallback
template <typename T>
DAXE_NODISCARD DAXE_ALWAYS_INLINE T min(const std::vector<T>& v, const T& fallback) {
    if (v.empty()) return fallback;
    return *std::min_element(v.begin(), v.end());
}

// min - any other container (fallback) - Safe by default
template <typename Container, typename = std::enable_if_t<
    detail::is_iterable_v<Container> && 
    !std::is_same_v<Container, std::vector<typename Container::value_type>>>>
DAXE_NODISCARD inline auto min(const Container& c) -> Option<typename Container::value_type> {
    if (std::empty(c)) return None;
    return Some(*std::min_element(std::begin(c), std::end(c)));
}

// minmax - get both min and max in one pass - Safe by default
template <typename T>
DAXE_NODISCARD inline Option<std::pair<T, T>> minmax(const std::vector<T>& v) {
    if (v.empty()) return None;
    auto [minit, maxit] = std::minmax_element(v.begin(), v.end());
    return Some(std::make_pair(*minit, *maxit));
}

// argmax - index of maximum element - Safe by default
template <typename T>
DAXE_NODISCARD inline Option<i64> argmax(const std::vector<T>& v) {
    if (v.empty()) return None;
    return Some(static_cast<i64>(std::max_element(v.begin(), v.end()) - v.begin()));
}

// argmax with fallback
template <typename T>
DAXE_NODISCARD inline i64 argmax(const std::vector<T>& v, i64 fallback) {
    if (v.empty()) return fallback;
    return static_cast<i64>(std::max_element(v.begin(), v.end()) - v.begin());
}

// argmin - index of minimum element - Safe by default
template <typename T>
DAXE_NODISCARD inline Option<i64> argmin(const std::vector<T>& v) {
    if (v.empty()) return None;
    return Some(static_cast<i64>(std::min_element(v.begin(), v.end()) - v.begin()));
}

// argmin with fallback
template <typename T>
DAXE_NODISCARD inline i64 argmin(const std::vector<T>& v, i64 fallback) {
    if (v.empty()) return fallback;
    return static_cast<i64>(std::min_element(v.begin(), v.end()) - v.begin());
}

// sum - vector (optimized, no SFINAE overhead)
template <typename T>
DAXE_NODISCARD DAXE_ALWAYS_INLINE auto sum(const std::vector<T>& v) noexcept -> T {
    return std::accumulate(v.begin(), v.end(), T{});
}

// sum - any other container (fallback)
#if DAXE_HAS_CONCEPTS
template <typename Container>
requires detail::is_iterable_v<Container>
#else
template <typename Container, typename = std::enable_if_t<detail::is_iterable_v<Container>>>
#endif
DAXE_NODISCARD inline auto sum(const Container& c) -> typename Container::value_type {
    return std::accumulate(std::begin(c), std::end(c), typename Container::value_type{});
}

// product - multiply all elements
template <typename T>
DAXE_NODISCARD DAXE_ALWAYS_INLINE auto product(const std::vector<T>& v) noexcept -> T {
    return std::accumulate(v.begin(), v.end(), T{1}, std::multiplies<T>());
}

// iota - generate range [start, start+count)
DAXE_NODISCARD inline vi64 iota(i64 count, i64 start = 0) {
    if (count < 0) return {};
    vi64 result(static_cast<size_t>(count));
    std::iota(result.begin(), result.end(), start);
    return result;
}

// prefixsum - cumulative sum from left
template <typename T>
DAXE_NODISCARD inline std::vector<T> prefixsum(const std::vector<T>& v) {
    std::vector<T> result(v.size());
    if (!v.empty()) {
        result[0] = v[0];
        for (size_t i = 1; i < v.size(); ++i) result[i] = result[i-1] + v[i];
    }
    return result;
}

// suffixsum - cumulative sum from right
template <typename T>
DAXE_NODISCARD inline std::vector<T> suffixsum(const std::vector<T>& v) {
    std::vector<T> result(v.size());
    if (!v.empty()) {
        result[v.size()-1] = v[v.size()-1];
        for (size_t i = v.size()-1; i-- > 0;) result[i] = result[i+1] + v[i];
    }
    return result;
}

// rangesum - sum from index l to r (inclusive) using prefix sum
template <typename T>
DAXE_NODISCARD inline T rangesum(const std::vector<T>& prefix, i64 l, i64 r) {
    if (l > r || r >= static_cast<i64>(prefix.size())) return T{};
    return prefix[r] - (l > 0 ? prefix[l-1] : T{});
}

// pairwise - apply function to adjacent pairs
template <typename T, typename Func>
DAXE_NODISCARD inline auto pairwise(const std::vector<T>& v, Func&& f) {
    using R = decltype(f(v[0], v[0]));
    std::vector<R> result;
    if (v.size() > 1) {
        result.reserve(v.size() - 1);
        for (size_t i = 0; i + 1 < v.size(); ++i) result.push_back(f(v[i], v[i+1]));
    }
    return result;
}

// differences - compute differences between adjacent elements
template <typename T>
DAXE_NODISCARD inline std::vector<T> differences(const std::vector<T>& v) {
    return pairwise(v, [](const T& a, const T& b) { return b - a; });
}

// first - any container
#if DAXE_HAS_CONCEPTS
template <typename Container>
requires detail::is_iterable_v<Container>
#else
template <typename Container, typename = std::enable_if_t<detail::is_iterable_v<Container>>>
#endif
DAXE_NODISCARD inline auto first(const Container& c) -> Option<typename Container::value_type> {
    if (std::begin(c) == std::end(c)) DAXE_UNLIKELY return None;
    return Some(*std::begin(c));
}

// last - any container with back()
template <typename Container>
DAXE_NODISCARD inline auto last(const Container& c) -> 
    std::enable_if_t<detail::has_back<Container>::value, Option<typename Container::value_type>> {
    if (c.empty()) DAXE_UNLIKELY return None;
    return Some(c.back());
}

// ==========================================
// UNIVERSAL PREDICATES
// ==========================================

// has - vector (optimized, no SFINAE overhead)
template <typename T>
DAXE_NODISCARD DAXE_ALWAYS_INLINE bool has(const std::vector<T>& v, const T& x) noexcept {
    return std::find(v.begin(), v.end(), x) != v.end();
}

// has - any other container (fallback)
template <typename Container, typename T, typename = std::enable_if_t<
    detail::is_iterable_v<Container> &&
    !std::is_same_v<Container, std::vector<T>>>>
DAXE_NODISCARD inline bool has(const Container& c, const T& x) noexcept {
    return std::find(std::begin(c), std::end(c), x) != std::end(c);
}

// Optimized has for set/map (O(log n))
template <typename T>
DAXE_NODISCARD inline bool has(const std::set<T>& s, const T& x) noexcept { return s.count(x) > 0; }

template <typename T>
DAXE_NODISCARD inline bool has(const std::unordered_set<T>& s, const T& x) noexcept { return s.count(x) > 0; }

template <typename K, typename V>
DAXE_NODISCARD inline bool has(const std::map<K, V>& m, const K& k) noexcept { return m.count(k) > 0; }

template <typename K, typename V>
DAXE_NODISCARD inline bool has(const std::unordered_map<K, V>& m, const K& k) noexcept { return m.count(k) > 0; }

// length - any container with size() (named to avoid 'len' macro conflict)
template <typename Container>
DAXE_NODISCARD inline i64 length(const Container& c) noexcept { return static_cast<i64>(c.size()); }

// empty - any container
template <typename Container>
DAXE_NODISCARD inline bool empty(const Container& c) noexcept { return c.empty(); }

// ==========================================
// FUNCTIONAL PREDICATES
// ==========================================

// all(container, pred) -> true if pred(x) is true for all x
template <typename Container, typename Pred>
DAXE_NODISCARD inline bool all(const Container& c, Pred&& pred) {
    return std::all_of(std::begin(c), std::end(c), std::forward<Pred>(pred));
}

// any(container, pred) -> true if pred(x) is true for any x
template <typename Container, typename Pred>
DAXE_NODISCARD inline bool any(const Container& c, Pred&& pred) {
    return std::any_of(std::begin(c), std::end(c), std::forward<Pred>(pred));
}

// none(container, pred) -> true if pred(x) is true for no x
template <typename Container, typename Pred>
DAXE_NODISCARD inline bool none(const Container& c, Pred&& pred) {
    return std::none_of(std::begin(c), std::end(c), std::forward<Pred>(pred));
}

// count(container, value) -> occurrences
template <typename Container, typename T>
DAXE_NODISCARD inline i64 count(const Container& c, const T& value) {
    return static_cast<i64>(std::count(std::begin(c), std::end(c), value));
}

// ==========================================
// RETURNING NEW COPIES - Past participles
// ==========================================
template <typename T>
DAXE_NODISCARD inline std::vector<T> sorted(std::vector<T> v) {
    std::sort(v.begin(), v.end());
    return v;
}

// Binary search utilities (named to avoid macro conflicts)
template <typename T>
DAXE_NODISCARD inline i64 indexlower(const std::vector<T>& v, const T& x) {
    return static_cast<i64>(std::lower_bound(v.begin(), v.end(), x) - v.begin());
}

template <typename T>
DAXE_NODISCARD inline i64 indexupper(const std::vector<T>& v, const T& x) {
    return static_cast<i64>(std::upper_bound(v.begin(), v.end(), x) - v.begin());
}

template <typename T>
DAXE_NODISCARD inline bool binsearch(const std::vector<T>& v, const T& x) {
    return std::binary_search(v.begin(), v.end(), x);
}

template <typename T>
DAXE_NODISCARD inline std::vector<T> reversed(std::vector<T> v) {
    std::reverse(v.begin(), v.end());
    return v;
}

// Rotate vector left by n positions
template <typename T>
DAXE_NODISCARD inline std::vector<T> rotated(std::vector<T> v, i64 n) {
    if (v.empty()) return v;
    n = ((n % static_cast<i64>(v.size())) + static_cast<i64>(v.size())) % static_cast<i64>(v.size());
    std::rotate(v.begin(), v.begin() + n, v.end());
    return v;
}

// Deduplicate sorted vector
template <typename T>
DAXE_NODISCARD inline std::vector<T> deduplicated(std::vector<T> v) {
    v.erase(std::unique(v.begin(), v.end()), v.end());
    return v;
}

template <typename T, typename Pred>
DAXE_NODISCARD inline std::vector<T> filtered(const std::vector<T>& v, Pred&& pred) {
    std::vector<T> r; r.reserve(v.size());
    for (const auto& x : v) if (pred(x)) r.push_back(x);
    r.shrink_to_fit();
    return r;
}

template <typename T>
DAXE_NODISCARD inline std::vector<T> sliced(const std::vector<T>& v, i64 start, i64 end) {
    const i64 n = static_cast<i64>(v.size());
    if (start < 0) start += n;
    if (end < 0) end += n;
    start = std::max(0LL, std::min(start, n));
    end = std::max(0LL, std::min(end, n));
    if (start >= end) DAXE_UNLIKELY return {};
    return std::vector<T>(v.begin() + start, v.begin() + end);
}

// flatten - convert nested vector to flat
template <typename T>
DAXE_NODISCARD inline std::vector<T> flatten(const std::vector<std::vector<T>>& nested) {
    std::vector<T> result;
    size_t total = 0;
    for (const auto& v : nested) total += v.size();
    result.reserve(total);
    for (const auto& v : nested) result.insert(result.end(), v.begin(), v.end());
    return result;
}

// ==========================================
// MAP UTILITIES
// ==========================================
template <typename K, typename V>
DAXE_NODISCARD inline std::vector<K> keys(const std::map<K, V>& m) {
    std::vector<K> r; r.reserve(m.size());
    for (const auto& [k, v] : m) r.push_back(k);
    return r;
}

// counter - count occurrences of each element
template <typename T>
DAXE_NODISCARD inline std::map<T, i64> counter(const std::vector<T>& v) {
    std::map<T, i64> counts;
    for (const auto& x : v) counts[x]++;
    return counts;
}

template <typename K, typename V>
DAXE_NODISCARD inline std::vector<V> values(const std::map<K, V>& m) {
    std::vector<V> r; r.reserve(m.size());
    for (const auto& [k, v] : m) r.push_back(v);
    return r;
}

// ==========================================
// MATH
// ==========================================
// Math functions moved to daxe/math.h

// ==========================================
// STRING
// ==========================================
DAXE_NODISCARD inline std::vector<str> split(const str& s, const str& sep = " ") {
    std::vector<str> r; r.reserve(16);
    if (sep.empty()) {
        for (char c : s) r.push_back(str(1, c));
        return r;
    }
    size_t start = 0, end;
    while ((end = s.find(sep, start)) != str::npos) {
        r.push_back(s.substr(start, end - start));
        start = end + sep.length();
    }
    r.push_back(s.substr(start));
    return r;
}

DAXE_NODISCARD inline std::vector<str> split(const str& s, char d) {
    return split(s, str(1, d));
}

DAXE_NODISCARD inline str join(const std::vector<str>& v, const str& sep = " ") {
    if (v.empty()) DAXE_UNLIKELY return "";
    str r = v[0];
    for (size_t i = 1; i < v.size(); ++i) { r += sep; r += v[i]; }
    return r;
}

DAXE_NODISCARD inline str lowercase(str s) noexcept {
    for (auto& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}

DAXE_NODISCARD inline str uppercase(str s) noexcept {
    for (auto& c : s) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    return s;
}

DAXE_NODISCARD inline bool startswith(const str& s, const str& prefix) noexcept {
    return s.size() >= prefix.size() && s.rfind(prefix, 0) == 0;
}

DAXE_NODISCARD inline bool endswith(const str& s, const str& suffix) noexcept {
    return s.size() >= suffix.size() && s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}

DAXE_NODISCARD inline str strip(str s) {
    const auto start = s.find_first_not_of(" \t\n\r");
    if (start == str::npos) return "";
    const auto end = s.find_last_not_of(" \t\n\r");
    return s.substr(start, end - start + 1);
}

DAXE_NODISCARD inline str lstrip(str s) {
    const auto start = s.find_first_not_of(" \t\n\r");
    if (start == str::npos) return "";
    return s.substr(start);
}

DAXE_NODISCARD inline str rstrip(str s) {
    const auto end = s.find_last_not_of(" \t\n\r");
    if (end == str::npos) return "";
    return s.substr(0, end + 1);
}

DAXE_NODISCARD inline str repeatstr(const str& s, i64 n) {
    str result;
    result.reserve(s.size() * static_cast<size_t>(n));
    for (i64 i = 0; i < n; ++i) result += s;
    return result;
}

DAXE_NODISCARD inline str replace(str s, const str& old_val, const str& new_val) {
    if (old_val.empty()) return s;
    size_t start = 0;
    while ((start = s.find(old_val, start)) != str::npos) {
        s.replace(start, old_val.length(), new_val);
        start += new_val.length();
    }
    return s;
}

// Substring check
DAXE_NODISCARD inline bool has(const str& s, const str& sub) noexcept {
    return s.find(sub) != str::npos;
}

DAXE_NODISCARD inline bool has(const str& s, char c) noexcept {
    return s.find(c) != str::npos;
}

// String predicates
DAXE_NODISCARD inline bool isalpha(const str& s) noexcept {
    if (s.empty()) return false;
    for (char c : s) if (!std::isalpha(static_cast<unsigned char>(c))) return false;
    return true;
}

DAXE_NODISCARD inline bool isdigit(const str& s) noexcept {
    if (s.empty()) return false;
    for (char c : s) if (!std::isdigit(static_cast<unsigned char>(c))) return false;
    return true;
}

DAXE_NODISCARD inline bool isalnum(const str& s) noexcept {
    if (s.empty()) return false;
    for (char c : s) if (!std::isalnum(static_cast<unsigned char>(c))) return false;
    return true;
}

// capitalize - first letter uppercase, rest lowercase
DAXE_NODISCARD inline str capitalize(str s) noexcept {
    if (!s.empty()) {
        s[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(s[0])));
        for (size_t i = 1; i < s.size(); ++i) 
            s[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(s[i])));
    }
    return s;
}

// title - capitalize first letter of each word
DAXE_NODISCARD inline str title(str s) noexcept {
    bool newword = true;
    for (char& c : s) {
        if (std::isspace(static_cast<unsigned char>(c))) newword = true;
        else if (newword) { c = static_cast<char>(std::toupper(static_cast<unsigned char>(c))); newword = false; }
        else c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return s;
}

// center - pad string to width with fill char
DAXE_NODISCARD inline str center(const str& s, size_t width, char fill = ' ') {
    if (s.size() >= width) return s;
    size_t padding = width - s.size();
    size_t left = padding / 2;
    size_t right = padding - left;
    return str(left, fill) + s + str(right, fill);
}

// reverse - reverse a string
DAXE_NODISCARD inline str reversed(str s) {
    std::reverse(s.begin(), s.end());
    return s;
}

DAXE_NAMESPACE_END

// ==========================================
// FUNCTIONAL
// ==========================================
template <typename T, typename U>
DAXE_NODISCARD inline std::vector<std::pair<T, U>> zip(const std::vector<T>& v1, const std::vector<U>& v2) {
    size_t n = std::min(v1.size(), v2.size());
    std::vector<std::pair<T, U>> result; result.reserve(n);
    for (size_t i = 0; i < n; ++i) {
        result.push_back({v1[i], v2[i]});
    }
    return result;
}

#endif // DAXE_FUNCTIONS_H
