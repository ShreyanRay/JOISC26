#include <bits/stdc++.h>
using namespace std;
#define int long long
#define INF (int)1e18

mt19937_64 RNG(chrono::steady_clock::now().time_since_epoch().count());

void Solve() 
{
    int n, lim; cin >> n >> lim;
    
    vector <int> a(n + 1);
    for (int i = 1; i <= n; i++){
        cin >> a[i];
    }
    
    vector<vector<int>> adj(n + 1);
    for (int i = 1; i < n; i++){
        int u, v; cin >> u >> v;
        
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    vector <int> ans(n + 1);
    
    struct fenwick{
        vector <int> fen;
        int mx;
        
        void upd(int x, int v){
            x = min(x, mx);
            if (x == 0) return;
            for (int i = x; i <= mx; i += i & (-i)){
                fen[i] += v;
            }  
        }
        
        int query(int x){
            int ans = 0;
            for (int i = x; i; i -= i & (-i)){
                ans += fen[i];
            }
            return ans;
        }
        
        void reset(int _mx){
            mx = _mx;
            for (int i = 1; i <= mx; i++){
                fen[i] = 0;
            }
        }
        
        void init(int n){
            fen.resize(n + 1);
        }
    };
    
    fenwick f1, f2;
    f1.init(n + 1);
    f2.init(n + 1);
    
    vector <int> sub(n + 1);
    vector <int> comp;
    vector <bool> alive(n + 1, true);
    vector <int> dep(n + 1);
    vector <int> A(n + 1), B(n + 1);
    
    auto dfs = [&](auto self, int u, int par) -> void{
        sub[u] = 1;
        comp.push_back(u);
        for (int v : adj[u]) if (v != par && alive[v]){
            self(self, v, u);
            sub[u] += sub[v];
        }
    };
    
    auto find = [&](int u){
        comp.clear();
        dfs(dfs, u, -1);
        int sz = comp.size();
        
        for (int x : comp){
            bool good = true;
            for (int y : adj[x]) if (alive[y] && sub[y] < sub[x]){
                if (sub[y] > sz / 2){
                    good = false;
                }
            }
            
            if (sz - sub[x] > sz / 2){
                good = false;
            }
            if (good){
                return x;
            }
        }
        
        assert(false);
        return -1LL;
    };
    
    auto cd = [&](auto self, int u) -> void{
        u = find(u);  
        
        // cout << "HI " << u << "\n";
        
        vector<vector<int>> groups;
        groups.push_back({u});
        
        // on path, we want minimum a[i] + dep[i], a[i] - dep[i]
        vector <int> nodes;
        
        auto dfs2 = [&](auto self, int u, int par, int curr_dep, int aplus, int aneg) -> void{
            nodes.push_back(u);
            aplus = min(aplus, a[u] + curr_dep);
            aneg = min(aneg, a[u] - curr_dep);
            dep[u] = curr_dep;
            A[u] = aplus;
            B[u] = aneg;
            
            for (int v : adj[u]) if (alive[v] && v != par){
                self(self, v, u, curr_dep + 1, aplus, aneg);
            }
        };
        
        dep[u] = 0;
        A[u] = a[u];
        B[u] = a[u];
        
        for (int x : adj[u]) if (alive[x]){
            nodes.clear();
            dfs2(dfs2, x, u, 1, a[u], a[u]);
            groups.push_back(nodes);
        }
        
        int mx = comp.size() + 1;
        
        f1.reset(mx);
        f2.reset(mx);
        
        auto upd = [&](int u){
            if (dep[u] > lim) return;
            
            {
                int lower_bound = 0;
                int upper_bound = lim - dep[u];
                upper_bound = min(upper_bound, mx - 1);
                if (lower_bound <= upper_bound){
                    f1.upd(lower_bound + 1, +1);
                    f1.upd(upper_bound + 2, -1);
                }
                
            //    cout << "HI " << u << " " << lower_bound << " " << upper_bound << "\n";
            }
            
            int upper_bound = lim - dep[u];
            int lower_bound = B[u];
            lower_bound = max(lower_bound, 0LL);
            upper_bound = min(upper_bound, mx - 1);
            
            if (lower_bound <= upper_bound){
                f2.upd(lower_bound + 1, +1);
                f2.upd(upper_bound + 2, -1);
            }
            
         //   cout << u << " " << lower_bound << " " << upper_bound << "\n";
        };
        
        auto query = [&](int u){
            if (dep[u] > lim) return;
            
       //     cout << "QUERYING " << u << " ";
            
            int val = A[u] - dep[u];
            if (val <= 0){
                // query f1
                // look at >= dep[u] + 1 
            //    cout << "GOOD TYPE " << f1.query(dep[u] + 1) << "\n";
                ans[u] += f1.query(dep[u] + 1);
            } else {
                // query f2
             //   cout << "BAD TYPE " << f2.query(dep[u] + 1) << "\n";
                ans[u] += f2.query(dep[u] + 1);
            }
        };  
        
        for (int it = 0; it < groups.size(); it++){
            // cout << it << " ";
            // for (auto x : groups[it]){
            //     cout << x << " ";
            // }
            // cout << "\n";
            for (auto x : groups[it]){
                query(x);
            }
            for (auto x : groups[it]){
                upd(x);
            }
        }
        
        f1.reset(mx);
        f2.reset(mx);
        
        for (int it = groups.size() - 1; it >= 0; it--){
            for (auto x : groups[it]){
                query(x);
            }
            for (auto x : groups[it]){
                upd(x);
            }
        }
        
        alive[u] = false;
        for (int v : adj[u]) if (alive[v]){
            self(self, v);
        }
    };
    
    cd(cd, 1);
    
    for (int i = 1; i <= n; i++){
        if (a[i] == 0){
            ans[i] += 1;
        }
    }
    
    for (int i = 1; i <= n; i++){
        cout << ans[i] << "\n\n"[i == n];
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
    
   // cin >> t;
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
