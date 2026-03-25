#include "multi.h"
#include<bits/stdc++.h>
using namespace std;

struct ufds{
    vector <int> root, sz;
    int n;
 
    void init(int nn){
        n = nn;
        root.resize(n + 1);
        sz.resize(n + 1, 1);
        for (int i = 1; i <= n; i++) root[i] = i;
    }
 
    int find(int x){
        if (root[x] == x) return x;
        return root[x] = find(root[x]);
    }
 
    bool unite(int x, int y){
        x = find(x); y = find(y);
        if (x == y) return false;
 
        if (y > x) swap(x, y);
        sz[x] += sz[y];
        root[y] = x;
        return true;
    }
};

std::vector<unsigned long long> strategy(int N, int r, int i,
                                         std::vector<unsigned long long> A,
                                         std::vector<unsigned long long> B) {
    int n = N;
    #define ull unsigned long long 

    if (r == 0){
        vector <pair<ull, int>> vec;
        for (int j = 0; j < n; j++) if (j != i){   
            vec.push_back({A[j], j});
        }
        sort(vec.begin(), vec.end());
        vec.push_back({0, i});

        ull val = vec[0].second + vec[1].second * (1ULL << 8) + vec[1].first * (1ULL << 16);
        vector <ull> ans(n, val);
        return ans;
    }

    ufds uf;
    uf.init(n);

    auto make = [&](vector <pair<int, int>> edges){
        vector <bool> vis(n, false);
        vector <int> pp(n, -1);
        vector<vector<int>> adj(n);
        for (auto [u, v] : edges){
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        auto dfs = [&](auto self, int u, int par) -> void{
            vis[u] = true;
            pp[u] = par;
            for (int v : adj[u]){
                if (v != par){
                    self(self, v, u);
                }
            }
        };

        for (int i = 0; i < n; i++) if (!vis[i]){
            dfs(dfs, i, i);
        }
        return pp;
    };

    if (r == 1){
        vector <pair<int, int>> edges;
        vector <pair<ull, pair<int, int>>> add;
        ull mst = 0;
        for (int j = 0; j < n; j++){
            ull val = B[j];
            ull v1 = val % (1ULL << 8);
            ull v2 = (val / (1ULL << 8)) % (1ULL << 8);
            ull w2 = val / (1ULL << 16);

            if (uf.unite(j, v1))
            edges.push_back({j, v1});

            // add.push_back(make_pair(w2, make_pair(v2, j)));
        }

        vector<vector<int>> at(n);
        for (int j = 0; j < n; j++) at[uf.find(j)].push_back(j);

        for (int j = 0; j < n; j++) if (at[j].size()){
            vector <pair<ull, pair<int, int>>> here;
            for (auto x : at[j]){
                ull val = B[x];
                ull v2 = (val / (1ULL << 8)) % (1ULL << 8);
                ull w2 = val / (1ULL << 16);

                here.push_back(make_pair(w2, make_pair(v2, x)));
            }

            sort(here.begin(), here.end());
            add.push_back(here[0]);
        }

        sort(add.begin(), add.end());
        for (auto pi : add){
            ull w = pi.first;
            int u = pi.second.first;
            int v = pi.second.second;
            if (uf.unite(u, v)){
                edges.push_back({u, v});
                mst += w;
            }
        }

        auto par = make(edges);
        vector <pair<ull, int>> vec;
        for (int j = 0; j < n; j++) if (uf.find(j) != uf.find(i)){
            vec.push_back({A[j], j});
        }
        sort(vec.begin(), vec.end());
        vec.push_back({0, i});

        ull val = par[i] + vec[0].second * (1ULL << 8) + vec[0].first * (1ULL << 16);
        vector <ull> ans(n, val);
        ans[i] = par[i] + mst * (1ULL << 8);
        return ans;
    }

    if (r == 2){
        ull mst = B[i] / (1ULL << 8);
        vector <pair<int, int>> edges;
        for (int j = 0; j < n; j++){
            int pa = B[j] % (1ULL << 8);
            if (pa != j){
                edges.push_back({pa, j});
                uf.unite(pa, j);
            }
        }
        vector <pair<ull, pair<int, int>>> add;
        vector <pair<ull, pair<int, int>>> a2(n);
        for (int j = 0; j < n; j++) if (j != i){
            int v = (B[j] / (1ULL << 8)) % (1ULL << 8);
            ull w = (B[j] / (1ULL << 16));
            // add.push_back(make_pair(w, make_pair(v, j)));
            a2[j] = make_pair(w, make_pair(v, j));
        }

        vector <pair<ull, int>> vec;
        for (int j = 0; j < n; j++) if (uf.find(j) != uf.find(i)){ 
            vec.push_back({A[j], j});
        }
        sort(vec.begin(), vec.end());
        vec.push_back({0, i});

        // add.push_back(make_pair(vec[0].first, make_pair(vec[0].second, i)));
        a2[i] = make_pair(vec[0].first, make_pair(vec[0].second, i));

        vector<vector<int>> at(n);
        for (int it = 0; it < n; it++) at[uf.find(it)].push_back(it);
        for (int it = 0; it < n; it++) if (at[it].size()){
            pair <ull, pair<int, int>> best = make_pair(1ULL << 48, make_pair(-1, -1));
            for (int x : at[it]){
                if (uf.find(a2[x].second.first) != uf.find(x)){
                    best = min(best, a2[x]);
                }
            }
            if (best.second.first != -1){
                add.push_back(best);
            }
        }
        sort(add.begin(), add.end());

        for (auto pi : add){
            ull w = pi.first;
            ull u = pi.second.first;
            ull v = pi.second.second;
            if (uf.unite(u, v)){
                mst += w;
                edges.push_back({u, v});
            }
        }

        auto par = make(edges);
        vec.clear();
        for (int j = 0; j < n; j++) if (uf.find(j) != uf.find(i)){
            vec.push_back({A[j], j});
        }
        sort(vec.begin(), vec.end());
        vec.push_back({0, i});

        ull val = par[i] + vec[0].second * (1ULL << 8) + vec[0].first * (1ULL << 16);
        vector <ull> ans(n, val);
        ans[i] = par[i] + mst * (1ULL << 8);
        return ans;
    }

    if (r == 3){
        ull mst = B[i] / (1ULL << 8);
        vector <pair<int, int>> edges;
        for (int j = 0; j < n; j++){
            int pa = B[j] % (1ULL << 8);
            if (pa != j){
                edges.push_back({pa, j});
                uf.unite(pa, j);
            }
        }
        vector <pair<ull, pair<int, int>>> add;
        vector <pair<ull, pair<int, int>>> a2(n);
        for (int j = 0; j < n; j++) if (j != i){
            int v = (B[j] / (1ULL << 8)) % (1ULL << 8);
            ull w = (B[j] / (1ULL << 16));
            // add.push_back(make_pair(w, make_pair(v, j)));
            a2[j] = make_pair(w, make_pair(v, j));
        }

        vector <pair<ull, int>> vec;
        for (int j = 0; j < n; j++) if (uf.find(j) != uf.find(i)){ 
            vec.push_back({A[j], j});
        }
        sort(vec.begin(), vec.end());
        vec.push_back({0, i});

        // add.push_back(make_pair(vec[0].first, make_pair(vec[0].second, i)));
        a2[i] = make_pair(vec[0].first, make_pair(vec[0].second, i));

        vector<vector<int>> at(n);
        for (int it = 0; it < n; it++) at[uf.find(it)].push_back(it);
        for (int it = 0; it < n; it++) if (at[it].size()){
            pair <ull, pair<int, int>> best = make_pair(1ULL << 48, make_pair(-1, -1));
            for (int x : at[it]){
                if (uf.find(a2[x].second.first) != uf.find(x)){
                    best = min(best, a2[x]);
                }
            }
            if (best.second.first != -1){
                add.push_back(best);
            }
        }
        sort(add.begin(), add.end());

        for (auto pi : add){
            ull w = pi.first;
            ull u = pi.second.first;
            ull v = pi.second.second;
            if (uf.unite(u, v)){
                mst += w;
                edges.push_back({u, v});
            }
        }

        auto par = make(edges);

        int comps = 0;
        vector <int> id(n);
        for (int j = 0; j < n; j++) if (uf.find(j) == j){
            id[j] = comps++;
        }

        for (int j = 0; j < n; j++) if (uf.find(j) != j){
            id[j] = id[uf.find(j)];
        }

        int total_need = comps * (comps - 1) / 2 + 1;
        assert(total_need <= n);

        vector <ull> mn(comps, 1ULL << 48);
        for (int j = 0; j < n; j++){
            mn[id[j]] = min(mn[id[j]], A[j]);
        }

        vector <ull> ans(n, par[i]);
        if (i == 0){
            ans[0] += mst * (1ULL << 8);
        } else {
            ans[0] += A[par[i]] * (1ULL << 8);
        }

        int ptr = 1;
        for (int j = 0; j < comps; j++){
            for (int k = j + 1; k < comps; k++){
                if (id[i] == j){
                    ans[ptr] += mn[k] * (1ULL << 8);
                }
                if (id[i] == k){
                    ans[ptr] += mn[j] * (1ULL << 8);
                }
                ptr++;
            }
        }

        return ans;
    }

    if (r == 4){
        for (int j = 0; j < n; j++){
            int pa = B[j] % (1ULL << 8);
            if (pa != j){
                uf.unite(pa, j);
            }
        }

        int comps = 0;
        vector <int> id(n);
        for (int j = 0; j < n; j++) if (uf.find(j) == j){
            id[j] = comps++;
        }

        for (int j = 0; j < n; j++) if (uf.find(j) != j){
            id[j] = id[uf.find(j)];
        }

        int total_need = comps * (comps - 1) / 2;
        int pa = B[i] % (1ULL << 8);
        if (i == 0){
            ull mst = 0;
            for (int j = 1; j < n; j++){
                mst += B[j] / (1ULL << 8);
            }
            vector <ull> ans(n, mst);
            return ans;
        } else if (i <= total_need){
            int ptr = 1;
            int nj, nk;
            for (int j = 0; j < comps; j++){
                for (int k = j + 1; k < comps; k++){
                    if (ptr == i){
                        nj = j, nk = k;
                    } 
                    ptr++;
                }
            }

            ull best = (1ULL << 48);
            for (int j = 0; j < n; j++){
                if (id[j] == nj){
                    best = min(best, B[j] / (1ULL << 8));
                }
            }

            vector <ull> ans(n, best);
            return ans;
        } else {
            vector <ull> ans(n, (1ULL << 48));
            return ans;
        }
    }

    if (r == 5){
        ull mst = B[0];
        vector <pair<ull, pair<int, int>>> vec;
        int ptr = 1;
        while (ptr < n && B[ptr] != (1ULL << 48)){
            ptr++;
        }

        ptr--;
        int comps = 1;
        while (comps * (comps - 1) / 2 != ptr){
            comps++;
        }

        int q = 1;
        for (int j = 0; j < comps; j++){
            for (int k = j + 1; k < comps; k++){
                vec.push_back(make_pair(B[q], make_pair(j, k)));
                q++;
            }
        }

        ufds uf;
        uf.init(comps);

        sort(vec.begin(), vec.end());
        for (auto pi : vec){
            ull w = pi.first;
            int u = pi.second.first;
            int v = pi.second.second;

            if (uf.unite(u, v)){
                mst += w;
            }
        }
        return {mst};
    }
    return {0};
}
