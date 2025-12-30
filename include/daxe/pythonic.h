/*
 * DAXE - PYTHONIC CONTAINERS
 * D.A's Axe - Cut through C++ verbosity
 * 
 * Naming: Flat style (no snake_case)
 */

#ifndef DAXE_PYTHONIC_H
#define DAXE_PYTHONIC_H

#include "vectors.h"
#include "containers.h"
#include "safe.h"
#include "english.h"
#include <stack>
#include <queue>
#include <algorithm>
#include <numeric>

DAXE_NAMESPACE_BEGIN

// ==========================================
// LIST CLASS
// ==========================================
template <typename T = i64>
class List : private std::vector<T> {
public:
    // Expose Vector Interface
    using std::vector<T>::vector;
    using std::vector<T>::begin;
    using std::vector<T>::end;
    using std::vector<T>::rbegin;
    using std::vector<T>::rend;
    using std::vector<T>::size;
    using std::vector<T>::empty;
    using std::vector<T>::resize;
    using std::vector<T>::reserve;
    using std::vector<T>::capacity;
    using std::vector<T>::shrink_to_fit;
    using std::vector<T>::push_back;
    using std::vector<T>::emplace_back;
    using std::vector<T>::pop_back;
    using std::vector<T>::operator[];    
    using std::vector<T>::at;
    using std::vector<T>::front;
    using std::vector<T>::back;
    using std::vector<T>::clear;
    using std::vector<T>::insert;
    using std::vector<T>::erase;
    using std::vector<T>::assign;
    using std::vector<T>::swap;

    // Type aliases need re-exposing
    using typename std::vector<T>::value_type;
    using typename std::vector<T>::size_type;
    using typename std::vector<T>::iterator;
    using typename std::vector<T>::const_iterator;

    // Explicit access to underlying vector (since inheritance is private)
    // Usages requiring std::vector& must now call .vec()
    DAXE_NODISCARD const std::vector<T>& vec() const noexcept { return *this; }
    DAXE_NODISCARD std::vector<T>& vec() noexcept { return *this; }

    // Allow explicit casting
    explicit operator const std::vector<T>&() const noexcept { return *this; }
    explicit operator std::vector<T>&() noexcept { return *this; }

    template <typename U> void append(U&& x) { this->push_back(std::forward<U>(x)); }
    
    // Safe by default: returns Option<T>
    DAXE_NODISCARD Option<T> pop() {
        if (this->empty()) DAXE_UNLIKELY return None;
        T val = std::move(this->back()); this->pop_back();
        return Some(std::move(val));
    }
    
    // Convenient safe access with fallback
    DAXE_NODISCARD T pop(const T& fallback) {
        if (this->empty()) DAXE_UNLIKELY return fallback;
        T val = std::move(this->back()); this->pop_back();
        return val;
    }
    
    void extend(const List<T>& other) {
        this->reserve(this->size() + other.size());
        for (const auto& x : other) this->push_back(x);
    }
    
    void insertat(i64 idx, const T& x) {
        i64 n = static_cast<i64>(this->size());
        if (idx < 0) idx += n;
        if (idx < 0) idx = 0;
        if (idx > n) idx = n;
        this->insert(this->begin() + idx, x);
    }
    
    bool remove(const T& x) noexcept {
        auto it = std::find(this->begin(), this->end(), x);
        if (it != this->end()) DAXE_LIKELY { this->erase(it); return true; }
        return false;
    }
    
    void removeat(i64 idx) noexcept {
        if (idx < 0) idx += static_cast<i64>(this->size());
        if (idx >= 0 && idx < static_cast<i64>(this->size())) this->erase(this->begin() + idx);
    }
    
    DAXE_NODISCARD i64 indexof(const T& x) const noexcept {
        auto it = std::find(this->begin(), this->end(), x);
        return it != this->end() ? it - this->begin() : -1;
    }
    
    DAXE_NODISCARD i64 count(const T& x) const noexcept { return std::count(this->begin(), this->end(), x); }
    DAXE_NODISCARD bool has(const T& x) const noexcept { return std::find(this->begin(), this->end(), x) != this->end(); }
    
    void sort() noexcept { std::sort(this->begin(), this->end()); }
    void rsort() noexcept { std::sort(this->rbegin(), this->rend()); }
    void reverse() noexcept { std::reverse(this->begin(), this->end()); }
    
    DAXE_NODISCARD List<T> sorted() const { List<T> copy = *this; copy.sort(); return copy; }
    DAXE_NODISCARD List<T> reversed() const { List<T> copy = *this; copy.reverse(); return copy; }
    
    DAXE_NODISCARD List<T> slice(i64 start, i64 end) const {
        const i64 n = static_cast<i64>(this->size());
        if (start < 0) start += n; 
        if (end < 0) end += n;
        start = std::max(0LL, std::min(start, n));
        end = std::max(0LL, std::min(end, n));
        if (start >= end) DAXE_UNLIKELY return {};
        return List<T>(this->begin() + start, this->begin() + end);
    }
    
    DAXE_NODISCARD Option<T> getat(i64 idx) const noexcept {
        if (idx < 0) idx += static_cast<i64>(this->size());
        if (idx < 0 || idx >= static_cast<i64>(this->size())) DAXE_UNLIKELY return None;
        return Some((*this)[static_cast<size_t>(idx)]);
    }
    
    DAXE_NODISCARD T getor(i64 idx, const T& def) const noexcept {
        if (idx < 0) idx += static_cast<i64>(this->size());
        if (idx < 0 || idx >= static_cast<i64>(this->size())) DAXE_UNLIKELY return def;
        return (*this)[static_cast<size_t>(idx)];
    }
    
    DAXE_NODISCARD T sum() const noexcept { return std::accumulate(this->begin(), this->end(), T{}); }
    DAXE_NODISCARD Option<T> max() const { if (this->empty()) return None; return Some(*std::max_element(this->begin(), this->end())); }
    DAXE_NODISCARD Option<T> min() const { if (this->empty()) return None; return Some(*std::min_element(this->begin(), this->end())); }
    DAXE_NODISCARD T max(const T& fallback) const { if (this->empty()) return fallback; return *std::max_element(this->begin(), this->end()); }
    DAXE_NODISCARD T min(const T& fallback) const { if (this->empty()) return fallback; return *std::min_element(this->begin(), this->end()); }
    
    template <typename Func>
    DAXE_NODISCARD List<T> filter(Func&& f) const {
        List<T> result; result.reserve(this->size());
        for (const auto& x : *this) if (f(x)) result.push_back(x);
        result.shrink_to_fit();
        return result;
    }
    
    template <typename Func>
    DAXE_NODISCARD auto transform(Func&& f) const {
        using U = decltype(f(std::declval<T>()));
        List<U> result; result.reserve(this->size());
        for (const auto& x : *this) result.push_back(f(x));
        return result;
    }
    
    template <typename Func> DAXE_NODISCARD bool any(Func&& f) const noexcept { for (const auto& x : *this) if (f(x)) DAXE_LIKELY return true; return false; }
    template <typename Func> DAXE_NODISCARD bool every(Func&& f) const noexcept { for (const auto& x : *this) if (!f(x)) DAXE_UNLIKELY return false; return true; }
    template <typename Func> DAXE_NODISCARD bool none(Func&& f) const noexcept { return !any(std::forward<Func>(f)); }
    
    void unique() { this->erase(std::unique(this->begin(), this->end()), this->end()); }
};

// Deduction guides
List() -> List<i64>;
template <typename T> List(std::initializer_list<T>) -> List<T>;

// ==========================================
// DICT CLASS
// ==========================================
template <typename K = str, typename V = i64>
class Dict : public std::map<K, V> {
public:
    using std::map<K, V>::map;
    DAXE_NODISCARD bool has(const K& key) const noexcept { return this->count(key) > 0; }
    DAXE_NODISCARD Option<V> getat(const K& key) const noexcept { auto it = this->find(key); return it != this->end() ? Some(it->second) : None; }
    DAXE_NODISCARD V getor(const K& key, const V& def) const noexcept { auto it = this->find(key); return it != this->end() ? it->second : def; }
    template <typename VV> void set(const K& key, VV&& value) { (*this)[key] = std::forward<VV>(value); }
    bool remove(const K& key) noexcept { return this->erase(key) > 0; }
    DAXE_NODISCARD List<K> keys() const { List<K> r; r.reserve(this->size()); for (const auto& [k, v] : *this) r.push_back(k); return r; }
    DAXE_NODISCARD List<V> values() const { List<V> r; r.reserve(this->size()); for (const auto& [k, v] : *this) r.push_back(v); return r; }
    DAXE_NODISCARD List<std::pair<K, V>> items() const { List<std::pair<K, V>> r; r.reserve(this->size()); for (const auto& [k, v] : *this) r.push_back({k, v}); return r; }
    void update(const Dict<K, V>& other) { for (const auto& [k, v] : other) (*this)[k] = v; }
    // Safe by default: returns Option<V>
    DAXE_NODISCARD Option<V> pop(const K& key) { 
        auto it = this->find(key); 
        if (it == this->end()) DAXE_UNLIKELY return None; 
        V val = std::move(it->second); 
        this->erase(it); 
        return Some(std::move(val)); 
    }
    
    // Core Polish
    DAXE_NODISCARD V get(const K& k, const V& def) const { return this->getor(k, def); }
    V& setdefault(const K& k, const V& def) {
        auto it = this->find(k);
        if (it != this->end()) return it->second;
        return (*this)[k] = def;
    }
    
    void merge(const Dict<K, V>& other) { for (const auto& [k, v] : other) (*this)[k] = v; }
};

// Deduction guides
Dict() -> Dict<str, i64>;

// ==========================================
// SET CLASS
// ==========================================
template <typename T = i64>
class Set : public std::set<T> {
public:
    using std::set<T>::set;
    template <typename U> void add(U&& x) { this->insert(std::forward<U>(x)); }
    DAXE_NODISCARD bool has(const T& x) const noexcept { return this->count(x) > 0; }
    bool remove(const T& x) noexcept { return this->erase(x) > 0; }
    DAXE_NODISCARD Option<T> pop() noexcept { if (this->empty()) DAXE_UNLIKELY return None; T val = std::move(*this->begin()); this->erase(this->begin()); return Some(std::move(val)); }
    DAXE_NODISCARD Set<T> unite(const Set<T>& other) const { Set<T> r = *this; for (const auto& x : other) r.insert(x); return r; }
    DAXE_NODISCARD Set<T> intersect(const Set<T>& other) const { Set<T> r; for (const auto& x : *this) if (other.has(x)) r.insert(x); return r; }
    DAXE_NODISCARD Set<T> difference(const Set<T>& other) const { Set<T> r; for (const auto& x : *this) if (!other.has(x)) r.insert(x); return r; }
    DAXE_NODISCARD bool issubset(const Set<T>& other) const noexcept { for (const auto& x : *this) if (!other.has(x)) DAXE_UNLIKELY return false; return true; }
    DAXE_NODISCARD bool issuperset(const Set<T>& other) const noexcept { return other.issubset(*this); }
    DAXE_NODISCARD List<T> tolist() const { return List<T>(this->begin(), this->end()); }
    DAXE_NODISCARD Set<T> symmetricdiff(const Set<T>& other) const {
        Set<T> r;
        for (const auto& x : *this) if (!other.has(x)) r.insert(x);
        for (const auto& x : other) if (!this->has(x)) r.insert(x);
        return r;
    }
};

// Deduction guides
Set() -> Set<i64>;
template <typename T> Set(std::initializer_list<T>) -> Set<T>;

// ==========================================
// DEQUE CLASS
// ==========================================
template <typename T = i64>
class Deque : public std::deque<T> {
public:
    using std::deque<T>::deque;
    template <typename U> void append(U&& x) { this->push_back(std::forward<U>(x)); }
    template <typename U> void appendleft(U&& x) { this->push_front(std::forward<U>(x)); }
    DAXE_NODISCARD Option<T> pop() { if (this->empty()) DAXE_UNLIKELY return None; T val = std::move(this->back()); this->pop_back(); return Some(std::move(val)); }
    DAXE_NODISCARD Option<T> popleft() { if (this->empty()) DAXE_UNLIKELY return None; T val = std::move(this->front()); this->pop_front(); return Some(std::move(val)); }
    DAXE_NODISCARD T pop(const T& fallback) { if (this->empty()) return fallback; T val = std::move(this->back()); this->pop_back(); return val; }
    DAXE_NODISCARD T popleft(const T& fallback) { if (this->empty()) return fallback; T val = std::move(this->front()); this->pop_front(); return val; }
    
    // Core Polish
    void rotate(i64 n) {
        if (this->empty()) return;
        i64 len = static_cast<i64>(this->size());
        n %= len;
        if (n < 0) n += len;
        if (n == 0) return;
        // std::rotate rotates left (beg, mid, end). To rotate right by n, we rotate left by len-n.
        // Wait, standard python rotate(1) moves right: [1,2] -> [2,1].
        // std::rotate(begin, begin + (len - n), end) performs right rotation by n.
        std::rotate(this->begin(), this->begin() + (len - n), this->end());
    }
};
Deque() -> Deque<i64>;
template <typename T> Deque(std::initializer_list<T>) -> Deque<T>;

// ==========================================
// STACK CLASS (Wrapper)
// ==========================================
template <typename T = i64>
class Stack : public std::stack<T> {
public:
    using std::stack<T>::stack;
    template <typename U> void push(U&& x) { std::stack<T>::push(std::forward<U>(x)); }
    DAXE_NODISCARD Option<T> pop() { if (this->empty()) DAXE_UNLIKELY return None; T val = std::move(this->top()); std::stack<T>::pop(); return Some(std::move(val)); }
    DAXE_NODISCARD Option<T> peek() const { if (this->empty()) DAXE_UNLIKELY return None; return Some(this->top()); }
    DAXE_NODISCARD T pop(const T& fallback) { if (this->empty()) return fallback; T val = std::move(this->top()); std::stack<T>::pop(); return val; }
    DAXE_NODISCARD T peek(const T& fallback) const { if (this->empty()) return fallback; return this->top(); }
    
    // Core Polish
    bool isempty() const { return this->empty(); }
    void clear() { while (!this->empty()) { (void)this->pop(); } }
};
Stack() -> Stack<i64>;

// ==========================================
// QUEUE CLASS (Wrapper)
// ==========================================
template <typename T = i64>
class Queue : public std::queue<T> {
public:
    using std::queue<T>::queue;
    template <typename U> void push(U&& x) { std::queue<T>::push(std::forward<U>(x)); }
    DAXE_NODISCARD Option<T> pop() { if (this->empty()) DAXE_UNLIKELY return None; T val = std::move(this->front()); std::queue<T>::pop(); return Some(std::move(val)); }
    DAXE_NODISCARD Option<T> peek() const { if (this->empty()) DAXE_UNLIKELY return None; return Some(this->front()); }
    DAXE_NODISCARD T pop(const T& fallback) { if (this->empty()) return fallback; T val = std::move(this->front()); std::queue<T>::pop(); return val; }
    DAXE_NODISCARD T peek(const T& fallback) const { if (this->empty()) return fallback; return this->front(); }
    
    // Core Polish
    bool isempty() const { return this->empty(); }
    void clear() { while (!this->empty()) { (void)this->pop(); } }
};
Queue() -> Queue<i64>;

// ==========================================
// PRIORITY QUEUE CLASS (Wrapper around std::priority_queue)
// ==========================================
template <typename T = i64, typename Container = std::vector<T>, typename Compare = std::less<typename Container::value_type>>
class PriorityQueue : public std::priority_queue<T, Container, Compare> {
public:
    using std::priority_queue<T, Container, Compare>::priority_queue;
    template <typename U> void push(U&& x) { std::priority_queue<T, Container, Compare>::push(std::forward<U>(x)); }
    DAXE_NODISCARD Option<T> pop() { if (this->empty()) DAXE_UNLIKELY return None; T val = std::move(this->top()); std::priority_queue<T, Container, Compare>::pop(); return Some(std::move(val)); }
    DAXE_NODISCARD Option<T> peek() const { if (this->empty()) DAXE_UNLIKELY return None; return Some(this->top()); }
    DAXE_NODISCARD T pop(const T& fallback) { if (this->empty()) return fallback; T val = std::move(this->top()); std::priority_queue<T, Container, Compare>::pop(); return val; }
    DAXE_NODISCARD T peek(const T& fallback) const { if (this->empty()) return fallback; return this->top(); }
    
    // Core Polish
    bool isempty() const { return this->empty(); }
    void clear() { while (!this->empty()) { (void)this->pop(); } }
};
PriorityQueue() -> PriorityQueue<i64>;

// Type aliases
using Numbers = List<Int>;
using Floats = List<Float>;
using Strings = List<String>;
using Bools = List<Bool>;
using Chars = List<Char>;
using IntDict = Dict<String, Int>;
using StringDict = Dict<String, String>;
using IntSet = Set<Int>;
using StringSet = Set<String>;
using IntGrid = List<List<Int>>;
using CharGrid = List<List<Char>>;
using BoolGrid = List<List<Bool>>;

DAXE_NAMESPACE_END

#endif // DAXE_PYTHONIC_H
