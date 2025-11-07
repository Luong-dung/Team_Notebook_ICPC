// 1. STRING HASHING (DOUBLE HASHING)
struct StringHash {
    vll hash1, hash2;vll pow1, pow2;int n;

    StringHash(const string& s) {
        n = sz(s);
        hash1.resize(n + 1);hash2.resize(n + 1);
        pow1.resize(n + 1);pow2.resize(n + 1);
        pow1[0] = pow2[0] = 1;hash1[0] = hash2[0] = 0;

        for (int i = 0; i < n; ++i) {
            pow1[i + 1] = (pow1[i] * BASE) % MOD1;
            pow2[i + 1] = (pow2[i] * BASE) % MOD2;
            hash1[i + 1] = (hash1[i] * BASE + (s[i] - 'a' + 1)) % MOD1;
            hash2[i + 1] = (hash2[i] * BASE + (s[i] - 'a' + 1)) % MOD2;
        }
    }
    // Lấy hash của chuỗi con s[l...r] (0-based)
    pll get_hash(int l, int r) {
        ll h1 = (hash1[r + 1] - (hash1[l] * pow1[r - l + 1]) % MOD1 + MOD1) % MOD1;
        ll h2 = (hash2[r + 1] - (hash2[l] * pow2[r - l + 1]) % MOD2 + MOD2) % MOD2;
        return {h1, h2};
    }
};
// 2. KMP (KNUTH-MORRIS-PRATT)
// Xây dựng mảng LPS (Longest Proper Prefix which is also Suffix)
vi build_lps(const string& pattern) {
    int i = 1;int m = sz(pattern);vi lps(m, 0);int len = 0; // Độ dài của lps trước đó

    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++; lps[i] = len; i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0; i++;
            }
        }
    }
    return lps;
}
// Tìm tất cả vị trí xuất hiện của pattern trong text
vi kmp_search(const string& text, const string& pattern) {
    int n = sz(text);int m = sz(pattern);
    if (m == 0) return {};
    vi lps = build_lps(pattern);vi matches;
    int i = 0;int j = 0; // con trỏ cho pattern

    while (i < n) {
        if (pattern[j] == text[i]) {
            i++; j++;
        }
        if (j == m) {
            matches.pb(i - j); // Tìm thấy 1 match tại vị trí i - j
            j = lps[j - 1];
        } else if (i < n && pattern[j] != text[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
    return matches;
}
// 3. Z-ALGORITHM
// Z[i] = độ dài chuỗi con dài nhất bắt đầu từ i
//        và cũng là tiền tố của s. Z[0] = 0.
vi z_function(const string& s) {
    int n = sz(s);vi z(n, 0);int l = 0, r = 0;
    for (int i = 1; i < n; ++i) {
        if (i < r) z[i] = min(r - i, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
        if (i + z[i] > r) {
            l = i; r = i + z[i];
        }
    }
    return z;
}
// 4. TRIE (PREFIX TREE)
struct TrieNode {
    TrieNode* children[26];
    bool is_end_of_word;int count; // Đếm số từ đi qua node này (prefix count)
    TrieNode() {
        is_end_of_word = false;
        count = 0;
        for (int i = 0; i < 26; ++i) {
            children[i] = nullptr;
        }
    }
};

class Trie {
public:
    TrieNode* root;
    Trie() {
        root = new TrieNode();
    }

    void insert(const string& word) {
        TrieNode* curr = root;
        for (char c : word) {
            int idx = c - 'a';
            if (curr->children[idx] == nullptr) {
                curr->children[idx] = new TrieNode();
            }
            curr = curr->children[idx];
            curr->count++;
        }
        curr->is_end_of_word = true;
    }

    bool search(const string& word) {
        TrieNode* curr = root;
        for (char c : word) {
            int idx = c - 'a';
            if (curr->children[idx] == nullptr) {
                return false;
            }
            curr = curr->children[idx];
        }
        return curr->is_end_of_word;
    }

    int count_prefix(const string& prefix) {
        TrieNode* curr = root;
        for (char c : prefix) {
            int idx = c - 'a';
            if (curr->children[idx] == nullptr) {
                return 0;
            }
            curr = curr->children[idx];
        }
        return curr->count;
    }
};
// 5. MANACHER'S ALGORITHM
// Tìm chuỗi con đối xứng dài nhất trong O(N)
// Trả về {độ dài, chuỗi con}
pair<int, string> manacher(const string& s) {
    string t = "$#";
    for (char c : s) {
        t += c;
        t += '#';
    }
    t += '@';
    
    int n = sz(t);int c = 0, r = 0; // c = tâm, r = bán kính phải
    vi p(n, 0); // p[i] = bán kính của palindrome tâm i
    for (int i = 1; i < n - 1; ++i) {
        int mirr = 2 * c - i; // i' = mirror của i
        if (i < r) {
            p[i] = min(r - i, p[mirr]);
        }
        
        while (t[i + (1 + p[i])] == t[i - (1 + p[i])]) {
            p[i]++;
        }
        
        if (i + p[i] > r) {
            c = i;
            r = i + p[i];
        }
    }
    
    int max_len = 0;int center_idx = 0;
    for (int i = 1; i < n - 1; ++i) {
        if (p[i] > max_len) {
            max_len = p[i];
            center_idx = i;
        }
    }
    // (center_idx - 1 - max_len) / 2 là vị trí bắt đầu trong chuỗi s gốc
    return {max_len, s.substr((center_idx - 1 - max_len) / 2, max_len)};
}
// 6. SUFFIX ARRAY (O(N log N))
// sa[i] = vị trí bắt đầu của hậu tố nhỏ thứ i
// pos[i] = xếp hạng của hậu tố bắt đầu tại i
vi sa;vi lcp; // LCP Array
void build_suffix_array(const string& s) {
    string t = s + "$"; // Thêm ký tự $ nhỏ nhất
    int n = sz(t);
    sa.assign(n, 0);
    vi pos(n, 0); // p
    vi c(n, 0);   // c
    // k = 0 (sắp xếp theo 1 ký tự)
    {
        vector<pair<char, int>> a(n);
        for(int i = 0; i < n; ++i) a[i] = {t[i], i};
        sort(all(a));
        for(int i = 0; i < n; ++i) sa[i] = a[i].sc;
        c[sa[0]] = 0;
        for(int i = 1; i < n; ++i) {
            c[sa[i]] = c[sa[i-1]] + (a[i].ft != a[i-1].ft);
        }
    }
    // k -> k + 1
    int k = 0;
    while((1 << k) < n) {
        // Sắp xếp theo cặp (c[i], c[i + 2^k])
        // Dùng counting sort (radix sort)
        vi pn(n), cn(n); vi cnt(n, 0);

        for (int i = 0; i < n; ++i) pn[i] = (sa[i] - (1 << k) + n) % n;
        for (int i = 0; i < n; ++i) cnt[c[pn[i]]]++;
        for (int i = 1; i < n; ++i) cnt[i] += cnt[i-1];
        for (int i = n - 1; i >= 0; --i) sa[--cnt[c[pn[i]]]] = pn[i];

        cn[sa[0]] = 0;
        for(int i = 1; i < n; ++i) {
            pll cur = {c[sa[i]], c[(sa[i] + (1 << k)) % n]};
            pll prev = {c[sa[i-1]], c[(sa[i-1] + (1 << k)) % n]};
            cn[sa[i]] = cn[sa[i-1]] + (cur != prev);
        }
        c = cn;
        k++;
    }
    // sa.erase(sa.begin()); // Xóa $ nếu cần
}
// 7. LCP ARRAY (O(N)) - Kasai's Algorithm
// lcp[i] = LCP(sa[i], sa[i-1])
// Cần Suffix Array (sa) đã được build
void build_lcp_array(const string& s) {
    int n = sz(s) + 1; // +1 cho ký tự $
    if (sa.empty() || sz(sa) != n) build_suffix_array(s);
    
    lcp.assign(n, 0);
    vi pos(n, 0); // pos[i] = rank của hậu tố s[i...]
    for (int i = 0; i < n; ++i) {
        pos[sa[i]] = i;
    }

    int k = 0; // k = LCP của hậu tố trước đó
    for (int i = 0; i < n - 1; ++i) { // i là vị trí bắt đầu
        if (pos[i] == n - 1) {
            k = 0; continue;
        }
        int j = sa[pos[i] + 1]; // j = vị trí bắt đầu của hậu tố ngay sau
        while (i + k < n && j + k < n && s[i + k] == s[j + k]) k++;
        lcp[pos[i] + 1] = k;
        if (k > 0) k--;
    }
    // lcp.erase(lcp.begin()); // Xóa lcp của $ nếu cần
}
void solve() {
    string text, pattern;
    cin >> text >> pattern;

    // --- VÍ DỤ CÁCH GỌI HÀM ---

    // // 1. String Hashing
    // StringHash text_hash(text);
    // StringHash pattern_hash(pattern);
    // pll pattern_h = pattern_hash.get_hash(0, sz(pattern) - 1);
    // for (int i = 0; i <= sz(text) - sz(pattern); ++i)
    //     if (text_hash.get_hash(i, i + sz(pattern) - 1) == pattern_h)
    //         cout << "Hash match at index " << i << "\n";

    // // 2. KMP
    // vi matches = kmp_search(text, pattern);
    // cout << "KMP matches: ";
    // for (int idx : matches)
    //     cout << idx << " ";
    // cout << "\n";

    // // 3. Z-Algorithm (dùng để tìm pattern)
    // string combined = pattern + "$" + text;
    // vi z = z_function(combined);
    // cout << "Z-algo matches: ";
    // for (int i = sz(pattern) + 1; i < sz(z); ++i) {
    //     if (z[i] == sz(pattern)) {
    //         cout << i - (sz(pattern) + 1) << " ";
    //     }
    // }
    // cout << "\n";
    
    // // 4. Trie
    // Trie trie;
    // trie.insert("hello");// trie.insert("hell");// trie.insert("world");
    // cout << "Search 'hello': " << trie.search("hello") << "\n";
    // cout << "Search 'hel': " << trie.search("hel") << "\n";
    // cout << "Count prefix 'hel': " << trie.count_prefix("hel") << "\n";

    // // 5. Manacher
    // pair<int, string> lps = manacher(text);
    // cout << "Longest Palindromic Substring: " << lps.sc << " (length " << lps.ft << ")\n";

    // // 6. Suffix Array + LCP
    // build_suffix_array(text);
    // build_lcp_array(text);
    
    // // sa[0] luôn là $ (chuỗi rỗng)
    // cout << "Suffix Array (bỏ $):\n";
    // for (int i = 1; i < sz(sa); ++i)
    //     cout << "Rank " << i << ": " << sa[i] << " -> " << text.substr(sa[i]) << "\n";
    // cout << "LCP Array (bỏ $):\n";
    // for (int i = 2; i < sz(lcp); ++i) // Bắt đầu từ 2 để bỏ LCP($, ...)
    //     cout << "LCP(" << sa[i-1] << ", " << sa[i] << ") = " << lcp[i] << "\n";
}
