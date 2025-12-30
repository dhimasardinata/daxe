#include <daxe/core.h>
#include <cassert>

using namespace dax;

int main() {
    // Types
    i64 a = 10;
    f64 b = 3.14;
    
    // Math
    assert(clamp(a, 0LL, 5LL) == 5);
    assert(sign(-5) == -1);
    assert(gcd(12, 18) == 6);
    
    // Option
    Option<i64> opt = Some(42LL);
    assert(opt.has_value());
    
    // Vec
    vi64 v = {1, 2, 3};
    assert(v.size() == 3);
    
    (void)b; // Suppress warning
    
    return 0;
}
