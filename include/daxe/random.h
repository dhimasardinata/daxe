/*
 * DAXE - RANDOM UTILITIES
 * D.A's Axe - Cut through C++ verbosity
 * 
 * Naming: Flat style (no snake_case)
 */

#ifndef DAXE_RANDOM_H
#define DAXE_RANDOM_H

#include "base.h"
#include "safe.h"
#include "macros.h"
#include <random>
#include <chrono>
#include <vector>
#include <algorithm>
#include <numeric>

DAXE_NAMESPACE_BEGIN

class Random {
    std::mt19937_64 rng_;
public:
    Random() {
        rng_.seed(std::chrono::steady_clock::now().time_since_epoch().count());
    }

    explicit Random(u64 seed) {
        rng_.seed(seed);
    }

    // Generic rand(min, max)
    template <typename T>
    T rand(T min, T max) {
        if (min > max) std::swap(min, max);  // Guard against invalid range
        if constexpr (std::is_integral_v<T>) {
            // uniform_int_distribution requires larger integer types
            using DistType = std::conditional_t<(sizeof(T) < 2), int, T>;
            return static_cast<T>(std::uniform_int_distribution<DistType>(static_cast<DistType>(min), static_cast<DistType>(max))(rng_));
        } else {
            return std::uniform_real_distribution<T>(min, max)(rng_);
        }
    }

    // Boolean
    bool randbool(f64 p = 0.5) {
        return std::bernoulli_distribution(p)(rng_);
    }

    // Choice from container - returns Option to handle empty case safely
    template <typename Container>
    auto choice(const Container& c) -> Option<typename Container::value_type> {
        if (c.empty()) DAXE_UNLIKELY return None; 
        auto idx = std::uniform_int_distribution<size_t>(0, c.size() - 1)(rng_);
        return Some(*std::next(std::begin(c), idx));
    }

    // Shuffle container
    template <typename Container>
    void shuffle(Container& c) {
        std::shuffle(std::begin(c), std::end(c), rng_);
    }

    // Sample (k unique elements) - No replacement
    template <typename T>
    std::vector<T> sample(const std::vector<T>& population, size_t k) {
        if (k > population.size()) k = population.size(); // Safe fallback: clamp to max available
        std::vector<T> result; result.reserve(k);
        std::vector<size_t> indices(population.size());
        std::iota(indices.begin(), indices.end(), 0);
        
        // Fisher-Yates partial
        for (size_t i = 0; i < k; ++i) {
            size_t j = std::uniform_int_distribution<size_t>(i, indices.size() - 1)(rng_);
            std::swap(indices[i], indices[j]);
            result.push_back(population[indices[i]]);
        }
        return result;
    }

    // Choices (k elements) - With replacement
    template <typename T>
    std::vector<T> choices(const std::vector<T>& population, size_t k) {
        if (population.empty()) return {}; // Safe fallback: return empty
        std::vector<T> result; result.reserve(k);
        std::uniform_int_distribution<size_t> dist(0, population.size() - 1);
        for (size_t i = 0; i < k; ++i) {
            result.push_back(population[dist(rng_)]);
        }
        return result;
    }
};

#ifndef DAXE_MODULE
// Thread-local instance for thread safety (header-only mode)
inline thread_local Random rng;

// Convenience functions using global instance
template <typename T>
inline T rand(T min, T max) { return rng.rand(min, max); }

inline bool randbool(f64 p = 0.5) { return rng.randbool(p); }

template <typename Container>
inline auto choice(const Container& c) { return rng.choice(c); }

template <typename Container>
inline void shuffle(Container& c) { rng.shuffle(c); }

template <typename T>
inline std::vector<T> sample(const std::vector<T>& p, size_t k) { return rng.sample(p, k); }

template <typename T>
inline std::vector<T> choices(const std::vector<T>& p, size_t k) { return rng.choices(p, k); }
#endif

DAXE_NAMESPACE_END

#endif // DAXE_RANDOM_H
