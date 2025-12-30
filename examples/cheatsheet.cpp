#include <daxe.h>
#include <daxe/compat.h>
#include <daxe/english.h>

using namespace dax;

// ==========================================
// 1. Functions (fx, fn, func)
// ==========================================
fx square(i64 x) { 
    return x * x; // Returns i64
}

fx greet(str name) {
    print("Hello,", name); // Returns void
}

// ==========================================
// 2. Main Logic
// ==========================================
int main() {
    // 3. Variables (var, let)
    var x = 10;
    let pi = 3.14159;
    
    // 4. Containers (Bracket-less defaults)
    List nums = {1, 2, 3};  // List<i64>
    Dict data;              // Dict<str, i64>
    Set unique;             // Set<i64>
    Vec v;                  // std::vector<i64>

    // 5. Usage
    nums.append(square(x));
    data["result"] = nums.sum();
    unique.add(42);
    
    // 6. Output
    greet("Daxe User");
    print("List:", nums);
    print("Dict:", data);
    print("Set:", unique);
    
    return 0;
}
