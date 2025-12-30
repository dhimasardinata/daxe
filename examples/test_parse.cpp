#include <daxe/convert.h>
#include <iostream>
using namespace dax; int main() { auto r = parse<i64>("  123 "); if (r.isok() && r.unwrap() == 123) std::cout << "PASS" << std::endl; else std::cout << "FAIL" << std::endl; return 0; }
