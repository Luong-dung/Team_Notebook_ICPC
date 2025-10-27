/**
 * ===================================================
 * TEMPLATE ADVANCED TOPICS (REST)
 * * Bao gồm:
 * 1. Matrix Exponentiation (Lũy thừa ma trận)
 * 2. Dinic's Algorithm (Max Flow)
 * ===================================================
 */

#include <bits/stdc++.h>

using namespace std;
// Định nghĩa kiểu dữ liệu
using ll = long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;
using vpii = vector<pii>;
using vpll = vector<pll>;
using vvi = vector<vi>;

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
// const int MOD = 998244353;
const int MAXN = 2e5 + 5; 

// ========================================
// 1. MATRIX EXPONENTIATION
// ========================================
// (Phụ thuộc vào binpow(a, b, m) từ file math)
/*
// Cần hàm binpow từ file math.h
ll binpow(ll a, ll b, ll m) {
    a %= m;
    ll res = 1;
    while (b > 0) {
        if (b & 1) res = (res * a) % m;
        a = (a * a) % m;
        b >>= 1;
    }
    return res;
}
*/

struct Matrix {
    int dim;
    vector<vll> mat;
    Matrix(int _dim) : dim(_dim), mat(_dim, vll(_dim, 0)) {}

    // Phép nhân ma trận
    Matrix operator*(const Matrix& other) const {
        Matrix result(dim);
        for (int i = 0; i < dim; ++i) {
            for (int j = 0; j < dim; ++j) {
                for (int k = 0; k < dim; ++k) {
                    result.mat[i][j] = (result.mat[i][j] + mat[i][k] * other.mat[k][j]) % MOD;
                }
            }
        }
        return result;
    }
    
    // Tạo ma trận đơn vị
    static Matrix identity(int dim) {
        Matrix I(dim);
        for (int i = 0; i < dim; ++i) I.mat[i][i] = 1;
        return I;
    }
};

// Lũy thừa ma trận (dùng binpow)
Matrix matrix_pow(Matrix a, ll b) {
    Matrix res = Matrix::identity(a.dim);
    while (b > 0) {
        if (b & 1) res = res * a;
        a = a * a;
        b >>= 1;
    }
    return res;
}

// ========================================
// 2. DINIC'S ALGORITHM (MAX FLOW)
// ========================================
struct Edge {
    int to;
    ll cap;  // Khả năng thông qua
    ll flow; // Luồng hiện tại
    int rev; // Chỉ số của cạnh ngược
};

struct Dinic {
    int n;
    vector<vector<Edge>> adj;
    vi level; // Đồ thị mức (cho BFS)
    vi ptr;   // Con trỏ (cho DFS)

    Dinic(int _n) : n(_n), adj(_n) {}

    void add_edge(int u, int v, ll cap) {
        Edge fwd = {v, cap, 0, sz(adj[v])};
        Edge bwd = {u, 0, 0, sz(adj[u])}; // Cạnh ngược ban đầu có cap = 0
        adj[u].pb(fwd);
        adj[v].pb(bwd);
    }

    // Xây dựng đồ thị mức
    bool bfs(int s, int t) {
        level.assign(n, -1);
        level[s] = 0;
        queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto& edge : adj[u]) {
                if (edge.cap - edge.flow > 0 && level[edge.to] == -1) {
                    level[edge.to] = level[u] + 1;
                    q.push(edge.to);
                }
            }
        }
        return level[t] != -1;
    }

    // Đẩy luồng bằng DFS
    ll dfs(int u, int t, ll pushed) {
        if (pushed == 0) return 0;
        if (u == t) return pushed;
        
        for (int& cid = ptr[u]; cid < sz(adj[u]); ++cid) {
            auto& edge = adj[u][cid];
            int v = edge.to;
            if (level[u] + 1 != level[v] || edge.cap - edge.flow == 0) continue;
            
            ll tr = dfs(v, t, min(pushed, edge.cap - edge.flow));
            if (tr == 0) continue;
            
            edge.flow += tr;
            adj[v][edge.rev].flow -= tr; // Cập nhật cạnh ngược
            return tr;
        }
        return 0;
    }

    // Tính luồng cực đại từ s đến t
    ll max_flow(int s, int t) {
        ll total_flow = 0;
        while (bfs(s, t)) {
            ptr.assign(n, 0);
            while (ll pushed = dfs(s, t, LINF)) {
                total_flow += pushed;
            }
        }
        return total_flow;
    }
};


/**
 * ========================================
 * HÀM SOLVE()
 * Nơi đọc input và gọi các thuật toán
 * ========================================
 */
void solve() {
    int n, m;
    cin >> n >> m;

    // --- VÍ DỤ CÁCH GỌI HÀM ---
    
    // // 1. Matrix Exponentiation (Ví dụ: tính Fibonacci)
    // Matrix T(2);
    // T.mat[0][0] = 1; T.mat[0][1] = 1;
    // T.mat[1][0] = 1; T.mat[1][1] = 0;
    // Matrix T_n = matrix_pow(T, n - 1);
    // // (F_n, F_{n-1}) = (F_1, F_0) * T^(n-1) = (1, 0) * T^(n-1)
    // cout << "Fib(n) = " << T_n.mat[0][0] << "\n";

    // // 2. Dinic Max Flow (Giả sử 0 là nguồn, n-1 là đích)
    // Dinic flow_solver(n);
    // for (int i = 0; i < m; ++i) {
    //     int u, v;
    //     ll cap;
    //     cin >> u >> v >> cap;
    //     flow_solver.add_edge(u, v, cap);
    // }
    // cout << "Max flow = " << flow_solver.max_flow(0, n - 1) << "\n";

}

/**
 * ========================================
 * HÀM MAIN()
 * ========================================
 */
int main() {

    int t = 1;
    cin >> t; // Đọc số lượng test cases

    while (t--) {
        solve();
    }

    // Nếu bài chỉ có 1 test case:
    // solve();

    return 0;
}