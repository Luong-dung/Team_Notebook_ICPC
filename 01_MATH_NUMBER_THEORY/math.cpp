#include <bits/stdc++.h>
using namespace std;

struct Math {
    using ll = long long;
    const ll MOD = 1e9 + 7; // chỉnh tùy bài
    // ---------- Catalan ---------
    /*
        1/(n + 1) * (2n)C(n)
    */

    // ---------- Stars and bars --------
    /*
        x1​ + x2 ​+ ⋯ + xd ​= s
        0 ≤ xi​ ≤ b
        ans =  ∑(i : 0 -> d)​ (−1)^i * (d C i​)(d−1 C s+d−1−(b+1)i​)
    */
    // ---------- Basic ----------
    ll add(ll a, ll b) { a += b; if (a >= MOD) a -= MOD; return a; }
    ll sub(ll a, ll b) { a -= b; if (a < 0) a += MOD; return a; }
    ll mul(ll a, ll b) { return (a * 1LL * b) % MOD; }
    ll power(ll a, ll b) {
        ll res = 1;
        while (b) {
            if (b & 1) res = mul(res, a);
            a = mul(a, a);
            b >>= 1;
        }
        return res;
    }
    ll inv(ll a) { return power(a, MOD - 2); } // MOD phải là prime

    // ---------- GCD & LCM ----------
    ll gcd(ll a, ll b) { return b ? gcd(b, a % b) : a; }
    ll lcm(ll a, ll b) { return a / gcd(a, b) * b; }

    // ---------- Sieve ----------
    vector<int> primes;
    vector<bool> is_prime;
    void sieve(int n) {
        is_prime.assign(n + 1, true);
        is_prime[0] = is_prime[1] = false;
        for (int i = 2; i * i <= n; i++)
            if (is_prime[i])
                for (int j = i * i; j <= n; j += i)
                    is_prime[j] = false;
        for (int i = 2; i <= n; i++)
            if (is_prime[i]) primes.push_back(i);
    }

    // ---------- Prime Factorization ----------
    vector<pair<ll,int>> factorize(ll n) {
        vector<pair<ll,int>> res;
        for (ll p = 2; p * p <= n; p++) {
            if (n % p == 0) {
                int cnt = 0;
                while (n % p == 0) n /= p, cnt++;
                res.push_back({p, cnt});
            }
        }
        if (n > 1) res.push_back({n, 1});
        return res;
    }

    // ---------- Modular Combinatorics ----------
    vector<ll> fact, invfact;
    void init_combi(int n) {
        fact.resize(n + 1);
        invfact.resize(n + 1);
        fact[0] = 1;
        for (int i = 1; i <= n; i++) fact[i] = mul(fact[i - 1], i);
        invfact[n] = inv(fact[n]);
        for (int i = n - 1; i >= 0; i--) invfact[i] = mul(invfact[i + 1], i + 1);
    }
    ll C(int n, int k) {
        if (k < 0 || k > n) return 0;
        return mul(fact[n], mul(invfact[k], invfact[n - k]));
    }

    // ---------- Euler Phi ----------
    ll phi(ll n) {
        ll res = n;
        for (ll p = 2; p * p <= n; p++) {
            if (n % p == 0) {
                while (n % p == 0) n /= p;
                res -= res / p;
            }
        }
        if (n > 1) res -= res / n;
        return res;
    }

    // ---------- Extended GCD (for non-prime MOD) ----------
    ll extgcd(ll a, ll b, ll &x, ll &y) {
        if (!b) { x = 1; y = 0; return a; }
        ll x1, y1;
        ll g = extgcd(b, a % b, x1, y1);
        x = y1;
        y = x1 - y1 * (a / b);
        return g;
    }

    ll modinv_general(ll a, ll m) {
        ll x, y;
        ll g = extgcd(a, m, x, y);
        if (g != 1) return -1; // không tồn tại
        x = (x % m + m) % m;
        return x;
    }

    // ---------- Miller Rabin ---------------

    bool test(long long a, long long n, long long k, long long m)
    {
        long long mod = binaryPower(a, m, n); // a^m % n
        if (mod == 1 || mod == n - 1)
                return true;
        for (int l = 1; l < k; ++l)
        {
            mod = (mod * mod) % n;
            if (mod == n - 1)
                return true;
        }
        return false;
    }
    bool MillerRabin(long long n)
    {
        static vector<int> checkSet = {2,3,5,7,11,13,17,19,23,29,31,37};
        for (auto a : checkSet)
            if (n == a)
                return true;
        if (n < 41)
            return false;

        long long k = 0, m = n - 1;
        while (m % 2 == 0)
        {
            m /= 2;
            k++;
        }

        for (auto a : checkSet)
            if (!test(a, n, k, m))
                return false;
        return true;
    }


};

