#include <bits/stdc++.h>
using namespace std;
#define int long long
#define INF (int)1e18

mt19937_64 RNG(chrono::steady_clock::now().time_since_epoch().count());

const int N = 4e5 + 69;
int seg[4 * N], lz[4 * N];

void build(int l, int r, int pos){
    seg[pos] = 0;
    lz[pos] = 0;
    if (l == r) return;
    
    int mid = (l + r) / 2;
    build(l, mid, pos * 2);
    build(mid + 1, r, pos * 2 + 1);
}

void updlz(int l, int r, int pos){
    seg[pos] += lz[pos];
    if (l != r){
        lz[pos * 2] += lz[pos];
        lz[pos * 2 + 1] += lz[pos];
    }
    lz[pos] = 0;
}

void upd(int l, int r, int pos, int ql, int qr, int v){
    updlz(l, r, pos);
    if (l >= ql && r <= qr){
        lz[pos] += v;
        updlz(l, r, pos);
    } else if (l > qr || r < ql){
        
    } else {
        int mid = (l + r) / 2;
        upd(l, mid, pos * 2, ql, qr, v);
        upd(mid + 1, r, pos * 2 + 1, ql, qr, v);
        seg[pos] = max(seg[pos * 2], seg[pos * 2 + 1]);
    }
}

void Solve() 
{
    int h, w, n, x; cin >> h >> w >> n >> x;
    
    vector <int> u(n), d(n), l(n), r(n), c(n);
    
    vector <int> v1, v2;
    for (int i = 0; i < n; i++){
        cin >> u[i] >> d[i] >> l[i] >> r[i] >> c[i];
        v1.push_back(u[i]);
        v1.push_back(d[i]);
        v2.push_back(l[i]);
        v2.push_back(r[i]);
    }
    
    sort(v1.begin(), v1.end());
    sort(v2.begin(), v2.end());
    
    v1.erase(unique(v1.begin(), v1.end()), v1.end());
    v2.erase(unique(v2.begin(), v2.end()), v2.end());
    
    for (int i = 0; i < n; i++){
        u[i] = upper_bound(v1.begin(), v1.end(), u[i]) - v1.begin();
        d[i] = upper_bound(v1.begin(), v1.end(), d[i]) - v1.begin();
        l[i] = upper_bound(v2.begin(), v2.end(), l[i]) - v2.begin();
        r[i] = upper_bound(v2.begin(), v2.end(), r[i]) - v2.begin();
    }
    
    auto solve1 = [&](vector <int> u, vector <int> d, vector <int> l, vector <int> r, vector <int> c, vector <int> v1, vector <int> v2){
        int h = v1.size();
        int w = v2.size();
        
        build(1, w, 1);
        
        vector<vector<int>> add(h + 1), era(h + 1);
        for (int i = 0; i < n; i++){
            add[d[i]].push_back(i);
            era[u[i]].push_back(i);
        }
        int T = n - 1;
        vector <int> ans(n, -INF);
        for (int row = h; row >= 1; row--){
            for (auto i : add[row]){
                if (i <= T){
                    upd(1, w, 1, l[i], r[i], c[i]);
                }
            }
            
            while (seg[1] >= x){
                ans[T] = v1[row - 1];
                if (u[T] <= row && row <= d[T]){
                    upd(1, w, 1, l[T], r[T], -c[T]);
                }
                T--;
            }
            
            for (auto i : era[row]){
                if (i <= T){
                    upd(1, w, 1, l[i], r[i], -c[i]);
                }
            }
        }
        
        return ans;
    };
    
    auto solve2 = [&](vector <int> u, vector <int> d, vector <int> l, vector <int> r, vector <int> c, vector <int> v1, vector <int> v2){
        int h = v1.size();
        int w = v2.size();
        
        build(1, w, 1);
        
        vector<vector<int>> add(h + 1), era(h + 1);
        for (int i = 0; i < n; i++){
            add[u[i]].push_back(i);
            era[d[i]].push_back(i);
        }
        int T = n - 1;
        vector <int> ans(n, INF);
        for (int row = 1; row <= h; row++){
            for (auto i : add[row]){
                if (i <= T){
                    upd(1, w, 1, l[i], r[i], +c[i]);
                }
            }
            
            while (seg[1] >= x){
                ans[T] = v1[row - 1];
                if (u[T] <= row && row <= d[T]){
                    upd(1, w, 1, l[T], r[T], -c[T]);
                }
                T--;
            }
            
            for (auto i : era[row]){
                if (i <= T){
                    upd(1, w, 1, l[i], r[i], -c[i]);
                }
            }
        }
        
        return ans;
    };
    
    auto max_row = solve1(u, d, l, r, c, v1, v2);
    auto min_row = solve2(u, d, l, r, c, v1, v2);
    auto max_col = solve1(l, r, u, d, c, v2, v1);
    auto min_col = solve2(l, r, u, d, c, v2, v1);
    
    int r1 = INF, r2 = -INF, c1 = INF, c2 = -INF;
    for (int i = 0; i < n; i++){
        r1 = min(r1, min_row[i]);
        r2 = max(r2, max_row[i]);
        c1 = min(c1, min_col[i]);
        c2 = max(c2, max_col[i]);
        if (r1 <= r2 && c1 <= c2){
            int ans = (r2 - r1 + 1) * (c2 - c1 + 1);
            cout << ans << "\n";
        } else {
            cout << 0 << "\n";
        }
    }
}

int32_t main() 
{
    auto begin = std::chrono::high_resolution_clock::now();
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int t = 1;
    // freopen("in",  "r", stdin);
    // freopen("out", "w", stdout);
    
//    cin >> t;
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
