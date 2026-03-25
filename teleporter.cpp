#include <bits/stdc++.h>
using namespace std;
#define int long long
#define INF (int)1e18

mt19937_64 RNG(chrono::steady_clock::now().time_since_epoch().count());

const int N = 1e5 + 69;
pair <int, int> seg[4 * N];
int lz[4 * N];

void updlz(int l, int r, int pos){
    seg[pos].first += lz[pos];
    if (l != r){
        lz[pos * 2] += lz[pos];
        lz[pos * 2 + 1] += lz[pos];
    }
    lz[pos] = 0;
}

void upd(int l, int r, int pos, int qp, int v1, int v2){
    updlz(l, r, pos);
    if (!(l <= qp && qp <= r)){
        return;
    }
    if (l == r){
        seg[pos] = make_pair(v1, v2);
        return;
    }
    int mid = (l + r) / 2;
    upd(l, mid, pos * 2, qp, v1, v2);
    upd(mid + 1, r, pos * 2 + 1, qp, v1, v2);
    seg[pos] = min(seg[pos * 2], seg[pos * 2 + 1]);
}

void add(int l, int r, int pos, int ql, int qr, int v){
    updlz(l, r, pos);
    if (l >= ql && r <= qr){
        lz[pos] += v;
        updlz(l, r, pos);
    } else if (l > qr || r < ql){
        
    } else {
        int mid = (l + r) / 2;
        add(l, mid, pos * 2, ql, qr, v);
        add(mid + 1, r, pos * 2 + 1, ql, qr, v);
        seg[pos] = min(seg[pos * 2], seg[pos * 2 + 1]);
    }
}

void Solve() 
{
    int n, m, k; cin >> n >> m >> k;
    
    vector <int> s(m), t(m), c(m);
    for (int i = 0; i < m; i++){
        cin >> s[i] >> t[i] >> c[i];
    }
    
    vector<vector<pair<int, int>>> adj(n + 2);
    for (int i = 0; i < m; i++){
        adj[t[i]].push_back({s[i], c[i]});
    }

    k = min(k, n);
    
    auto get = [&](int penalty){
        vector <pair<int, int>> dp(n + 2, make_pair(INF, 0));
        
        for (int i = 0; i <= 4 * (n + 1); i++){
            lz[i] = 0;
            seg[i] = make_pair(INF, 0);
        }
        
        dp[0] = make_pair(0, 0);
        for (int i = 0; i <= n + 1; i++){
            if (i > 0){
                auto [v1, v2] = seg[1];
                dp[i] = make_pair(v1 + penalty, v2 + 1);
            }
            
            if (i <= n){
                upd(0, n, 1, i, dp[i].first, dp[i].second);
            }
            
            for (auto [s, c] : adj[i]){
                add(0, n, 1, 0, s, c);
            }
        }
        return dp[n + 1];
    };
    
    int lo = 0, hi = 1e14;
    while (lo != hi){
        int mid = (lo + hi) / 2;
        
        auto [x, cnt] = get(mid);
        if (cnt <= k + 1){
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    
    auto [x, cnt] = get(lo);
    int ans = (x - (k + 1) * lo);
    cout << ans << "\n";
}

int32_t main() 
{
    auto begin = std::chrono::high_resolution_clock::now();
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int t = 1;
    // freopen("in",  "r", stdin);
    // freopen("out", "w", stdout);
    
 //   cin >> t;
    for(int i = 1; i <= t; i++) 
    {
        //cout << "Case #" << i << ": ";
        Solve();
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    cerr << "Time measured: " << elapsed.count() * 1e-9 << " seconds.\n"; 
    return 0;
}
