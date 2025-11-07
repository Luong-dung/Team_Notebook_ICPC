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
mt19937 rng(chrono::high_resolution_clock::now().time_since_epoch().count());

ll rand(ll left,ll right){
    uniform_int_distribution<ll> rd(left,right);
    return rd(rng);
}
