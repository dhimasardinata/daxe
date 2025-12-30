# DAXE Naming Conventions

This document explains the naming patterns used in daxe.

## Core Principles

1. **Full readable words** - No cryptic abbreviations in the clean API
2. **Flat compound words** - No underscores (e.g., `getat` not `get_at`)
3. **Consistent patterns** - Same naming style across all modules

## Naming Patterns

### Getters (Nouns)
Functions that return a value without modification:
```cpp
max(v)      // Maximum element
min(v)      // Minimum element  
sum(v)      // Sum of elements
first(v)    // First element (Option)
last(v)     // Last element (Option)
len(v)      // Size as i64
```

### Mutators (Verbs)
Functions/macros that modify in-place:
```cpp
sortasc(v)   // Sort ascending
sortdesc(v)  // Sort descending
flip(v)      // Reverse
uniquify(v)  // Sort + deduplicate
```

### Creators (Past Participles)
Functions that return a new copy:
```cpp
sorted(v)    // Returns sorted copy
reversed(v)  // Returns reversed copy
filtered(v)  // Returns filtered copy
sliced(v)    // Returns sliced copy
```

### Predicates (is/has)
Functions returning bool:
```cpp
isprime(n)   // Is n prime?
has(v, x)    // Does v contain x?
issome(opt)  // Is option Some?
isnone(opt)  // Is option None?
isok(res)    // Is result Ok?
iserr(res)   // Is result Err?
```

### Safe Accessors (get + modifier)
Functions with fallback behavior:
```cpp
getat(v, i)      // Get at index → Option
getor(v, i, d)   // Get or default → T
charat(s, i)     // Char at index → Option
valueor(o, d)    // Value or default (was unwrapor)
```

### Update Functions (update + direction)
Functions that conditionally update:
```cpp
updatemin(a, b)  // a = min(a, b), returns true if updated
updatemax(a, b)  // a = max(a, b), returns true if updated
```

## Loop Macros

| Macro | Description |
|-------|-------------|
| `loop(i, n)` | 0 to n-1 |
| `loop1(i, n)` | 1 to n |
| `loopdown(i, n)` | n-1 down to 0 |
| `loopfrom(i, a, b)` | a to b-1 |
| `foreach(x, c)` | Range-based for |
| `foreachconst(x, c)` | Const range-based for |

## Type Aliases

| Pattern | Examples |
|---------|----------|
| `[iu][8/16/32/64/128]` | `i64`, `u32` |
| `f[32/64/80]` | `f64`, `f32` |
| `v[type]` | `vi64`, `vstr` |
| `p[type]` | `pi64`, `pstr` |
| `s[type]` | `si64` (set) |
| `m[K][V]` | `mi64str` (map) |

## Compat.h Shortcuts

For competitive programming, short aliases are in `compat.h`:

| Clean API | CP Shortcut |
|-----------|-------------|
| `loop` | `rep` |
| `loopdown` | `rrep` |
| `loopfrom` | `FOR` |
| `sortasc` | `sor` |
| `flip` | `rev` |
| `reverseall` | `rall` |
| `countbits` | `popcount` |
| `leadingzeros` | `clz` |
| `trailingzeros` | `ctz` |
| `updatemin` | `ckmin` |
| `updatemax` | `ckmax` |
