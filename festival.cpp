#include <bits/stdc++.h>
using namespace std;
#include <ext/pb_ds/assoc_container.hpp>
using namespace __gnu_pbds;

#define int long long
#define INF (int)1e18

template<class T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

mt19937_64 RNG(chrono::steady_clock::now().time_since_epoch().count());

struct segtree{
    struct node{
        int cl, cr, dw;
    };  
    
    // const int N = 1.5e5 + 69;
    // node seg[4 * N];
    // node def;
    vector <node> seg;
    node def;
    bool dir = false;
    
    node merge(node x, node y){
        // (x + dw) clamped to [cl, cr] 
        if (!dir){
            node z;
            z.dw = x.dw + y.dw;
            z.cl = max(x.cl, min(x.cr, y.cl + x.dw));
            z.cr = max(x.cl, min(x.cr, y.cr + x.dw));
            return z;
        } else {
            node z;
            z.dw = x.dw + y.dw;
            z.cl = max(y.cl, min(y.cr, x.cl + x.dw)) - x.dw;
            z.cr = max(y.cl, min(y.cr, x.cr + x.dw)) - x.dw;
            return z;
        }
    }
    
    void build(int l, int r, int pos, vector <int> &cl, vector <int> &cr, vector <int> &dw){
        if (l == r){
            seg[pos].cl = cl[l];
            seg[pos].cr = cr[l];
            seg[pos].dw = dw[l];
            return;
        }
        
        int mid = (l + r) / 2;
        build(l, mid, pos * 2, cl, cr, dw);
        build(mid + 1, r, pos * 2 + 1, cl, cr, dw);
        
        seg[pos] = merge(seg[pos * 2], seg[pos * 2 + 1]);
    }
    
    void upd_seg(int l, int r, int pos, int qp, int cl, int cr, int dw){
        if (l == r){
            seg[pos].cl = cl;
            seg[pos].cr = cr;
            seg[pos].dw = dw;
            return;
        }
        
        int mid = (l + r) / 2;
        if (qp <= mid) upd_seg(l, mid, pos * 2, qp, cl, cr, dw);
        else upd_seg(mid + 1, r, pos * 2 + 1, qp, cl, cr, dw);
        
        seg[pos] = merge(seg[pos * 2], seg[pos * 2 + 1]);
    }
    
    node query(int l, int r, int pos, int ql, int qr){
        if (l >= ql && r <= qr){
            return seg[pos];
        } else if (l > qr || r < ql){
            return def;
        } else {
            int mid = (l + r) / 2;
            node v1 = query(l, mid, pos * 2, ql, qr);
            node v2 = query(mid + 1, r, pos * 2 + 1, ql, qr);
            return merge(v1, v2);
        }
    }
    
    void init(int n){
        seg.resize(4 * n + 1);
    }
};

segtree s1, s2;

void Solve() 
{
    int n; cin >> n;
    s1.init(n);
    s2.init(n);
    s1.def.dw = s2.def.dw = 0;
    s1.def.cl = s2.def.cl = -INF, s1.def.cr = s2.def.cr = INF;
    s2.dir = true;
    
    vector <array<int, 3>> e;
    for (int i = 1; i < n; i++){
        int u, v, w; cin >> u >> v >> w;
        e.push_back({u, v, w});
    }
    
    vector <int> c(n + 1);
    for (int i = 1; i <= n; i++){
        cin >> c[i];
    }
    
    int q; cin >> q;
    
    // let A be (K - 1)th among light nodes 
    // let B be K-th among light nodes 
    // answer = (x + w) clamped to [A, B] 
    
    vector<vector<pair<int, int>>> g(n + 1);
    for (auto [u, v, w] : e){
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }
    
    vector <int> tin(n + 1), tout(n + 1), sub(n + 1), heavy(n + 1), top(n + 1), bot(n + 1), dp(n + 1), who(n + 1), pp(n + 1), dw(n + 1), pw(n + 1);
    vector <int> ii(n - 1);
    
    auto dfs = [&](auto self, int u, int par) -> void{
        sub[u] = 1;
        vector <int> vec = {0};
        for (auto [v, w] : g[u]) if (v != par){
            self(self, v, u);
            sub[u] += sub[v];
            vec.push_back(dp[v] + w);
        }
        
        sort(vec.begin(), vec.end());
        if (c[u] < vec.size()){
            dp[u] = vec[c[u]];
        } else {
            dp[u] = INF;
        }
    };
    dfs(dfs, 1, -1);
    
    int timer = 0;
    
    struct data_struct{
        ordered_set <pair<int, int>> os;
        int tt = 0;
        
        void add(int x){
            os.insert({x, tt++});
        }
        
        void era(int x){
            auto it = os.lower_bound({x, -1});
            assert(it != os.end());
            assert(it->first == x);
            os.erase(it);
        }
        
        int find_kth(int k){
            if (k <= 0){
                return 0LL;
            }  
            if (k > os.size()){
                return INF;
            }
            
            return os.find_by_order(k - 1)->first;
        }
    };
    
    vector <data_struct> os(n + 1);
    
    auto dfs2 = [&](auto self, int u, int par, int h) -> void{
        vector <pair<int, int>> ch;
        for (auto [v, w] : g[u]) if (v != par){
            ch.push_back({v, w});
        }
        sort(ch.begin(), ch.end(), [&](pair <int, int> x, pair <int, int> y){
            return sub[x.first] > sub[y.first];
        });
        
        tin[u] = ++timer;
        who[tin[u]] = u;
        top[tin[u]] = tin[h];
        if (u != 1)
        pp[tin[u]] = tin[par];
        
        bool fir = false;
        for (auto [v, w] : ch){
            if (!fir){
                fir = true;
                self(self, v, u, h);
                bot[tin[u]] = bot[tin[v]];
                heavy[tin[u]] = tin[v];
                dw[tin[u]] = w;
            } else {
                self(self, v, u, v);
            }
            
            pw[tin[v]] = w;
            
            if (tin[v] != heavy[tin[u]]){
                os[tin[u]].add(dp[v] + w);
            }
        }
        
        tout[u] = timer;
        if (tin[u] == tout[u]) bot[tin[u]] = tin[u];
    };
    dfs2(dfs2, 1, -1, 1);
    
    {
        vector <int> cc(n + 1);
        for (int i = 1; i <= n; i++){
            cc[tin[i]] = c[i];
        }
        c = cc;
    }
    
    {
        vector <int> ndp(n + 1);
        for (int i = 1; i <= n; i++){
            ndp[tin[i]] = dp[i];
        }
        dp = ndp;
    }
    
    int ptr1 = 0;
    for (auto [u, v, w] : e){
        if (tin[u] <= tin[v] && tout[u] >= tout[v]){
            ii[ptr1] = tin[u];
        } else {
            ii[ptr1] = tin[v];
        }
        ptr1++;
    }
    
    vector <int> cl(n + 1), cr(n + 1);
    for (int i = 1; i <= n; i++){
        cl[i] = os[i].find_kth(c[i] - 1);
        cr[i] = os[i].find_kth(c[i]);
    }
    
    // dp[i] should be computed for all points which are tops or bottoms 
    
    s1.build(1, n, 1, cl, cr, dw);
    s2.build(1, n, 1, cl, cr, dw);
    
    auto upd = [&](int v){
        // upd cl[v], cr[v] in segtree 
        // calculate top[v] by taking product of all in [top[v], bot[v]]
        while (true){
            cl[v] = os[v].find_kth(c[v] - 1);
            cr[v] = os[v].find_kth(c[v]);
            s1.upd_seg(1, n, 1, v, cl[v], cr[v], dw[v]);
            s2.upd_seg(1, n, 1, v, cl[v], cr[v], dw[v]);
            int prev = dp[top[v]];
            dp[bot[v]] = cr[bot[v]];
            
            if (top[v] != bot[v]){
                auto g = s1.query(1, n, 1, top[v], bot[v] - 1);
                
                dp[top[v]] = dp[bot[v]] + g.dw;
                dp[top[v]] = max(dp[top[v]], g.cl);
                dp[top[v]] = min(dp[top[v]], g.cr);
            }
            
            if (top[v] != 1){
                int got = pp[top[v]];
                os[got].era(prev + pw[top[v]]);
                os[got].add(dp[top[v]] + pw[top[v]]);
                v = got;
            } else {
                break;
            }
        }
    };
    
    auto get1 = [&](int v){
        if (v == bot[v]){
            return dp[v];
        }
        auto g = s1.query(1, n, 1, v, bot[v] - 1);
        int val = dp[bot[v]] + g.dw;
        val = max(val, g.cl);
        val = min(val, g.cr);
        return val;
    };
    
    auto get2 = [&](int v){
        vector <int> path;
        vector <int> ppath;
        int curr = v;
        
        while (top[curr] != 1){
            path.push_back(top[curr]);
            ppath.push_back(pp[top[curr]]);
            curr = pp[top[curr]];
        }
        reverse(path.begin(), path.end());
        reverse(ppath.begin(), ppath.end());
        
        int curr_val = INF;
        int sz = path.size();
        
        for (int i = 0; i < sz; i++){
            int v = ppath[i];
            curr_val += pw[top[v]];
            if (v != top[v]){
                auto g = s2.query(1, n, 1, top[v], v - 1);
                curr_val = max(g.cl, min(g.cr, curr_val)) + g.dw; 
            }
            
            int hc = INF;
            if (bot[v] != v){ 
                hc = get1(heavy[v]) + dw[v];
            }
            
            int other = path[i];
            os[v].era(dp[other] + pw[other]);
            os[v].add(hc);
            os[v].add(curr_val);
            
            int got = os[v].find_kth(c[v]);
            
            os[v].era(curr_val);
            os[v].era(hc);
            os[v].add(dp[other] + pw[other]);
            
            curr_val = got;
        }
        
        curr_val += pw[top[v]];
        if (v != top[v]){
            auto g = s2.query(1, n, 1, top[v], v - 1);
            curr_val = max(g.cl, min(g.cr, curr_val)) + g.dw;
        }
        
        return curr_val;
    };
    
    auto query_vertex = [&](int v){
        // need to calculate dp[heavy_child of v] 
        int hc = INF;
        if (tin[who[v]] != tout[who[v]]){
            hc = get1(heavy[v]) + dw[v];
        }
        
        int upper = INF;
        if (v != 1){
            upper = get2(v);
        }
        
        os[v].add(hc);
        os[v].add(upper);
        
        int ans = os[v].find_kth(c[v]);
        if (ans >= INF){
            ans = -1;
        }
        
        os[v].era(hc);
        os[v].era(upper);
        
        return ans;
    };
    
    while (q--){
        int t; cin >> t;
        
        if (t == 1){
            int v, x; cin >> v >> x;
            v = tin[v];
            c[v] = x;
            
            upd(v);
        } else if (t == 2){
            int ei, x; cin >> ei >> x;
            ei--;
            int v = ii[ei];
            int v1 = e[ei][0];
            int v2 = e[ei][1];
            int other = tin[v1] ^ tin[v2] ^ v; 
            if (heavy[v] == other){
                dw[v] = x;
            } else {
                os[v].era(dp[other] + pw[other]);
                pw[other] = x;
                os[v].add(dp[other] + pw[other]);
            }
            upd(v);
        } else {
            int v; cin >> v;
            v = tin[v];
            
            cout << query_vertex(v) << "\n";
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
