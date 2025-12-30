/*
 * DAXE - GRAPH UTILITIES
 * D.A's Axe - Cut through C++ verbosity
 * 
 * Common graph data structures and helpers
 */

#ifndef DAXE_GRAPH_H
#define DAXE_GRAPH_H

#include "base.h"
#include "macros.h"
#include <vector>
#include <queue>
#include <numeric>

DAXE_NAMESPACE_BEGIN

// ==========================================
// ADJACENCY LIST TYPE
// ==========================================
using Graph = std::vector<std::vector<i64>>;
using WeightedGraph = std::vector<std::vector<std::pair<i64, i64>>>; // {neighbor, weight}

// ==========================================
// GRAPH CONSTRUCTION
// ==========================================

// Create empty graph with n nodes
DAXE_NODISCARD inline Graph makegraph(i64 n) {
    return Graph(static_cast<size_t>(n));
}

// Create empty weighted graph with n nodes
DAXE_NODISCARD inline WeightedGraph makeweightedgraph(i64 n) {
    return WeightedGraph(static_cast<size_t>(n));
}

// Add undirected edge (with bounds check)
inline void addedge(Graph& g, i64 u, i64 v) {
    i64 n = static_cast<i64>(g.size());
    if (u >= 0 && u < n && v >= 0 && v < n) {
        g[u].push_back(v);
        g[v].push_back(u);
    }
}

// Add directed edge (with bounds check)
inline void adddirected(Graph& g, i64 u, i64 v) {
    i64 n = static_cast<i64>(g.size());
    if (u >= 0 && u < n && v >= 0 && v < n) {
        g[u].push_back(v);
    }
}

// Add weighted undirected edge (with bounds check)
inline void addedge(WeightedGraph& g, i64 u, i64 v, i64 w) {
    i64 n = static_cast<i64>(g.size());
    if (u >= 0 && u < n && v >= 0 && v < n) {
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }
}

// Add weighted directed edge (with bounds check)
inline void adddirected(WeightedGraph& g, i64 u, i64 v, i64 w) {
    i64 n = static_cast<i64>(g.size());
    if (u >= 0 && u < n) {
        g[u].push_back({v, w});
    }
}

// ==========================================
// GRAPH QUERIES
// ==========================================

// Get node count
DAXE_NODISCARD inline i64 nodecount(const Graph& g) { return static_cast<i64>(g.size()); }
DAXE_NODISCARD inline i64 nodecount(const WeightedGraph& g) { return static_cast<i64>(g.size()); }

// Get edge count (for undirected, counts each edge twice)
DAXE_NODISCARD inline i64 edgecount(const Graph& g) {
    i64 count = 0;
    for (const auto& adj : g) count += static_cast<i64>(adj.size());
    return count;
}

// Get degree of node (with bounds check, returns 0 for invalid node)
DAXE_NODISCARD inline i64 degree(const Graph& g, i64 node) {
    if (node < 0 || node >= static_cast<i64>(g.size())) return 0;
    return static_cast<i64>(g[node].size());
}

// ==========================================
// FENWICK TREE (BINARY INDEXED TREE)
// ==========================================
class FenwickTree {
    std::vector<i64> tree;
    i64 n;
public:
    explicit FenwickTree(i64 size) : tree(static_cast<size_t>(size + 1), 0), n(size) {}
    
    void update(i64 i, i64 delta) {
        if (i < 0 || i >= n) return;  // Bounds check
        for (++i; i <= n; i += i & (-i)) tree[i] += delta;
    }
    
    DAXE_NODISCARD i64 query(i64 i) const {
        if (i < 0) return 0;  // Bounds check
        if (i >= n) i = n - 1;  // Clamp to valid range
        i64 sum = 0;
        for (++i; i > 0; i -= i & (-i)) sum += tree[i];
        return sum;
    }
    
    DAXE_NODISCARD i64 rangequery(i64 l, i64 r) const {
        if (l > r || r < 0 || l >= n) return 0;  // Bounds check
        return query(r) - (l > 0 ? query(l - 1) : 0);
    }
};

// ==========================================
// DISJOINT SET UNION (UNION-FIND)
// ==========================================
class DSU {
    std::vector<i64> parent, rank_;
public:
    explicit DSU(i64 n) : parent(static_cast<size_t>(n)), rank_(static_cast<size_t>(n), 0) {
        std::iota(parent.begin(), parent.end(), 0LL);
    }
    
    i64 find(i64 x) {
        if (x < 0 || x >= static_cast<i64>(parent.size())) return -1;  // Bounds check
        if (parent[x] != x) parent[x] = find(parent[x]); // Path compression
        return parent[x];
    }
    
    bool unite(i64 x, i64 y) {
        i64 px = find(x), py = find(y);
        if (px < 0 || py < 0 || px == py) return false;  // Invalid or already united
        // Union by rank
        if (rank_[px] < rank_[py]) std::swap(px, py);
        parent[py] = px;
        if (rank_[px] == rank_[py]) rank_[px]++;
        return true;
    }
    
    bool connected(i64 x, i64 y) {
        i64 px = find(x), py = find(y);
        return px >= 0 && py >= 0 && px == py;
    }
    
    i64 components() {
        i64 count = 0;
        for (size_t i = 0; i < parent.size(); ++i) if (find(static_cast<i64>(i)) == static_cast<i64>(i)) count++;
        return count;
    }
};

DAXE_NAMESPACE_END

#endif // DAXE_GRAPH_H
