#include <bits/stdc++.h>

using namespace std;
// Định nghĩa kiểu dữ liệu
using ll = long long;
using ld = long double;
using pii = pair<int, int>;    // pair<int, int>
using pll = pair<ll, ll>;    // pair<long long, long long>
using vi = vector<int>;        // vector<int>
using vll = vector<ll>;        // vector<long long>
using vpii = vector<pii>;      // vector<pair<int, int>>
using vpll = vector<pll>;      // vector<pair<long long, long long>>
using vvi = vector<vi>;        // vector<vector<int>>

// Macros
#define all(a) (a).begin(), (a).end()
#define rall(a) (a).rbegin(), (a).rend()
#define pb push_back
#define mp make_pair
#define sz(x) (int)(x).size()
#define ft first
#define sc second

// Hằng số
const int INF = 1e9 + 7;
const ll LINF = 1e18 + 7;
const int MOD = 1e9 + 7;
const int MAXN = 2e5 + 5; // Kích thước tối đa, thay đổi nếu cần

// ========================================
// BIẾN TOÀN CỤC CHO ĐỒ THỊ
// (Dùng chung cho nhiều thuật toán)
// ========================================

int n, m;                 // Số đỉnh, số cạnh
vvi adj;                  // Danh sách kề (đồ thị không trọng số)
vector<vpii> adj_weighted; // Danh sách kề (đồ thị có trọng số)
vector<bool> visited;     // Mảng đánh dấu đã thăm

// Hàm reset/khởi tạo
void init_graph(int num_nodes) {
    n = num_nodes;
    adj.assign(n + 1, vi());
    adj_weighted.assign(n + 1, vpii());
    visited.assign(n + 1, false);
}

// ========================================
// 1. DSU (DISJOINT SET UNION) / UNION-FIND
// ========================================
struct DSU {
    vi parent;
    vi sz; // Kích thước của mỗi tập
    DSU(int n) {
        parent.resize(n + 1);
        iota(all(parent), 0); // Gán parent[i] = i
        sz.assign(n + 1, 1);
    }

    // Tìm gốc của tập chứa u (có nén đường)
    int find(int u) {
        if (parent[u] == u)
            return u;
        return parent[u] = find(parent[u]);
    }

    // Hợp nhất hai tập chứa u và v (hợp nhất theo kích thước)
    bool unite(int u, int v) {
        int root_u = find(u);
        int root_v = find(v);
        if (root_u == root_v)
            return false; // Đã cùng một tập
        if (sz[root_u] < sz[root_v])
            swap(root_u, root_v);
        parent[root_v] = root_u;
        sz[root_u] += sz[root_v];
        return true;
    }
};

// ========================================
// 2. DFS (DEPTH FIRST SEARCH)
// ========================================
// Dùng biến toàn cục: adj, visited

void dfs(int u) {
    visited[u] = true;
    // cout << u << " "; // Xử lý đỉnh u
    for (int v : adj[u]) {
        if (!visited[v]) {
            dfs(v);
        }
    }
}

// ========================================
// 3. BFS (BREADTH FIRST SEARCH)
// ========================================
// Dùng biến toàn cục: adj
// Trả về vector khoảng cách từ start_node
vi bfs(int start_node) {
    vi dist(n + 1, -1); // Khởi tạo khoảng cách là -1 (chưa thăm)
    queue<int> q;

    q.push(start_node);
    dist[start_node] = 0;
    visited[start_node] = true; // Có thể dùng mảng visited toàn cục

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        // cout << u << " "; // Xử lý đỉnh u

        for (int v : adj[u]) {
            if (dist[v] == -1) { // !visited[v]
                dist[v] = dist[u] + 1;
                visited[v] = true;
                q.push(v);
            }
        }
    }
    return dist;
}

// ========================================
// 4. DIJKSTRA (ĐƯỜNG ĐI NGẮN NHẤT)
// ========================================
// Dùng adj_weighted
// Trả về vector khoảng cách từ start_node
vll dijkstra(int start_node) {
    vll dist(n + 1, LINF);
    dist[start_node] = 0;

    // {khoảng cách, đỉnh}
    priority_queue<pll, vpll, greater<pll>> pq;
    pq.push({0, start_node});

    while (!pq.empty()) {
        ll d = pq.top().ft;
        int u = pq.top().sc;
        pq.pop();

        // Tối ưu: nếu khoảng cách đã cũ thì bỏ qua
        if (d > dist[u])
            continue;

        for (auto& edge : adj_weighted[u]) {
            int v = edge.ft;
            int w = edge.sc;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

// ========================================
// 5. KRUSKAL (MINIMUM SPANNING TREE)
// ========================================

// Cần cấu trúc Edge
struct Edge {
    int u, v, w;
    // Sắp xếp theo trọng số tăng dần
    bool operator<(const Edge& other) const {
        return w < other.w;
    }
};

// Cần danh sách cạnh và DSU
ll kruskal(vector<Edge>& edge_list, DSU& dsu) {
    sort(all(edge_list));
    ll mst_cost = 0;
    int edges_count = 0;

    for (const auto& edge : edge_list) {
        if (dsu.unite(edge.u, edge.v)) {
            mst_cost += edge.w;
            edges_count++;
        }
    }

    // Kiểm tra xem có đủ n-1 cạnh không (đồ thị liên thông)
    if (edges_count != n - 1) {
        return -1; // Hoặc LINF, tùy vào bài toán
    }
    return mst_cost;
}

// ========================================
// 6. TOPOLOGICAL SORT (KAHN'S ALGORITHM)
// ========================================
// Dùng cho đồ thị có hướng (DAG)
// Dùng biến toàn cục: adj
vi kahn_topo_sort() {
    vi in_degree(n + 1, 0);
    for (int u = 1; u <= n; ++u) {
        for (int v : adj[u]) {
            in_degree[v]++;
        }
    }

    queue<int> q;
    for (int i = 1; i <= n; ++i) {
        if (in_degree[i] == 0) {
            q.push(i);
        }
    }

    vi topo_order;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        topo_order.pb(u);

        for (int v : adj[u]) {
            in_degree[v]--;
            if (in_degree[v] == 0) {
                q.push(v);
            }
        }
    }

    // Nếu sz(topo_order) == n thì đồ thị là DAG
    // Nếu sz(topo_order) < n thì đồ thị có chu trình
    return topo_order;
}

/**
 * ========================================
 * HÀM SOLVE()
 * Nơi đọc input và gọi các thuật toán
 * ========================================
 */
void solve() {
    // Đọc số đỉnh và số cạnh
    cin >> n >> m;

    // Khởi tạo các cấu trúc
    init_graph(n);

    // Dùng cho Kruskal
    vector<Edge> edge_list;
    // Dùng cho DSU
    // DSU dsu(n); 

    for (int i = 0; i < m; ++i) {
        int u, v;
        // cin >> u >> v; // Đồ thị không trọng số
        int w;
        cin >> u >> v >> w; // Đồ thị có trọng số

        // 1. Cho đồ thị không trọng số (DFS, BFS, TopoSort)
        // adj[u].pb(v);
        // adj[v].pb(u); // Bỏ comment nếu là đồ thị vô hướng

        // 2. Cho đồ thị có trọng số (Dijkstra)
        adj_weighted[u].pb({v, w});
        adj_weighted[v].pb({u, w}); // Bỏ comment nếu là đồ thị vô hướng

        // 3. Cho Kruskal
        edge_list.pb({u, v, w});
    }

    // --- VÍ DỤ CÁCH GỌI HÀM ---

    // // Ví dụ 1: Chạy DFS từ đỉnh 1
    // fill(all(visited), false);
    // dfs(1);

    // // Ví dụ 2: Chạy BFS từ đỉnh 1 và in khoảng cách
    // fill(all(visited), false);
    // vi dist_bfs = bfs(1);
    // for (int i = 1; i <= n; ++i) {
    //     cout << "Dist from 1 to " << i << " = " << dist_bfs[i] << "\n";
    // }

    // // Ví dụ 3: Chạy Dijkstra từ đỉnh 1 và in khoảng cách
    // vll dist_dijkstra = dijkstra(1);
    // for (int i = 1; i <= n; ++i) {
    //     cout << "Shortest path from 1 to " << i << " = " << dist_dijkstra[i] << "\n";
    // }

    // // Ví dụ 4: Chạy Kruskal
    // DSU dsu_kruskal(n);
    // ll mst_cost = kruskal(edge_list, dsu_kruskal);
    // cout << "MST cost: " << mst_cost << "\n";
    
    // // Ví dụ 5: Chạy TopoSort
    // vi topo_order = kahn_topo_sort();
    // if (sz(topo_order) < n) {
    //     cout << "Graph has a cycle!\n";
    // } else {
    //     cout << "Topo order: ";
    //     for (int node : topo_order) {
    //         cout << node << " ";
    //     }
    //     cout << "\n";
    // }
}

/**
 * ========================================
 * HÀM MAIN()
 * ========================================
 */
int main() {
    fast_io();

    int t = 1;
    cin >> t; // Đọc số lượng test cases

    while (t--) {
        solve();
    }

    // Nếu bài chỉ có 1 test case:
    // solve();

    return 0;
}