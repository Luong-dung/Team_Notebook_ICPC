#include <bits/stdc++.h>

using namespace std;

#define ar array
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define rep(i, a, b) for (int i = (a); i < (b); ++i)
#define fi first
#define se second
#define PB push_back
#define MP make_pair

using ll = long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;

using ld = long double;
using vpii = vector<pii>;      // vector<pair<int, int>>
using vpll = vector<pll>;      // vector<pair<long long, long long>>
using vvi = vector<vi>;        // vector<vector<int>>

const int MAX_N = 1e5 + 5;
const long long MOD = 1e9 + 7;
const long long INF = 1e9;
const ll MOD1 = 1e9 + 7;
const ll MOD2 = 1e9 + 9;
const int BASE = 31; // Hoặc 53, 313, 331,... (số nguyên tố)
const int MAXN = 1e6 + 5; // Kích thước tối đa, thay đổi nếu cần

void fast_io() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
}

// Binary Search Snippet (Standard Lower Bound)
// Finds the first element >= x
int bin_search_lower(const vector<int>& arr, int x) {
    int l = 0, r = arr.size() - 1;
    int ans = -1;
    while (l <= r) {
        int mid = l + (r - l) / 2;
        if (arr[mid] >= x) {
            ans = mid;
            r = mid - 1;
        } else {
            l = mid + 1;
        }
    }
    return ans;
}

// Binary Search on Answer Snippet
// Finds the maximum value 'ans' such that check(ans) is true
bool check(int val) {
    // Check function logic here
    return true; 
}

int bin_search_answer() {
    int l = 0, r = 1e9; 
    int ans = 0;
    while (l <= r) {
        int mid = l + (r - l) / 2;
        if (check(mid)) {
            ans = mid;
            l = mid + 1;
        } else {
            r = mid - 1;
        }
    }
    return ans;
}


void solve() {
    // Your code here
}

int main() {
    fast_io();
    // int tc = 1;
    // cin >> tc;
    // for (int t = 1; t <= tc; t++) {
        solve();
    // }
    return 0;
}
