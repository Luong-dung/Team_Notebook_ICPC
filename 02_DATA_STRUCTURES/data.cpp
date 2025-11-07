struct DataStructures {
    using ll = long long;
    // ==================== Fenwick Tree (BIT) ====================
    struct Fenwick {
        int n;
        vector<ll> bit;
        Fenwick(int n = 0) { init(n); }
        void init(int n_) { n = n_; bit.assign(n + 1, 0); }
        void add(int i, ll val) {
            for (; i <= n; i += i & -i) bit[i] += val;
        }
        ll sum(int i) {
            ll res = 0;
            for (; i > 0; i -= i & -i) res += bit[i];
            return res;
        }
        ll range_sum(int l, int r) { return sum(r) - sum(l - 1); }
    };
    // ==================== Disjoint Set Union (Union-Find) ====================
    struct DSU {
        vector<int> p, sz;
        DSU(int n = 0) { init(n); }
        void init(int n) {
            p.resize(n + 1);
            sz.assign(n + 1, 1);
            iota(p.begin(), p.end(), 0);
        }
        int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
        bool unite(int a, int b) {
            a = find(a); b = find(b);
            if (a == b) return false;
            if (sz[a] < sz[b]) swap(a, b);
            p[b] = a;
            sz[a] += sz[b];
            return true;
        }
    };
    // ==================== Segment Tree ====================
    struct SegmentTree {
        int n;
        vector<ll> st;
        SegmentTree(int n = 0) { init(n); }
        void init(int n_) { n = n_; st.assign(4 * n, 0); }
        void build(vector<ll>& a, int id, int l, int r) {
            if (l == r) { st[id] = a[l]; return; }
            int mid = (l + r) >> 1;
            build(a, id << 1, l, mid);
            build(a, id << 1 | 1, mid + 1, r);
            st[id] = st[id << 1] + st[id << 1 | 1];
        }
        void build(vector<ll>& a) { build(a, 1, 1, n); }
        void update(int id, int l, int r, int pos, ll val) {
            if (l == r) { st[id] = val; return; }
            int mid = (l + r) >> 1;
            if (pos <= mid) update(id << 1, l, mid, pos, val);
            else update(id << 1 | 1, mid + 1, r, pos, val);
            st[id] = st[id << 1] + st[id << 1 | 1];
        }
        void update(int pos, ll val) { update(1, 1, n, pos, val); }
        ll query(int id, int l, int r, int u, int v) {
            if (v < l || r < u) return 0;
            if (u <= l && r <= v) return st[id];
            int mid = (l + r) >> 1;
            return query(id << 1, l, mid, u, v)
                 + query(id << 1 | 1, mid + 1, r, u, v);
        }
        ll query(int l, int r) { return query(1, 1, n, l, r); }
    };
    // ==================== Segment Tree with Lazy Propagation ====================
    struct SegmentTreeLazy {
        int n;
        vector<ll> st, lazy;
        SegmentTreeLazy(int n = 0) { init(n); }
        void init(int n_) { n = n_; st.assign(4 * n, 0); lazy.assign(4 * n, 0); }
        void push(int id, int l, int r) {
            if (lazy[id] == 0) return;
            st[id] += 1LL * (r - l + 1) * lazy[id];
            if (l != r) {
                lazy[id << 1] += lazy[id];
                lazy[id << 1 | 1] += lazy[id];
            }
            lazy[id] = 0;
        }
        void update(int id, int l, int r, int u, int v, ll val) {
            push(id, l, r);
            if (v < l || r < u) return;
            if (u <= l && r <= v) {
                lazy[id] += val;
                push(id, l, r);
                return;
            }
            int mid = (l + r) >> 1;
            update(id << 1, l, mid, u, v, val);
            update(id << 1 | 1, mid + 1, r, u, v, val);
            st[id] = st[id << 1] + st[id << 1 | 1];
        }
        ll query(int id, int l, int r, int u, int v) {
            push(id, l, r);
            if (v < l || r < u) return 0;
            if (u <= l && r <= v) return st[id];
            int mid = (l + r) >> 1;
            return query(id << 1, l, mid, u, v)
                 + query(id << 1 | 1, mid + 1, r, u, v);
        }
    };
    // ==================== Sparse Table (for RMQ / GCD / etc.) ====================
    struct SparseTable {
        int n, K;
        vector<vector<ll>> st;
        function<ll(ll,ll)> f;
        SparseTable() {}
        SparseTable(vector<ll>& a, function<ll(ll,ll)> func) {
            build(a, func);
        }
        void build(vector<ll>& a, function<ll(ll,ll)> func) {
            f = func;
            n = (int)a.size();
            K = __lg(n) + 1;
            st.assign(K, vector<ll>(n));
            st[0] = a;
            for (int k = 1; k < K; k++) {
                for (int i = 0; i + (1 << k) <= n; i++) {
                    st[k][i] = f(st[k-1][i], st[k-1][i + (1 << (k-1))]);
                }
            }
        }
        ll query(int l, int r) {
            int k = __lg(r - l + 1);
            return f(st[k][l], st[k][r - (1 << k) + 1]);
        }
    };
    // ================= LICHAO TREE (Convex-Hull trick) ==============================
    struct Line {
        int a, b;
        Line(int _a = 0, int _b = (long long)1e18) : a(_a), b(_b) {}
    };
    struct LichaoTree {
        const int MAXX;
        vector<Line> tree;

        LichaoTree(int _MAXX) : MAXX(_MAXX) {
            tree.assign(4 * MAXX + 5, Line());
        }

        int f(const Line& line, int x) {
            return line.a * x + line.b;
        }

        void update(int id, int l, int r, Line line) {
            int mid = (l + r) / 2;
            bool left = f(tree[id], l) < f(line, l);
            bool m = f(tree[id], mid) < f(line, mid);

            if (!m) swap(tree[id], line);

            if (l == r - 1) return;

            if (left != m) update(id * 2, l, mid, line);
            else update(id * 2 + 1, mid, r, line);
        }

        int get(int id, int l, int r, int x) {
            int mid = (l + r) / 2;
            int ans = f(tree[id], x);

            if (l == r - 1) return ans;

            if (x < mid) return min(ans, get(id * 2, l, mid, x));
            else return min(ans, get(id * 2 + 1, mid, r, x));
        }

        void addLine(Line line) {
            update(1, 1, MAXX, line);
        }

        int query(int x) {
            return get(1, 1, MAXX, x);
        }
    };
};
