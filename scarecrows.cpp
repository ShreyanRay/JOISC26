#include <bits/stdc++.h>
using namespace std;
#define int long long
#define INF (int)1e18

mt19937_64 RNG(chrono::steady_clock::now().time_since_epoch().count());

void Solve() 
{
    int n, k; cin >> n >> k;
    
    vector <array<int, 3>> xs, ys;
    for (int i = 0; i < n; i++){
        int t, x, y, c; cin >> t >> x >> y >> c;
        
        if (t == 1 || t == 2){
            xs.push_back({x, 2 - t, c});
        } else {
            ys.push_back({y, 4 - t, c});
        }
    }
    
    auto solve = [&](vector <array<int, 3>> a, int L){
        priority_queue <pair<int, int>> pq;
        int pairs = 0;
        int ans = 0;
        for (auto [x, t, c] : a){
            if (t == 0){
                pq.push({-(c - L), 0});
            } else {
                if (!pq.empty() && (-pq.top().first) + c < 0){
                    ans += c + (-pq.top().first);
                    if (pq.top().second == 0) pairs++;
                    pq.pop();
                    pq.push({c, 1});
                }
            }
        }
        return make_pair(ans, pairs);
    };
    
    sort(xs.begin(), xs.end());
    sort(ys.begin(), ys.end());
    
    // need to take well bracketed sequence 
    // 0 is open bracket 
    // 1 is close bracket 
    
    auto get_max = [&](vector <array<int, 3>> a){
        int cur = 0;
        int bal = 0;
        for (auto [x, t, c]: a){
            if (t == 0){
                cur += 1;
                bal += 1;
            } else {
                if (bal > 0){
                    bal -= 1;
                    cur += 1;
                }
            }
        }
        
        cur -= bal;
        return (cur / 2);
    };
    
    if (get_max(xs) + get_max(ys) < k){
        cout << -1 << "\n";
        return;
    }
    
    int lo = 0, hi = 2e9;
    while (lo != hi){
        int mid = (lo + hi + 1) / 2;
        
        auto [s1, p1] = solve(xs, mid);
        auto [s2, p2] = solve(ys, mid);
        
        if (p1 + p2 <= k){
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    
    auto [s1, p1] = solve(xs, lo);
    auto [s2, p2] = solve(ys, lo);
    
    int ans = s1 + s2;
    ans += lo * k;
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
