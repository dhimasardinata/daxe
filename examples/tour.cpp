/*
 * DAXE TOUR - A comprehensive example of "Zero-Overhead Pythonic C++"
 * 
 * Demonstrates:
 * 1. Strict Aliases (var, let, fx) & Strings
 * 2. Containers & Iteration (including new Deque/Stack/Queue)
 * 3. Functional Algorithms (including Zip & Filtering)
 * 4. Safe Types (Option/Result)
 * 5. Data Science (Stats & Random Sampling)
 * 6. File I/O
 * 7. Profiling (RAII)
 */

#include <daxe.h>
#include <daxe/compat.h> // Enable CP shortcuts (rep, pb, etc.)
#include <daxe/english.h> // var, let, fx

using namespace dax;

// A simple structure
struct User {
    str name;
    i64 score;
};

std::ostream& operator<<(std::ostream& os, const User& u) {
    return os << "User(" << u.name << ", " << u.score << ")";
}

fx section(const str& title) {
    print("\n= " + title + " " + str(40 - title.size(), '='));
}

int main() {
    section("1. STRICT ALIASES & I/O");
    let msg = "Hello, Daxe!";
    print(msg); 
    
    var raw = "  DATA: 10, 20, 30  ";
    var clean = strip(raw);
    
    if (startswith(clean, "DATA:")) {
        var nums_part = strip(replace(clean, "DATA:", ""));
        Vec<i64> nums; // Default is i64
        for (var part : split(nums_part, ',')) {
            // Safe parsing with Option/Result chain
            parse<i64>(strip(part)).then([&](i64 n) {
                nums.pb(n);
                return Ok(n);
            }).ignore();
        }
        print("Parsed nums:", nums);
    }

    section("2. PYTHONIC CONTAINERS");
    // Bracket-less initialization & Type Deduction
    List l = {5, 2, 8, 1, 9}; // List<i64>
    Dict d;                   // Dict<str, i64>
    d["alice"] = 100;
    
    // Deque with rotate
    Deque dq = {1, 2, 3, 4, 5};
    dq.rotate(2);
    print("Rotated Deque (2):", dq); // 4 5 1 2 3
    
    // Stack with clear/pop-value
    Stack s;
    s.push(10); s.push(20);
    print("Popped from stack:", s.pop()); // 20
    s.clear();
    if (s.isempty()) print("Stack cleared.");

    section("3. FUNCTIONAL ALGO & ZIP");
    l.sort(); // In-place
    l.reverse();
    print("Sorted Desc:", l);
    
    // Zip Iteration
    Vec<str> names = {"Alice", "Bob", "Charlie"};
    Vec<i64> ages = {25, 30, 35};
    
    print("Zipped:");
    for (const auto& [name, age] : zip(names, ages)) {
        print("  -", name, "is", age);
    }
    
    // Predicates
    if (any(l, [](i64 x){ return x > 5; })) print("Found value > 5");
    var evens = l.filter([](i64 x){ return iseven(x); });
    print("Evens:", evens);

    section("4. SAFETY (OPTION/RESULT)");
    Option<i64> head = l.getat(0);  // Safe access
    Option<i64> ghost = l.getat(100); // Out of bounds
    
    print("First:", valueor(head, -1LL));
    print("Ghost:", valueor(ghost, -1LL)); // Default fallback
    
    // Dict safe access
    print("Get 'alice':", d.get("alice", -1));
    print("Get 'bob' (default):", d.get("bob", -1));

    section("5. DATA SCIENCE (STATS & RANDOM)");
    // Statistics
    Vec<i64> data = {1, 2, 2, 3, 4, 5, 100}; // 100 is outlier
    print("Data:", data);
    print("  Mean:  ", mean(data));   // ~16.7
    print("  Median:", median(data)); // 3
    print("  Mode:  ", mode(data));   // 2
    print("  StdDev:", stddev(data)); // High due to outlier
    
    // Random Sampling
    Vec<str> population = {"A", "B", "C", "D", "E"};
    print("Sample (3 unique):", sample(population, 3));
    print("Choices (5 w/repl):", choices(population, 5));

    section("6. FILE I/O");
    if (!fileexists("tour.txt")) {
        writefile("tour.txt", "This is a test file.\nAppended line.");
        print("Written 'tour.txt'");
    }
    var lines = readlines("tour.txt");
    print("Read", lines.size(), "lines from file.");
    removefile("tour.txt"); // Cleanup

    section("7. PROFILING (RAII)");
    {
        Timer t("Heavy Computation");
        sleep(50); // Simulate work
        vi64 big;
        rep(i, 1000) big.pb(i);
        shuffle(big);
        sortasc(big);
    } // Timer prints automatically here

    section("DONE");
    return 0;
}
