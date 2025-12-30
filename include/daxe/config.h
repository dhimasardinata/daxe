/*
 * DAXE - CONFIGURATION & VERSION DETECTION
 * D.A's Axe - Cut through C++ verbosity
 * 
 * Supports C++11 through C++26 with automatic fallbacks
 */

#ifndef DAXE_CONFIG_H
#define DAXE_CONFIG_H

// ==========================================
// VERSION
// ==========================================
#define DAXE_VERSION_MAJOR 0
#define DAXE_VERSION_MINOR 1
#define DAXE_VERSION_PATCH 0
#define DAXE_VERSION_STRING "0.1.0"

// ==========================================
// C++ VERSION DETECTION
// ==========================================
#if defined(_MSVC_LANG)
    #define DAXE_CPLUSPLUS _MSVC_LANG
#else
    #define DAXE_CPLUSPLUS __cplusplus
#endif

#define DAXE_CPP11 (DAXE_CPLUSPLUS >= 201103L)
#define DAXE_CPP14 (DAXE_CPLUSPLUS >= 201402L)
#define DAXE_CPP17 (DAXE_CPLUSPLUS >= 201703L)
#define DAXE_CPP20 (DAXE_CPLUSPLUS >= 202002L)
#define DAXE_CPP23 (DAXE_CPLUSPLUS >= 202302L)
#define DAXE_CPP26 (DAXE_CPLUSPLUS >= 202600L)

// ==========================================
// FEATURE DETECTION MACROS
// ==========================================

// [[nodiscard]] - C++17+
#if DAXE_CPP17
    #define DAXE_NODISCARD [[nodiscard]]
#else
    #define DAXE_NODISCARD
#endif

// [[nodiscard("reason")]] - C++20+
#if DAXE_CPP20
    #define DAXE_NODISCARD_MSG(msg) [[nodiscard(msg)]]
#else
    #define DAXE_NODISCARD_MSG(msg) DAXE_NODISCARD
#endif

// [[likely]] / [[unlikely]] - C++20+
#if DAXE_CPP20
    #define DAXE_LIKELY [[likely]]
    #define DAXE_UNLIKELY [[unlikely]]
#else
    #define DAXE_LIKELY
    #define DAXE_UNLIKELY
#endif

// [[assume]] - C++23+
#if DAXE_CPP23 && defined(__cpp_lib_assume)
    #define DAXE_ASSUME(expr) [[assume(expr)]]
#elif defined(__GNUC__)
    #define DAXE_ASSUME(expr) __builtin_assume(expr)
#elif defined(_MSC_VER)
    #define DAXE_ASSUME(expr) __assume(expr)
#else
    #define DAXE_ASSUME(expr) ((void)0)
#endif

// consteval - C++20+
#if DAXE_CPP20
    #define DAXE_CONSTEVAL consteval
#else
    #define DAXE_CONSTEVAL constexpr
#endif

// constinit - C++20+
#if DAXE_CPP20
    #define DAXE_CONSTINIT constinit
#else
    #define DAXE_CONSTINIT
#endif

// Concepts support - C++20+
#if DAXE_CPP20 && defined(__cpp_concepts)
    #define DAXE_HAS_CONCEPTS 1
#else
    #define DAXE_HAS_CONCEPTS 0
#endif

// Ranges support - C++20+
#if DAXE_CPP20 && defined(__cpp_lib_ranges)
    #define DAXE_HAS_RANGES 1
#else
    #define DAXE_HAS_RANGES 0
#endif

// std::expected - C++23+
#if DAXE_CPP23 && defined(__cpp_lib_expected)
    #define DAXE_HAS_EXPECTED 1
#else
    #define DAXE_HAS_EXPECTED 0
#endif

// std::print - C++23+
#if DAXE_CPP23 && defined(__cpp_lib_print)
    #define DAXE_HAS_PRINT 1
#else
    #define DAXE_HAS_PRINT 0
#endif

// std::mdspan - C++23+
#if DAXE_CPP23 && defined(__cpp_lib_mdspan)
    #define DAXE_HAS_MDSPAN 1
#else
    #define DAXE_HAS_MDSPAN 0
#endif

// Static lambda - C++23+
#if DAXE_CPP23
    #define DAXE_STATIC_LAMBDA static
#else
    #define DAXE_STATIC_LAMBDA
#endif

// ==========================================
// COMPILER DETECTION
// ==========================================
#if defined(__clang__)
    #define DAXE_CLANG 1
    #define DAXE_GCC 0
    #define DAXE_MSVC 0
#elif defined(__GNUC__)
    #define DAXE_CLANG 0
    #define DAXE_GCC 1
    #define DAXE_MSVC 0
#elif defined(_MSC_VER)
    #define DAXE_CLANG 0
    #define DAXE_GCC 0
    #define DAXE_MSVC 1
#else
    #define DAXE_CLANG 0
    #define DAXE_GCC 0
    #define DAXE_MSVC 0
#endif

// ==========================================
// ATTRIBUTE HELPERS
// ==========================================
#if DAXE_GCC || DAXE_CLANG
    #define DAXE_ALWAYS_INLINE __attribute__((always_inline)) inline
    #define DAXE_NOINLINE __attribute__((noinline))
    #define DAXE_FLATTEN __attribute__((flatten))
    #define DAXE_PURE __attribute__((pure))
    #define DAXE_CONST __attribute__((const))
    #define DAXE_RESTRICT __restrict__
#elif DAXE_MSVC
    #define DAXE_ALWAYS_INLINE __forceinline
    #define DAXE_NOINLINE __declspec(noinline)
    #define DAXE_FLATTEN
    #define DAXE_PURE
    #define DAXE_CONST
    #define DAXE_RESTRICT __restrict
#else
    #define DAXE_ALWAYS_INLINE inline
    #define DAXE_NOINLINE
    #define DAXE_FLATTEN
    #define DAXE_PURE
    #define DAXE_CONST
    #define DAXE_RESTRICT
#endif

// ==========================================
// NAMESPACE CONFIGURATION
// ==========================================
#ifndef DAXE_NO_NAMESPACE
    // Module support
    #ifndef DAXE_EXPORT
        #define DAXE_EXPORT
    #endif

    #define DAXE_NAMESPACE_BEGIN DAXE_EXPORT namespace dax {
    #define DAXE_NAMESPACE_END }
    #define DAXE_USING using namespace dax;
#else
    #define DAXE_NAMESPACE_BEGIN
    #define DAXE_NAMESPACE_END
    #define DAXE_USING
#endif

// ==========================================
// DEBUG/RELEASE DETECTION
// ==========================================
#if defined(NDEBUG) || defined(_NDEBUG)
    #define DAXE_RELEASE 1
    #define DAXE_DEBUG 0
#else
    #define DAXE_RELEASE 0
    #define DAXE_DEBUG 1
#endif

#if DAXE_DEBUG
    #define DAXE_DEBUG_ONLY(x) x
#else
    #define DAXE_DEBUG_ONLY(x)
#endif

// ==========================================
// PLATFORM DETECTION
// ==========================================
#if defined(_WIN32) || defined(_WIN64)
    #define DAXE_WINDOWS 1
    #define DAXE_LINUX 0
    #define DAXE_MACOS 0
#elif defined(__linux__)
    #define DAXE_WINDOWS 0
    #define DAXE_LINUX 1
    #define DAXE_MACOS 0
#elif defined(__APPLE__)
    #define DAXE_WINDOWS 0
    #define DAXE_LINUX 0
    #define DAXE_MACOS 1
#else
    #define DAXE_WINDOWS 0
    #define DAXE_LINUX 0
    #define DAXE_MACOS 0
#endif

#endif // DAXE_CONFIG_H
