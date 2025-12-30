import daxe.compat;
import daxe; // For print

using namespace dax; // Enable shorthand access

int main() {
    println("Testing daxe.compat module...");
    
    ll big_num = 1000000000000LL; println("Big:", big_num);
    vll nums = {3, 1, 4, 1, 5};
    
    // Test sor()
    sor(nums);
    println("Sorted:", nums.size()); // size check
    
    // Test sz()
    println("Size:", sz(nums));
    
    // Test ckmin
    ll x = 100;
    ckmin(x, 50LL);
    println("ckmin(100, 50) ->", x);

    return 0;
}
