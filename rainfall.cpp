#include <bits/stdc++.h>
using namespace std;
#define int long long
#define INF (int)1e18
#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")

mt19937_64 RNG(chrono::steady_clock::now().time_since_epoch().count());

void Solve() 
{
    // x >= X[i], y >= Y[i], x + y <= Z[i] 
    // at x = X[i], add it to set (Y[i], Z[i])
    // maintain monotonic set, when Y increases, Z must increase also 
    // for each event, maintain when it was added, and then when removing it, calculate value 
    
    int l, n, k; cin >> l >> n >> k;
    
    vector <array<int, 3>> a(n);
    for (auto &[x, y, z] : a){
        cin >> x >> y >> z;
        z = x + y + z - 1;
    }
    
    int KLIM = k;
    
    auto solve = [&](vector <array<int, 3>> a){
        vector <int> ans(6, 0);
        
        set <array<int, 3>> S[6]; // {y, z, x at which added} 
        // at each y, at most one Z 
        for (int k = 1; k <= KLIM; k++){
            S[k].insert({-1, -1, INF});
            S[k].insert({INF, INF, INF});
        }
        
        sort(a.begin(), a.end());
        
        auto add2 = [&](int y1, int y2, int x1, int x2, int z, int k){
            x2 = min(x2, z);
            y2 = min(y2, z);
            if (x1 <= x2 && y1 <= y2 && x1 + y1 <= z){
                if (x1 + y2 <= z){
                    // x1 fully covers 
                    // some more rows also probably
                    int r = min(x2, z - y2); // upto row r, fully covers 
                    ans[k] += (r - x1 + 1) * (y2 - y1 + 1);
                    x1 = r + 1;
                }
                
                if (x1 <= x2 && x1 + y1 <= z){
                    int mxy = z - x1; 
                    int cnt = mxy - y1 + 1;
                    int num = (x2 - x1 + 1);
                    num = min(num, cnt);
                    // cnt + cnt - 1 + ... + cnt - (num - 1) 
                    ans[k] += cnt * num;
                    ans[k] -= num * (num - 1) / 2;
                }
            }  
        };
        
        auto add = [&](int y1, int y2, int x1, int x2, int z, int k){
            add2(y1, y2, x1, x2, z, k);
            add2(y1, y2, x1, x2, z - 1, k);
        };
        
        auto process = [&](auto self, int x, int yl, int yr, int z, int base_k) -> void{
            for (int k = base_k; k <= KLIM; k++){
                if (yl > yr) break;
                auto id = S[k].upper_bound({yl, INF, INF});
                --id;
                
                if ((*id)[1] >= z){
                    continue;
                }
                
                // this element loses control over some y-s 
                auto pi = *id;
                ++id;
                auto nx = *id;
                
                int up = min(nx[0] - 1, yr);
                add(yl, up, pi[2], x - 1, pi[1], k);
                self(self, x, yl, up, pi[1], k + 1);
                
                if (pi[0] == yl){
                    S[k].erase(pi);
                }
                
                S[k].insert({yl, z, x});
                
                while (true){
                    id = S[k].upper_bound({yl, INF, INF});
                    if ((*id)[0] > yr) break;
                    if ((*id)[1] <= z){
                        auto id2 = id; ++id2;
                        int en = (*id2)[0] - 1;
                        if (en <= yr){
                            add((*id)[0], (*id2)[0] - 1, (*id)[2], x - 1, (*id)[1], k);
                            self(self, x, (*id)[0], (*id2)[0] - 1,(*id)[1], k + 1);
                            S[k].erase(id);
                        } else {
                            add((*id)[0], yr, (*id)[2], x - 1, (*id)[1], k);
                            self(self, x, (*id)[0], yr,(*id)[1], k + 1);
                            auto pi = *id;
                            S[k].erase(id);
                            S[k].insert({yr + 1, pi[1], pi[2]});
                        }
                    } else {
                        break;
                    }
                }
                
                auto era = [&](int yl, int z, int x){
                    auto it = S[k].upper_bound({yl, INF, INF});
                    if ((*it)[1] == z){
                        // S[k].erase(it);
                        auto it2 = it;
                        it2++;

                        add((*it)[0], (*it2)[0] - 1, (*it)[2], x - 1, (*it)[1], k);
                        S[k].erase(it);
                    }
                };

                era(yl, z, x);

                // update y 
                id = S[k].upper_bound({yl, INF, INF});
                yl = min(l, (*id)[0]);
            }
        };
        
        for (auto [x, y, z] : a){
            // should it be added at all? 
            
            // you start at y, but then you might progress if you end up taking a segment 
            process(process, x, y, l, z, 1);
        }
        
        for (int k = 1; k <= KLIM; k++){
            auto it = --S[k].end();
            int prev = -1;
            while (it != S[k].begin()){
                prev = (*it)[0];
                --it;
                add((*it)[0], prev - 1, (*it)[2], l, (*it)[1], k);
            }
        }
        
        return ans;
    };
    
    auto ans1 = solve(a);
    // for (auto &[x, y, z] : a){
    //     z -= 1;
    // }
    // auto ans2 = solve(a);
    
    for (int i = 1; i <= k; i++){
        cout << (ans1[i]) << "\n";
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
    
  //  cin >> t;
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
