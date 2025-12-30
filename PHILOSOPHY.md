# DAXE Design Philosophy

> **"Cut through C++ verbosity"**

---

## Core Principles

### 1. Safe by Default
Functions return `Option<T>` or use bounds checking when failure is possible.  
No silent crashes, no undefined behavior.

```cpp
// Safe: returns None if out of bounds
list.getat(999);          // → Option<T>
list.getor(999, default); // → T with fallback

// Unsafe explicit opt-in if needed
list[999];                // Raw access (fast, unchecked)
```

### 2. Zero Abstraction
Wrappers compile to the same code as raw std::.  
No virtual functions. No heap allocations in wrappers.

```cpp
// List<T> IS std::vector<T>
// max(v) compiles to std::max_element(...)
```

### 3. Explicit Over Implicit
Clear naming. No magic. If it allocates, it says so.  
If it mutates, it says so.

```cpp
list.sort();       // Mutates in-place
sorted(list);      // Returns new copy

list.unique();     // Mutates
deduplicated(v);   // Returns new copy
```

### 4. General by Default
Functions accept the broadest useful type.  
Smart detection via concepts/SFINAE when beneficial.

```cpp
// Works with any container
sum(vector);
sum(list);
sum(set);

// Type is inferred
Dict d = {{"a", 1}};  // Dict<str, i64>
```

### 5. Composable
Functions return values suitable for chaining.  
Small, focused utilities that combine well.

```cpp
auto result = sorted(filtered(v, ispositive));
auto total = sum(prefixsum(data));
```

### 6. Semantic Naming
Names describe behavior, not implementation.  
No cryptic abbreviations.

| ❌ Avoid | ✅ Use |
|----------|--------|
| `ctz` | `trailingzeros` |
| `clz` | `leadingzeros` |
| `ncr` | `combinations` |
| `binpow` | `binarypower` |
| `getat` | `getat` (already clear) |

---

## Naming Convention

### Functions: `flatcase`
No underscores, no camelCase. Clean, readable.

```cpp
prefixsum()
rangequery()
binarysearch()
tostring()
```

### Types: `PascalCase`
```cpp
List<T>
Dict<K, V>
Option<T>
FenwickTree
```

### Constants: `SCREAMING_CASE`
```cpp
MOD
INF
MAXN
```

### Template Parameters: Single Uppercase
```cpp
template <typename T>
template <typename K, typename V>
```

---

## Safety Spectrum

| Level | Pattern | Use Case |
|-------|---------|----------|
| **Safe** | `getat() → Option<T>` | Default |
| **Fallback** | `getor(default)` | When you have a default |
| **Unsafe** | `operator[]` | Hot loops, known bounds |

---

## Performance Guarantees

- All type aliases are zero-cost (`i64` = `long long`)
- All container wrappers are zero-cost
- Inline + constexpr where possible
- noexcept on pure functions
