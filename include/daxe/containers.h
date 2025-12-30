/*
 * DAXE - CONTAINERS
 * D.A's Axe - Cut through C++ verbosity
 */

#ifndef DAXE_CONTAINERS_H
#define DAXE_CONTAINERS_H

#include "vectors.h"
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <stack>

DAXE_NAMESPACE_BEGIN

// Sets
using si32 = std::set<i32>;
using si64 = std::set<i64>;
using su64 = std::set<u64>;
using sf64 = std::set<f64>;
using sstr = std::set<str>;
using schar = std::set<char>;
using spi32 = std::set<pi32>;
using spi64 = std::set<pi64>;

// Multisets
using msi32 = std::multiset<i32>;
using msi64 = std::multiset<i64>;
using msstr = std::multiset<str>;

// Unordered sets
using usi32 = std::unordered_set<i32>;
using usi64 = std::unordered_set<i64>;
using usstr = std::unordered_set<str>;

// Maps
using mi32i32 = std::map<i32, i32>;
using mi32i64 = std::map<i32, i64>;
using mi64i32 = std::map<i64, i32>;
using mi64i64 = std::map<i64, i64>;
using mi32str = std::map<i32, str>;
using mi64str = std::map<i64, str>;
using mstri32 = std::map<str, i32>;
using mstri64 = std::map<str, i64>;
using mstrstr = std::map<str, str>;
using mchari32 = std::map<char, i32>;
using mpi32i32 = std::map<pi32, i32>;
using mpi64i64 = std::map<pi64, i64>;

// Unordered maps
using umi32i32 = std::unordered_map<i32, i32>;
using umi64i64 = std::unordered_map<i64, i64>;
using umstri32 = std::unordered_map<str, i32>;
using umstri64 = std::unordered_map<str, i64>;
using umstrstr = std::unordered_map<str, str>;

// Stacks
using sti32 = std::stack<i32>;
using sti64 = std::stack<i64>;
#if DAXE_HAS_INT128
using sti128 = std::stack<i128>;
#endif
using ststr = std::stack<str>;
using stpi32 = std::stack<pi32>;
using stpi64 = std::stack<pi64>;

// Queues
using qi32 = std::queue<i32>;
using qi64 = std::queue<i64>;
#if DAXE_HAS_INT128
using qi128 = std::queue<i128>;
#endif
using qstr = std::queue<str>;
using qpi32 = std::queue<pi32>;
using qpi64 = std::queue<pi64>;

// Deques
using di32 = std::deque<i32>;
using di64 = std::deque<i64>;
#if DAXE_HAS_INT128
using di128 = std::deque<i128>;
#endif
using dstr = std::deque<str>;
using dpi32 = std::deque<pi32>;
using dpi64 = std::deque<pi64>;

// Priority Queues - Max Heaps
using pqi32 = std::priority_queue<i32>;
using pqi64 = std::priority_queue<i64>;
#if DAXE_HAS_INT128
using pqi128 = std::priority_queue<i128>;
#endif
using pqf64 = std::priority_queue<f64>;
using pqpi32 = std::priority_queue<pi32>;
using pqpi64 = std::priority_queue<pi64>;

// Priority Queues - Min Heaps
using pqmini32 = std::priority_queue<i32, vi32, std::greater<i32>>;
using pqmini64 = std::priority_queue<i64, vi64, std::greater<i64>>;
using pqminf64 = std::priority_queue<f64, vf64, std::greater<f64>>;
using pqminpi32 = std::priority_queue<pi32, vpi32, std::greater<pi32>>;
using pqminpi64 = std::priority_queue<pi64, vpi64, std::greater<pi64>>;

DAXE_NAMESPACE_END

#endif // DAXE_CONTAINERS_H
