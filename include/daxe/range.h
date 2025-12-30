/*
 * DAXE - RANGE & ITERATION UTILITIES
 * D.A's Axe - Cut through C++ verbosity
 * 
 * Naming: Flat style (no snake_case)
 */

#ifndef DAXE_RANGE_H
#define DAXE_RANGE_H

#include "base.h"
#include "safe.h"

DAXE_NAMESPACE_BEGIN

// ==========================================
// RANGE CLASS
// ==========================================
class Range {
    i64 start_, end_, step_;
public:
    constexpr Range(i64 end) noexcept : start_(0), end_(end), step_(1) {}
    constexpr Range(i64 start, i64 end) noexcept : start_(start), end_(end), step_(1) {}
    constexpr Range(i64 start, i64 end, i64 step) noexcept : start_(start), end_(end), step_(step == 0 ? 1 : step) {}
    
    class Iterator {
        i64 current_, step_;
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = i64;
        using difference_type = i64;
        constexpr Iterator(i64 val, i64 step) noexcept : current_(val), step_(step) {}
        DAXE_NODISCARD constexpr i64 operator*() const noexcept { return current_; }
        constexpr Iterator& operator++() noexcept { current_ += step_; return *this; }
        DAXE_NODISCARD constexpr bool operator!=(const Iterator& o) const noexcept {
            if (step_ > 0) DAXE_LIKELY return current_ < o.current_;
            return current_ > o.current_;
        }
        DAXE_NODISCARD constexpr bool operator==(const Iterator& o) const noexcept { return !(*this != o); }
    };
    
    DAXE_NODISCARD constexpr Iterator begin() const noexcept { return Iterator(start_, step_); }
    DAXE_NODISCARD constexpr Iterator end() const noexcept { return Iterator(end_, step_); }
    DAXE_NODISCARD constexpr i64 size() const noexcept {
        if (step_ > 0 && end_ > start_) return (end_ - start_ + step_ - 1) / step_;
        if (step_ < 0 && start_ > end_) return (start_ - end_ - step_ - 1) / (-step_);
        return 0;
    }
};

DAXE_NODISCARD constexpr Range range(i64 end) noexcept { return Range(end); }
DAXE_NODISCARD constexpr Range range(i64 start, i64 end) noexcept { return Range(start, end); }
DAXE_NODISCARD constexpr Range range(i64 start, i64 end, i64 step) noexcept { return Range(start, end, step); }

// ==========================================
// ENUMERATE
// ==========================================
template <typename Container>
class Enumerate {
    Container container_;
    i64 start_;
public:
    constexpr Enumerate(Container c, i64 start = 0) noexcept : container_(std::forward<Container>(c)), start_(start) {}
    
    // safe begin/end that handles both const and non-const
    auto begin() noexcept { 
        using std::begin;
        return Iterator(begin(container_), start_); 
    }
    auto end() noexcept { 
        using std::begin; using std::end;
        return Iterator(end(container_), start_ + static_cast<i64>(std::distance(begin(container_), end(container_)))); 
    }

    class Iterator {
        // use auto to deduce iterator type (const_iterator vs iterator)
        decltype(std::begin(std::declval<std::remove_reference_t<Container>&>())) it_;
        i64 idx_;
    public:
        using iter_type = decltype(it_);
        constexpr Iterator(iter_type it, i64 idx) noexcept : it_(it), idx_(idx) {}
        
        DAXE_NODISCARD constexpr auto operator*() noexcept { return std::pair<i64, decltype(*it_)>{idx_, *it_}; }
        constexpr Iterator& operator++() noexcept { ++it_; ++idx_; return *this; }
        DAXE_NODISCARD constexpr bool operator!=(const Iterator& o) const noexcept { return it_ != o.it_; }
    };
};

template <typename Container>
DAXE_NODISCARD auto enumerate(Container&& c, i64 start = 0) noexcept -> Enumerate<Container> { 
    return Enumerate<Container>(std::forward<Container>(c), start); 
}

// ==========================================
// ZIP
// ==========================================
// ==========================================
// ZIP RANGE (Lazy)
// ==========================================
template <typename C1, typename C2>
class ZipRange {
    C1 c1_; C2 c2_;
public:
    constexpr ZipRange(C1 c1, C2 c2) noexcept : c1_(std::forward<C1>(c1)), c2_(std::forward<C2>(c2)) {}
    
    auto begin() noexcept {
        using std::begin;
        return Iterator(begin(c1_), begin(c2_));
    }
    
    auto end() noexcept {
        using std::end;
        return Iterator(end(c1_), end(c2_));
    }

    class Iterator {
        decltype(std::begin(std::declval<std::remove_reference_t<C1>&>())) it1_;
        decltype(std::begin(std::declval<std::remove_reference_t<C2>&>())) it2_;
    public:
        using iter1_type = decltype(it1_);
        using iter2_type = decltype(it2_);
        
        constexpr Iterator(iter1_type i1, iter2_type i2) noexcept : it1_(i1), it2_(i2) {}
        DAXE_NODISCARD constexpr auto operator*() noexcept { return std::pair<decltype(*it1_), decltype(*it2_)>{*it1_, *it2_}; }
        constexpr Iterator& operator++() noexcept { ++it1_; ++it2_; return *this; }
        DAXE_NODISCARD constexpr bool operator!=(const Iterator& o) const noexcept { return it1_ != o.it1_ && it2_ != o.it2_; }
    };
};

template <typename C1, typename C2>
DAXE_NODISCARD auto ziprange(C1&& c1, C2&& c2) noexcept -> ZipRange<C1, C2> { 
    return ZipRange<C1, C2>(std::forward<C1>(c1), std::forward<C2>(c2)); 
}

// ==========================================
// FUNCTIONAL UTILITIES - Flat naming
// ==========================================
template <typename T> 
DAXE_NODISCARD inline std::vector<T> repeat(const T& value, i64 n) { 
    return std::vector<T>(static_cast<size_t>(n), value); 
}

template <typename T> 
DAXE_NODISCARD inline std::vector<T> chain(const std::vector<T>& a, const std::vector<T>& b) { 
    std::vector<T> r; r.reserve(a.size() + b.size()); 
    r.insert(r.end(), a.begin(), a.end()); 
    r.insert(r.end(), b.begin(), b.end()); 
    return r; 
}

template <typename T> 
DAXE_NODISCARD inline std::vector<T> take(const std::vector<T>& v, i64 n) { 
    n = std::min(n, static_cast<i64>(v.size())); 
    return std::vector<T>(v.begin(), v.begin() + n); 
}

template <typename T> 
DAXE_NODISCARD inline std::vector<T> drop(const std::vector<T>& v, i64 n) { 
    n = std::min(n, static_cast<i64>(v.size())); 
    return std::vector<T>(v.begin() + n, v.end()); 
}

template <typename T, typename Pred>
DAXE_NODISCARD inline Option<T> findif(const std::vector<T>& v, Pred&& pred) noexcept {
    for (const auto& x : v) if (pred(x)) DAXE_LIKELY return Some(x);
    return None;
}

template <typename T, typename Pred>
DAXE_NODISCARD inline std::vector<T> filter(const std::vector<T>& v, Pred&& pred) {
    std::vector<T> r; r.reserve(v.size());
    for (const auto& x : v) if (pred(x)) r.push_back(x);
    r.shrink_to_fit();
    return r;
}

template <typename T, typename Func>
DAXE_NODISCARD inline auto transform(const std::vector<T>& v, Func&& f) -> std::vector<decltype(f(std::declval<T>()))> {
    using U = decltype(f(std::declval<T>()));
    std::vector<U> r; r.reserve(v.size());
    for (const auto& x : v) r.push_back(f(x));
    return r;
}

template <typename T, typename Func>
DAXE_NODISCARD inline T reduce(const std::vector<T>& v, Func&& f) {
    if (v.empty()) DAXE_UNLIKELY return T{};
    T r = v[0];
    for (size_t i = 1; i < v.size(); ++i) r = f(r, v[i]);
    return r;
}

template <typename T, typename U, typename Func>
DAXE_NODISCARD inline U fold(const std::vector<T>& v, U init, Func&& f) {
    for (const auto& x : v) init = f(std::move(init), x);
    return init;
}

DAXE_NAMESPACE_END

#endif // DAXE_RANGE_H
