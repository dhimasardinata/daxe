/*
 * DAXE - TIME UTILITIES
 * D.A's Axe - Cut through C++ verbosity
 * 
 * Simple profiling and time functions.
 */

#ifndef DAXE_TIME_H
#define DAXE_TIME_H

#include "base.h"
#include "io.h"
#include "safe.h"
#include <chrono>
#include <thread>

DAXE_NAMESPACE_BEGIN

// ==========================================
// TIME FUNCTIONS
// ==========================================

// now() -> seconds since epoch (high precision)
DAXE_NODISCARD inline f64 now() noexcept {
    using namespace std::chrono;
    return duration<f64>(high_resolution_clock::now().time_since_epoch()).count();
}

// sleep(ms) -> sleep for milliseconds
inline void sleep(i64 rs) noexcept {
    if (rs > 0) std::this_thread::sleep_for(std::chrono::milliseconds(rs));
}

// ==========================================
// TIMER CLASS (Profiling)
// ==========================================
class Timer {
    str name_;
    f64 start_;
public:
    explicit Timer(str name) : name_(std::move(name)), start_(now()) {}
    
    ~Timer() {
        f64 end = now();
        // Use standard io to avoid dependency loop if print() is complex, 
        // but robust io.h usage is preferred.
        // Format: [TIMER] Name: 1.234s
    #if DAXE_HAS_PRINT
        std::print("[TIMER] {}: {:.6f}s\n", name_, end - start_);
    #else
        std::printf("[TIMER] %s: %.6fs\n", name_.c_str(), end - start_);
    #endif
    }
};

DAXE_NAMESPACE_END

#endif // DAXE_TIME_H
