module;
#include <vector>
#include <utility>
#include <string>
#include <tuple>
#include <algorithm>
#include <cstdint>

export module daxe.compat;

// Export CP Types directly
export {
    using ll = long long;
    using ull = unsigned long long;
    using ld = double;
    using pii = std::pair<int, int>;
    using pll = std::pair<long long, long long>;
    using vll = std::vector<long long>;
    using vvll = std::vector<std::vector<long long>>;
    using vs = std::vector<std::string>;
    using vvs = std::vector<std::vector<std::string>>;
}

// Export CP Utility Functions
export {
    template<typename T>
    constexpr bool ckmin(T& a, const T& b) noexcept { 
        if (b < a) { a = b; return true; } 
        return false; 
    }

    template<typename T>
    constexpr bool ckmax(T& a, const T& b) noexcept { 
        if (b > a) { a = b; return true; } 
        return false; 
    }

    // Function replacements for common macros
    
    // sz(x) -> x.size()
    template<typename C>
    constexpr auto sz(const C& c) { return static_cast<long long>(c.size()); }

    // pb(val) -> requires object.pb(val) which impossible. 
    // We provide func: pb(vec, val)
    template<typename C, typename V>
    constexpr void pb(C& c, V&& v) { c.push_back(std::forward<V>(v)); }
    
    // eb(vec, val...)
    template<typename C, typename... Args>
    constexpr void eb(C& c, Args&&... args) { c.emplace_back(std::forward<Args>(args)...); }
    
    // all(x) -> x.begin(), x.end() (Cannot be function, must be macro for args expansion? No, ranges?)
    // In C++20 we typically use ranges. But for legacy sort(all(x)):
    // We can't easily export 'all'.
    
    // sor(x) -> sort(all(x)) replacement: sort(x)
    template<typename C>
    void sor(C& c) { std::sort(c.begin(), c.end()); }
    
    template<typename C>
    void rsor(C& c) { std::sort(c.begin(), c.end(), std::greater<>()); }

    template<typename C>
    void rev(C& c) { std::reverse(c.begin(), c.end()); }
}
