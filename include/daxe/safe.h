/*
 * DAXE - SAFE TYPES (Rust-inspired)
 * D.A's Axe - Cut through C++ verbosity
 * 
 * Features:
 * - Option<T>: Monadic null safety (map, then, otherwise)
 * - Result<T, E>: Error handling (isok, iserr, unwrap)
 * - Safety: defer, panic, todo, unreachable
 */

#ifndef DAXE_SAFE_H
#define DAXE_SAFE_H

#include "base.h"
#include <optional>
#include <variant>
#include <functional>
#include <stdexcept>
#include <iostream>
#include <cmath>

DAXE_NAMESPACE_BEGIN

// ==========================================
// PANIC & TODO
// ==========================================
[[noreturn]] inline void panic(const str& msg) {
    std::cerr << "[DAXE PANIC]: " << msg << std::endl;
    std::terminate();
}

[[noreturn]] inline void todo(const str& msg = "not implemented") {
    panic("TODO: " + msg);
}

[[noreturn]] inline void unreachable(const str& msg = "entered unreachable code") {
    panic("UNREACHABLE: " + msg);
}

// ==========================================
// DEFER (Go/Zig style)
// ==========================================
namespace detail {
    template <typename F>
    struct Deferrer {
        F f;
        ~Deferrer() { f(); }
    };
    struct DeferHelper {};
    template <typename F>
    Deferrer<F> operator+(DeferHelper, F&& f) { return {std::forward<F>(f)}; }
}

#define DAXE_DEFER_1(x, y) x##y
#define DAXE_DEFER_2(x, y) DAXE_DEFER_1(x, y)
#define DAXE_DEFER_3(x)    DAXE_DEFER_2(x, __COUNTER__)
#define defer              auto DAXE_DEFER_3(_defer_) = dax::detail::DeferHelper{} + [&]()

// ==========================================
// FORWARD DECLARATIONS
// ==========================================
template <typename T> struct Option;
inline constexpr std::nullopt_t None = std::nullopt;

// ==========================================
// OPTION TYPE
// ==========================================
template <typename T>
struct Option : public std::optional<T> {
    using Base = std::optional<T>;
    using Base::Base;
    
    // Allow implicit construction from T
    constexpr Option(const T& v) : Base(v) {}
    constexpr Option(T&& v) : Base(std::move(v)) {}
    constexpr Option(std::nullopt_t) : Base(std::nullopt) {}
    constexpr Option() : Base(std::nullopt) {}

    // Methods
    DAXE_NODISCARD constexpr bool issome() const noexcept { return this->has_value(); }
    DAXE_NODISCARD constexpr bool isnone() const noexcept { return !this->has_value(); }
    
    DAXE_NODISCARD constexpr T unwrap() const {
        if (isnone()) panic("called unwrap() on None Option");
        return this->value();
    }
    
    DAXE_NODISCARD constexpr T unwrap(const char* msg) const {
        if (isnone()) panic(msg);
        return this->value();
    }
    
    // map: Transform T -> U
    template <typename F>
    DAXE_NODISCARD constexpr auto map(F&& f) const {
        using U = std::invoke_result_t<F, const T&>;
        if (issome()) return Option<U>(f(**this));
        return Option<U>(None);
    }
    
    // then: Chain Option<T> -> Option<U> (monadic bind)
    template <typename F>
    DAXE_NODISCARD constexpr auto then(F&& f) const {
        if (issome()) return f(**this);
        using U = typename std::invoke_result_t<F, const T&>::value_type;
        return Option<U>(None);
    }
    
    // otherwise: Default value (lazy)
    template <typename F>
    DAXE_NODISCARD constexpr T otherwise(F&& f) const {
        if (issome()) return **this;
        return f();
    }
    
    // valueor: Default value (eager)
    DAXE_NODISCARD constexpr T valueor(T def) const {
        return this->value_or(std::move(def));
    }

    // Explicitly ignore the value to suppress nodiscard warnings
    constexpr void ignore() const noexcept {}
};

// Helper for Some construction
template <typename T>
DAXE_NODISCARD constexpr Option<std::decay_t<T>> Some(T&& value) noexcept {
    return Option<std::decay_t<T>>(std::forward<T>(value));
}

// ==========================================
// RESULT TYPE
// ==========================================
template <typename T, typename E = str>
struct Result {
    std::variant<T, E> data_;

    constexpr Result(T value) : data_(std::in_place_index<0>, std::move(value)) {}
    constexpr Result(std::in_place_index_t<1>, E error) : data_(std::in_place_index<1>, std::move(error)) {}

    static constexpr Result Ok(T value) { return Result(std::move(value)); }
    static constexpr Result Err(E error) { return Result(std::in_place_index<1>, std::move(error)); }

    DAXE_NODISCARD constexpr bool isok() const noexcept { return data_.index() == 0; }
    DAXE_NODISCARD constexpr bool iserr() const noexcept { return data_.index() == 1; }
    
    DAXE_NODISCARD constexpr T unwrap() const {
        if (iserr()) panic("called unwrap() on Err Result");
        return std::get<0>(data_);
    }
    
    DAXE_NODISCARD constexpr T unwrap(const char* msg) const {
        if (iserr()) panic(msg);
        return std::get<0>(data_);
    }
    
    DAXE_NODISCARD constexpr E error() const {
        if (isok()) panic("called error() on Ok Result");
        return std::get<1>(data_);
    }
    
    // map: Transform T -> U
    template <typename F>
    DAXE_NODISCARD auto map(F&& f) const {
        using U = std::invoke_result_t<F, const T&>;
        if (isok()) return Result<U, E>::Ok(f(std::get<0>(data_)));
        return Result<U, E>::Err(std::get<1>(data_));
    }
    
    // then: Chain Result<T, E> -> Result<U, E>
    template <typename F>
    DAXE_NODISCARD auto then(F&& f) const {
        if (isok()) return f(std::get<0>(data_));
        using Ret = std::invoke_result_t<F, const T&>;
        // using U = typename Ret::type_val; // Unused
        return Ret::Err(std::get<1>(data_));
    }
    
    // otherwise: Lazy recovery
    template <typename F>
    DAXE_NODISCARD constexpr T otherwise(F&& f) const {
        if (isok()) return std::get<0>(data_);
        return f(std::get<1>(data_));
    }
    
    // Explicitly ignore to suppress nodiscard
    constexpr void ignore() const noexcept {}
    
    // Type helpers
    using type_val = T;
    using type_err = E;
};

template <typename T>
DAXE_NODISCARD constexpr auto Ok(T&& value) {
    return Result<std::decay_t<T>, str>::Ok(std::forward<T>(value));
}

template <typename E>
DAXE_NODISCARD constexpr auto Err(E&& error) {
    // Note: Type T is unknown here, usually usage implies T via assignment or specific construction
    // This helper is partial. Usually prefer Result<T>::Err
    return Result<int, std::decay_t<E>>::Err(std::forward<E>(error)); // Fallback int generic
}

// Result<T> specialized helper for deducing E=str
template <typename T>
DAXE_NODISCARD constexpr Result<T, str> OkVal(T val) { return Result<T, str>::Ok(val); }


// ==========================================
// UNIVERSAL SAFE ACCESS
// ==========================================

// Safe helpers moved to free functions that return Option<T>
// ... (Copied from previous safe.h but returning new Option<T>)

template <typename Container>
DAXE_NODISCARD inline auto getat(const Container& c, i64 idx) noexcept 
    -> Option<typename Container::value_type> {
    i64 n = static_cast<i64>(c.size());
    if (idx < 0) idx += n;
    if (idx < 0 || idx >= n) return None;
    auto it = std::begin(c);
    std::advance(it, idx);
    return Some(*it);
}

template <typename Container, typename T>
DAXE_NODISCARD inline auto getor(const Container& c, i64 idx, T def) noexcept -> T {
    return getat(c, idx).valueor(std::move(def));
}

// String
DAXE_NODISCARD inline Option<char> charat(const str& s, i64 idx) noexcept {
    i64 n = static_cast<i64>(s.size());
    if (idx < 0) idx += n;
    if (idx < 0 || idx >= n) return None;
    return Some(s[static_cast<size_t>(idx)]);
}

// Math
DAXE_NODISCARD inline Result<i64> trydiv(i64 a, i64 b) noexcept {
    if (b == 0) return Result<i64>::Err("Division by zero");
    return Result<i64>::Ok(a / b);
}

DAXE_NODISCARD inline Result<i64> trymod(i64 a, i64 b) noexcept {
    if (b == 0) return Result<i64>::Err("Modulo by zero");
    return Result<i64>::Ok(a % b);
}

DAXE_NODISCARD inline Result<f64> trysqrt(f64 x) noexcept {
    if (x < 0) return Result<f64>::Err("Negative sqrt");
    return Result<f64>::Ok(std::sqrt(x));
}

// Compatibility Functions
template <typename T>
DAXE_NODISCARD constexpr bool issome(const Option<T>& opt) noexcept { return opt.issome(); }
template <typename T>
DAXE_NODISCARD constexpr bool isnone(const Option<T>& opt) noexcept { return opt.isnone(); }
template <typename T>
DAXE_NODISCARD constexpr T unwrap(const Option<T>& opt) { return opt.unwrap(); }
template <typename T>
DAXE_NODISCARD constexpr T valueor(const Option<T>& opt, T def) noexcept { return opt.valueor(def); }

// Bounds
template <typename T>
DAXE_NODISCARD constexpr bool inbounds(T value, T low, T high) noexcept {
    return value >= low && value < high;
}

template <typename T>
DAXE_NODISCARD constexpr T clamp(T value, T low, T high) noexcept {
    return value < low ? low : (value > high ? high : value);
}

DAXE_NODISCARD constexpr bool ingrid(i64 r, i64 c, i64 rows, i64 cols) noexcept {
    return static_cast<u64>(r) < static_cast<u64>(rows) && 
           static_cast<u64>(c) < static_cast<u64>(cols);
}

DAXE_NAMESPACE_END

#endif // DAXE_SAFE_H
