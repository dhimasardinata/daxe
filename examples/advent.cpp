/*
 * DAXE Example: Advent of Code Style
 * Problem: Find two entries in 'input.txt' that sum to 2020 and multiply them.
 * 
 * Demonstrates:
 * - File IO (readfile)
 * - String parsing (split, parse)
 * - O(N) lookup (Set)
 * - Result handling
 */

#include <daxe.h>
#include <daxe/compat.h>

using namespace dax;

// Mock input generator
void generate_input() {
    print("Generating input.txt...");
    vi64 data = {1721, 979, 366, 299, 675, 1456};
    // Add some noise
    loop(i, 50) data.pb(rand(1, 2000));
    
    // Ensure solution exists: 1721 + 299 = 2020
    if (!has(data, 1721LL)) data.pb(1721);
    if (!has(data, 299LL)) data.pb(299);
    
    // Write to file
    std::ofstream f("input.txt");
    for (i64 x : data) f << x << '\n';
}

int main() {
    // Setup
    generate_input();
    
    // 1. Read File
    print("Reading input.txt...");
    str content = readfile("input.txt");
    if (content.empty()) panic("Failed to read input!");
    
    // 2. Parse Lines
    Set<> seen; // Default is Set<i64>
    vi64 nums;
    
    for (str line : split(content, '\n')) {
        str s = strip(line);
        if (s.empty()) continue;
        
        // Parse and process
        parse<i64>(s).then([&](i64 n) {
            nums.pb(n);
            
            // 3. Solve (2SUM)
            i64 target = 2020 - n;
            if (has(seen, target)) {
                print("\nFOUND SOLUTION!");
                print(n, "+", target, "= 2020");
                print("Result:", n * target);
            }
            
            seen.insert(n);
            return Ok(n);
        }).ignore();
    }
    
    print("\nProcessed", nums.size(), "numbers.");
    
    // Cleanup
    std::remove("input.txt");
    return 0;
}
