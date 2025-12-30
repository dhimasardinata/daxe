#include <daxe.h>
#include <daxe/compat.h>
#include <daxe/english.h>
#include <cassert>

using namespace dax;

fx testmath() {
    // clamp (from safe.h)
    assert(clamp(5, 0, 10) == 5);
    assert(clamp(-5, 0, 10) == 0);
    assert(clamp(15, 0, 10) == 10);
    
    // lerp
    assert(lerp(0, 10, 0.5) == 5);
    assert(lerp(0, 100, 0.25) == 25);
    
    // sign
    assert(sign(42) == 1);
    assert(sign(-42) == -1);
    assert(sign(0) == 0);
    
    // absval
    assert(absval(-5) == 5);
    assert(absval(5) == 5);
    assert(absval(0) == 0);
    
    print("Math tests passed.");
}

fx teststring() {
    // lstrip
    assert(lstrip("  hello") == "hello");
    assert(lstrip("hello  ") == "hello  ");
    
    // rstrip
    assert(rstrip("hello  ") == "hello");
    assert(rstrip("  hello") == "  hello");
    
    // repeatstr
    assert(repeatstr("ab", 3) == "ababab");
    assert(repeatstr("x", 0) == "");
    
    print("String tests passed.");
}

fx testcontainer() {
    // List::unique
    List l = {1, 1, 2, 2, 3, 3, 3};
    l.unique();
    assert(l.size() == 3);
    assert(l[0] == 1 && l[1] == 2 && l[2] == 3);
    
    // Dict::merge
    Dict d1;
    d1["a"] = 1;
    d1["b"] = 2;
    Dict d2;
    d2["b"] = 20;
    d2["c"] = 3;
    d1.merge(d2);
    assert(d1["a"] == 1);
    assert(d1["b"] == 20); // Overwritten
    assert(d1["c"] == 3);
    
    // Set::symmetricdiff
    Set s1 = {1, 2, 3};
    Set s2 = {2, 3, 4};
    var sd = s1.symmetricdiff(s2);
    assert(sd.has(1));
    assert(sd.has(4));
    assert(!sd.has(2));
    assert(!sd.has(3));
    
    print("Container tests passed.");
}

int main() {
    print("Running Core Utility Tests...");
    testmath();
    teststring();
    testcontainer();
    print("All tests passed!");
    return 0;
}
