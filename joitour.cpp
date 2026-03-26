#include <bits/stdc++.h>
using namespace std;
#define int long long
#define INF (int)1e18

mt19937_64 RNG(chrono::steady_clock::now().time_since_epoch().count());

void Solve() 
{
    int n; cin >> n; 
    
    vector <int> aa(n + 1);
    for (int i = 1; i <= n; i++){
        cin >> aa[i];
    }
    
    vector<vector<int>> adj(n + 1);
    for (int i = 1; i < n; i++){
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    vector <pair<int, int>> a;
    a.push_back({0, 0});
    vector <int> tin(n + 1), tout(n + 1), dep(n + 1);
    int timer = 0;
    vector<vector<int>> lift(n + 1, vector<int>(20, 0));
    for (int i = 0; i < 20; i++){
        lift[1][i] = 1;
    }
    
    vector <int> ord = {0};
    
    auto dfs = [&](auto self, int u, int par) -> void{
        lift[u][0] = par;
        tin[u] = ++timer;
        ord.push_back(u);
        a.push_back({aa[u], 1});
        for (int v : adj[u]) if (v != par){
            dep[v] = dep[u] + 1;
            self(self, v, u);
        }
        tout[u] = ++timer;
        a.push_back({aa[u], -1});
        ord.push_back(u);
    };
    dfs(dfs, 1, 1);
    
    for (int j = 1; j < 20; j++){
        for (int i = 1; i <= n; i++){
            lift[i][j] = lift[lift[i][j - 1]][j - 1];
        }
    }
    
    auto inc_dep = [&](int u, int k){
        for (int i = 0; i < 20; i++) if (k >> i & 1) u = lift[u][i];
        return u;
    };  
    
    auto get_lca = [&](int u, int v){
        if (dep[u] > dep[v]) swap(u, v);
        v = inc_dep(v, dep[v] - dep[u]);
        if (u == v) return u;
        for (int i = 19; i >= 0; i--){
            if (lift[u][i] != lift[v][i]){
                u = lift[u][i];
                v = lift[v][i];
            }
        }
        return lift[u][0];
    };
    
    const int B = 500;
    int m; cin >> m;
    vector <int> s(m), t(m);
    vector<vector<array<int, 3>>> ev(m);
    for (int i = 0; i < m; i++){
        cin >> s[i] >> t[i];
        if (tin[s[i]] > tin[t[i]]) swap(s[i], t[i]);
        
        int lc = get_lca(s[i], t[i]);
        if (lc != s[i]){
            ev[i].push_back({tin[lc], tin[lc], 1});
            int s1 = inc_dep(s[i], dep[s[i]] - dep[lc] - 1);
            int t1 = inc_dep(t[i], dep[t[i]] - dep[lc] - 1);
            
            ev[i].push_back({tout[s[i]], tout[s1], -1});
            ev[i].push_back({tin[t1], tin[t[i]], +1});
        } else {
            ev[i].push_back({tin[s[i]], tin[t[i]], 1});
        }
    }
    
    int q; cin >> q;
    vector <int> b(q);
    for (auto &x : b) cin >> x;
    vector <int> ans(q);
    
    vector <int> id(2 * n + 1);
    for (int i = 1; i <= 2 * n; i++){
        id[i] = (i - 1) / B + 1;
    }
    
    vector <array<int, 3>> self;
    int block_count = (2 * n + B - 1) / B;
    vector <int> self_freq(block_count + 2, 0);
    vector<array<int, 5>> events;
    
    for (int i = 0; i < m; i++){
        for (auto [l, r, v] : ev[i]){
            int bl = id[l], br = id[r];
            
            if (bl == br){
                self.push_back({l, r, v * v});
                continue;
            }
            
            int enl = bl * B;
            int enr = (br - 1) * B + 1;
            self.push_back({l, enl, v * v});
            self.push_back({enr, r, v * v});
            
            bl++;
            br--;
            if (bl <= br){
                self_freq[bl] += v * v;
                self_freq[br + 1] -= v * v;
            }
            
            events.push_back({l, enl, enr, r, v * v});
        }
        
        for (int k1 = 0; k1 < ev[i].size(); k1++){
            for (int k2 = k1 + 1; k2 < ev[i].size(); k2++){
                auto [l1, r1, v1] = ev[i][k1];
                auto [l2, r2, v2] = ev[i][k2];
                
                int bl1 = id[l1], br1 = id[r1];
                int bl2 = id[l2], br2 = id[r2];
                
                int enl1 = bl1 * B;
                int enr1 = (br1 - 1) * B + 1;
                int enl2 = bl2 * B;
                int enr2 = (br2 - 1) * B + 1;
                
                vector <pair<int, int>> e1, e2;
                if (bl1 == br1) e1.push_back({l1, r1});
                else {
                    e1.push_back({l1, enl1});
                    e1.push_back({enr1, r1});
                }
                
                if (bl2 == br2) e2.push_back({l2, r2});
                else {
                    e2.push_back({l2, enl2});
                    e2.push_back({enr2, r2});
                }
                
                for (auto [x1, y1] : e1){
                    for (auto [x2, y2] : e2){
                        events.push_back({x1, y1, x2, y2, v1 * v2});
                    }
                }
            } 
        }
    }
    
    for (int i = 1; i <= block_count; i++){
        self_freq[i] += self_freq[i - 1];
    }
    
    for (int i = 1; i <= block_count; i++){
        int st = (i - 1) * B + 1;
        int en = min(2 * n, i * B);
        self.push_back({st, en, self_freq[i]});
    }
    
    {
        for (int i = 1; i <= block_count; i++){
            vector <int> f(2 * n + 2, 0);
            
            auto add = [&](int l, int r, int v){
                f[l] += v;
                f[r + 1] -= v;
            };  
            
            for (int j = 0; j < m; j++){
                for (auto [l, r, v] : ev[j]){
                    int bl = id[l], br = id[r];
                    
                    if (bl == br){
                        continue;
                    }
                    
                    int enl = bl * B;
                    int enr = (br - 1) * B + 1;
                    
                    bl++; br--;
                    if (bl <= i && i <= br){
                        int st = (i - 1) * B + 1;
                        add(l, st - 1, v * v);
                        add(enr, r, v * v);
                    }
                }
                
                for (int k1 = 0; k1 < ev[j].size(); k1++){
                    auto [l, r, v] = ev[j][k1];
                    int bl = id[l];
                    int br = id[r];
                    
                    if (bl < i && i < br){
                        for (int k2 = k1 + 1; k2 < ev[j].size(); k2++){
                            auto [l2, r2, v2] = ev[j][k2];
                            add(l2, r2, v * v2);
                        }
                        
                        for (int k2 = 0; k2 < k1; k2++){
                            auto [l2, r2, v2] = ev[j][k2];
                            int bl2 = id[l2];
                            int br2 = id[r2];
                            int enl2 = bl2 * B;
                            int enr2 = (br2 - 1) * B + 1;
                            
                            if (bl2 == br2) add(l2, r2, v * v2);
                            else add(l2, enl2, v * v2), add(enr2, r2, v * v2);
                        }
                    }
                }
            }
            
            for (int j = 1; j <= 2 * n; j++){
                f[j] += f[j - 1];
            }
            
            vector <int> ff(n + 1, 0);
            for (int i = 1; i <= 2 * n; i++){
                ff[a[i].first] += f[i] * a[i].second;
            }
            
            int st = (i - 1) * B + 1;
            int en = min(i * B, 2 * n);
            for (int j = st; j <= en; j++){
                for (int k = 0; k < q; k++){
                    int left = b[k] - a[j].first;
                    if (1 <= left && left <= n){
                        ans[k] += ff[left] * a[j].second;
                    }
                }
            }
        }
    }
    
    {
        vector<vector<array<int, 3>>> tos(2 * n + 1), toe(2 * n + 1);
        for (auto [l, r, v] : self) if (abs(v) > 0){
            tos[l].push_back({l, r, v});
            toe[r].push_back({l, r, v});
        }
        
        vector <int> ff(n + 1, 0);
        int curr = 0;
        for (int i = 1; i <= 2 * n; i++){
            for (int j = 0; j < q; j++){
                int left = b[j] - a[i].first;
                if (1 <= left && left <= n){
                    ans[j] += ff[left] * a[i].second;
                }
            }
            
            for (auto [l, r, v] : tos[i]){
                curr += v;
            }
            
            ff[a[i].first] += curr * a[i].second;
            
            for (auto [l, r, v] : toe[i]){
                curr -= v;
                for (int j = l; j <= r; j++){
                    ff[a[j].first] -= v * a[j].second;
                }
            }
        }
    }
    
    {
        vector<vector<array<int, 3>>> tos(2 * n + 1), toe(2 * n + 1);
        for (auto [l1, r1, l2, r2, v] : events) if (v != 0){
            assert(l1 <= r1);
            assert(l2 <= r2);
            assert(r1 < l2);
            
            tos[l1].push_back({l2, r2, v});
            toe[r1].push_back({l2, r2, v});
        }
        
        vector <int> ff(n + 1);
        for (int i = 1; i <= 2 * n; i++){
            for (auto [l, r, v] : tos[i]){
                for (int j = l; j <= r; j++){
                    ff[a[j].first] += v * a[j].second;
                }
            }
            
            for (int j = 0; j < q; j++){
                int left = b[j] - a[i].first;
                if (1 <= left && left <= n){
                    ans[j] += ff[left] * a[i].second;
                }
            }
            
            for (auto [l, r, v] : toe[i]){
                for (int j = l; j <= r; j++){
                    ff[a[j].first] -= v * a[j].second;
                }
            }
        }
    }
    
    {
        vector <int> to_add(2 * n + 1, 0);
        vector<vector<int>> ok(2 * n + 1);
        for (int i = 0; i < m; i++){
            for (auto [l, r, v] : ev[i]){
                ok[l].push_back(r);
            }
        }
        
        vector <int> fen(2 * n + 1);
        
        auto upd = [&](int x, int v){
            for (int i = x; i <= 2 * n; i += i & (-i)){
                fen[i] += v;
            }  
        };
        
        auto query = [&](int x){
            int ans = 0;
            for (int i = x; i; i -= i & (-i)){
                ans += fen[i];
            }
            return ans;
        };  
        
        for (int i = 1; i <= 2 * n; i++){
            for (auto r : ok[i]){
                upd(r, 1);
            }
            
            int vv = ord[i];
            if (tin[vv] == i){
                int tt = tout[vv];
                int cnt = query(2 * n) - query(tt - 1);
                to_add[2 * aa[vv]] += cnt;
            }
        }
        
        for (int i = 0; i < q; i++){
            ans[i] += to_add[b[i]];
        }
    }
    
    for (auto x : ans){
        cout << x << "\n";
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
