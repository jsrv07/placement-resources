#include <bits/stdc++.h> // Includes all standard libraries
using namespace std;     // Using the standard namespace for brevity

// Use long long for differences to avoid overflow from large 'c' values
using ll = long long;

/**
 * @brief Disjoint Set Union (DSU) structure augmented to handle potential differences.
 *
 * This data structure is ideal for problems involving difference constraints
 * of the form `xi - xj = c`. It can efficiently unite sets of variables
 * and simultaneously check for contradictions.
 */
struct DSU {
    // parent[i] stores the parent of node i in the DSU tree.
    vector<int> parent;
    // diff[i] stores the potential difference: x_i - x_{parent[i]}.
    vector<ll> diff;

    // Constructor to initialize N nodes, 1-indexed.
    DSU(int n) {
        parent.resize(n + 1);
        // Initially, each node is its own parent.
        iota(parent.begin(), parent.end(), 0);
        // Initially, the difference with the parent (itself) is 0.
        diff.assign(n + 1, 0);
    }

    /**
     * @brief Finds the root of the set containing node 'i' and performs path compression.
     * @return The root of the set.
     */
    int find(int i) {
        // Base case: If 'i' is its own parent, it's the root.
        if (parent[i] == i) {
            return i;
        }

        // Recursively find the ultimate root of the set.
        int root = find(parent[i]);

        // Path Compression:
        // Update our diff to be relative to the final root.
        // The original diff[i] was `xi - x_{parent[i]}`.
        // The `find` call updated `diff[parent[i]]` to be `x_{parent[i]} - x_{root}`.
        // So, the new diff[i] = (xi - x_{parent[i]}) + (x_{parent[i]} - x_{root}) = xi - x_{root}.
        diff[i] += diff[parent[i]];
        
        // Point 'i' directly to the root for future efficiency.
        parent[i] = root;
        
        return root;
    }

    /**
     * @brief Unites the sets containing 'i' and 'j' based on the equation xi - xj = c.
     * @return true if the union is consistent with existing equations, false otherwise.
     */
    bool unite(int i, int j, int c) {
        int root_i = find(i);
        int root_j = find(j);
        
        // If 'i' and 'j' are not already in the same set, we merge them.
        if (root_i != root_j) {
            // We'll merge root_i's set into root_j's set by making root_j the parent.
            parent[root_i] = root_j;
            
            // We must now calculate the new diff[root_i], which must be `x_{root_i} - x_{root_j}`.
            // We know:
            // 1. `xi - xj = c` (from input)
            // 2. `xi - x_{root_i} = diff[i]` (after `find(i)`) -> `xi = x_{root_i} + diff[i]`
            // 3. `xj - x_{root_j} = diff[j]` (after `find(j)`) -> `xj = x_{root_j} + diff[j]`
            //
            // Substitute (2) and (3) into (1):
            // `(x_{root_i} + diff[i]) - (x_{root_j} + diff[j]) = c`
            //
            // Rearrange to solve for `x_{root_i} - x_{root_j}`:
            // `x_{root_i} - x_{root_j} = c + diff[j] - diff[i]`
            diff[root_i] = diff[j] - diff[i] + c;

        } else {
            // If they are already in the same set, we have an existing relationship.
            // Check if the new equation `xi - xj = c` contradicts it.
            // The established difference is `(xi - x_{root}) - (xj - x_{root}) = diff[i] - diff[j]`.
            if (diff[i] - diff[j] != c) {
                return false; // Contradiction found!
            }
        }
        return true; // This operation was consistent.
    }
};

// Main logic for a single test case
void solve() {
    int n, m;
    cin >> n >> m;

    DSU dsu(n);
    bool consistent = true;

    for (int k = 0; k < m; ++k) {
        int i, j, c;
        cin >> i >> j >> c;
        
        // If we've already found an inconsistency, no need to process further.
        // Just read the remaining input for this test case.
        if (consistent) {
            // The equation is `xi = xj + c`, which is `xi - xj = c`.
            if (!dsu.unite(i, j, c)) {
                consistent = false;
            }
        }
    }

    if (consistent) {
        cout << "YES\n";
    } else {
        cout << "NO\n";
    }
}

int main() {
    // Fast I/O for competitive programming
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t;
    cin >> t;
    while (t--) {
        solve();
    }

    return 0;
}