/*
 * DAXE - CONVERSION UTILITIES
 * D.A's Axe - Cut through C++ verbosity
 * 
 * Safe string parsing and conversion.
 */

#ifndef DAXE_CONVERT_H
#define DAXE_CONVERT_H

#include "base.h"
#include "safe.h"
#include <charconv>
#include <string>

#include <cctype>

DAXE_NAMESPACE_BEGIN

// ==========================================
// TO STRING
// ==========================================
template <typename T>
DAXE_NODISCARD inline str tostr(const T& value) {
    return std::to_string(value);
}

// Specialization for string itself
DAXE_NODISCARD inline str tostr(const str& value) { return value; }
DAXE_NODISCARD inline str tostr(const char* value) { return str(value); }

// ==========================================
// SAFE PARSING
// ==========================================

// Generic parse stub
template <typename T = i64>
DAXE_NODISCARD inline Result<T> parse(const str& s) {
    // Default implementation for arithmetic types using from_chars (C++17)
    if constexpr (std::is_arithmetic_v<T>) {
        if (s.empty()) return Result<T>::Err("empty string");
        
        // Skip whitespace
        const char* first = s.data();
        const char* last = s.data() + s.size();
        while (first != last && std::isspace(static_cast<unsigned char>(*first))) ++first;
        if (first == last) return Result<T>::Err("empty/whitespace string");
        while (last != first && std::isspace(static_cast<unsigned char>(*(last - 1)))) --last;

        T val{};
        auto [ptr, ec] = std::from_chars(first, last, val);
        if (ec == std::errc()) {
            if (ptr == last) return Result<T>::Ok(val); // Parsed entire trimmed string
            return Result<T>::Err("partial parse");
        }
        if (ec == std::errc::invalid_argument) return Result<T>::Err("invalid number");
        if (ec == std::errc::result_out_of_range) return Result<T>::Err("out of range");
        return Result<T>::Err("parse error");
    } else {
        static_assert(std::is_same_v<T, void>, "parse<T> not implemented for this type");
        return Result<T>::Err("unsupported type");
    }
}

// Specializations if needed (e.g. bool)
// parse<bool>("true") -> true

template <>
DAXE_NODISCARD inline Result<bool> parse<bool>(const str& s) {
    if (s == "true" || s == "1") return Result<bool>::Ok(true);
    if (s == "false" || s == "0") return Result<bool>::Ok(false);
    return Result<bool>::Err("invalid boolean");
}

DAXE_NAMESPACE_END

#endif // DAXE_CONVERT_H
