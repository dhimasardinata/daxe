# daxe 🪓

[![CI](https://github.com/dhimasardinata/daxe/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/dhimasardinata/daxe/actions/workflows/ci.yml)
[![GitHub release (latest by date)](https://img.shields.io/github/v/release/dhimasardinata/daxe?label=latest%20version&color=green)](https://github.com/dhimasardinata/daxe/releases)
[![License](https://img.shields.io/github/license/dhimasardinata/daxe)](https://github.com/dhimasardinata/daxe#license)
[![GitHub Sponsors](https://img.shields.io/badge/Sponsor-❤-pink?logo=github)](https://github.com/sponsors/dhimasardinata)

> **D.A's Axe** - Cut through C++ verbosity.

**daxe** is a zero-overhead, single-header C++ library designed for competitive programming and rapid prototyping. It provides Python-like ease of use with the raw performance of modern C++.

## ✨ Features

- **⚡ Zero Overhead**: Heavy use of `constexpr`, `inline`, and `noexcept`. Compiles to identical assembly as raw C++.
- **🐍 Pythonic Syntax**: `print()`, `range()`, `enumerate()`, `zip()`, `map()`, `filter()`.
- **🛡️ Safe by Default**:
    - `i128` support with platform detection.
    - `checked_cast` to prevent overflows.
    - `readfile` with strict error handling.
- **🔢 Rich Math**: `gcd`, `lcm`, `modinv` (Extended Euclidean), `isprime`, `combinations`.
- **🎲 Random**: Robust random number generation (`rand`, `choice`, `sample`, `shuffle`).
- **🔧 Containers**: `Vec`, `Set`, `Dict`, `List` aliases for standard containers.

## 📦 Installation

### Recommended: via [caxe](https://github.com/dhimasardinata/caxe)

The easiest way to use daxe is with the **caxe** project manager:

```bash
cx add daxe
```

### Manual Integration

Since daxe is a header-only library, you can simply clone it and add `include/` to your compiler's include path.

```bash
git clone https://github.com/dhimasardinata/daxe.git
g++ -std=c++20 -I./daxe/include main.cpp -o main
```

## 🚀 Quick Start

```cpp
#include <daxe.h>

int main() {
    // Pythonic printing
    print("Hello, Daxe!");
    
    // Easy I/O
    vi64 v = readvector<i64>(5);
    
    // Modern Loops
    for (auto [i, val] : enumerate(v)) {
        if (isprime(val)) {
            print("Prime at index", i, ":", val);
        }
    }
    
    // Math Utilities
    print("GCD(12, 18) =", gcd(12, 18));
    print("ModInv(3, 1000000007) =", modinv(3));
}
```

## 📖 API Overview

### Core Types
```cpp
using i64 = int64_t;
using u64 = uint64_t;
using f64 = double;
using str = std::string;
```

### Flow Control
```cpp
loop(i, n)             // for (i64 i = 0; i < n; ++i)
loop(i, a, b)          // for (i64 i = a; i < b; ++i)
foreach(x, container)  // for (auto& x : container)
```

### Algorithms
```cpp
sortasc(v);     // std::sort(v.begin(), v.end())
sortdesc(v);    // std::sort(v.rbegin(), v.rend())
uniquify(v);    // sort + unique + erase
reverse(v);     // std::reverse
```

### I/O
```cpp
print(a, b, c);        // std::cout << a << ' ' << b ... << '\n'
input(a, b);           // std::cin >> a >> b
readfile("data.txt");  // Returns file content as string
```

## 🤝 Contributing

Contributions are welcome!
1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Testing
This project uses **caxe** for testing.
```bash
cx test
```

## 💖 Sponsors

If you find daxe useful, consider supporting its development:

[![GitHub Sponsors](https://img.shields.io/badge/Sponsor-%E2%9D%A4-pink?logo=github)](https://github.com/sponsors/dhimasardinata)
[![Ko-fi](https://img.shields.io/badge/Ko--fi-Support-FF5E5B?logo=ko-fi)](https://ko-fi.com/dhimasardinata)
[![Open Collective](https://img.shields.io/badge/Open%20Collective-Donate-7FADF2?logo=opencollective)](https://opencollective.com/caxe)

## 📝 License

Licensed under the MIT License. See [LICENSE](LICENSE) for details.

---

**Made with ❤️ for the C/C++ community**
