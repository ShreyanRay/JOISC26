#include <bits/stdc++.h>
using namespace std;
#define int long long
#define INF (int)1e18

mt19937_64 RNG(chrono::steady_clock::now().time_since_epoch().count());

const int N = 1.5e6 + 69;
int seg[4 * N], lz[4 * N];

void build(int l, int r, int pos){
    lz[pos] = -1;
    if (l == r){
        seg[pos] = INF;
        return;
    }
    
    int mid = (l + r) / 2;
    build(l, mid, pos * 2);
    build(mid + 1, r, pos * 2 + 1);
}

void updlz(int l, int r, int pos){
    if (lz[pos] != -1){
        seg[pos] = lz[pos];
        if (l != r){
            lz[pos * 2] = lz[pos];
            lz[pos * 2 + 1] = lz[pos];
        }
        lz[pos] = -1;
    }
}

void upd(int l, int r, int pos, int ql, int qr, int v){
    updlz(l, r, pos);
    if (l >= ql && r <= qr){
        lz[pos] = v;
        updlz(l, r, pos);
    } else if (l > qr || r < ql){
        return;
    } else {
        int mid = (l + r) / 2;
        upd(l, mid, pos * 2, ql, qr, v);
        upd(mid + 1, r, pos * 2 + 1, ql, qr, v);
        
        seg[pos] = min(seg[pos * 2], seg[pos * 2 + 1]);
    }
}

int query(int l, int r, int pos, int qp){
    updlz(l, r, pos);
    if (l == r){
        return seg[pos];
    }
    
    int mid = (l + r) / 2;
    int v = -1;
    if (qp <= mid) v = query(l, mid, pos * 2, qp);
    else v = query(mid + 1, r, pos * 2 + 1, qp);
    
    seg[pos] = min(seg[pos * 2], seg[pos * 2 + 1]);
    return v;
}

void Solve() 
{
    int n, q, k; cin >> n >> q >> k;
    
    vector <int> a(n + 1), p(n + 1);
    for (int i = 1; i <= n; i++){
        cin >> a[i];
        // a[i] = 1 + RNG() % ((int)1e9);
        if (i % 2 == 0){
            a[i] *= -1;
        }
        p[i] = p[i - 1] + a[i];
    }
    
    vector <int> nx(n + 2);
    vector<vector<pair<int, int>>> affect(n + 1);
    
    auto omk = [&](int x, int k){
        int y = x % k;
        y += k;
        y %= k;
        return y;
    };
    
    for (int i = n; i >= 1; i--){
        if (a[i] < 0){
            int v = abs(a[i]);
            if (v >= k){
                affect[i].push_back(make_pair(0, k - 1));
            } else {
                int A = omk(p[i - 1], k);
                int B = v - 1;
                
                if (A - B < 0){
                    affect[i].push_back({0, A});
                    affect[i].push_back({A - B + k, k - 1});
                } else {
                    affect[i].push_back({A - B, A});
                }
            }
        }
    }
    
    vector <int> S;
    for (int i = 1; i <= n; i++){
        for (auto [l, r] : affect[i]){
            S.push_back(l);
            S.push_back(r);
        }
    } 
    
    for (int i = 0; i <= n; i++){
        S.push_back(omk(p[i], k));
    }
    
    sort(S.begin(), S.end());
    S.erase(unique(S.begin(), S.end()), S.end());
    
    auto mp = [&](int x){
        return upper_bound(S.begin(), S.end(), x) - S.begin();  
    };
    
    int m = S.size();
    assert(m < N);
    build(1, m, 1);
    
    for (int i = n; i >= 1; i--){
        for (auto [l, r] : affect[i]){
            l = mp(l);
            r = mp(r);
            upd(1, m, 1, l, r, i);
        }
        
        int val = query(1, m, 1, mp(omk(p[i - 1], k)));
        nx[i - 1] = min(n + 1, val);
    }
    nx[n] = nx[n + 1] = n + 1;
    
    auto get = [&](int l, int r){
        int ans = p[r] - p[l - 1];
        return (ans / k);
    };
    
    vector<vector<int>> jump(20, vector<int>(n + 2));
    vector<vector<int>> f(20, vector<int>(n + 2));
    for (int i = 0; i <= n + 1; i++){
        jump[0][i] = nx[i];
        f[0][i] = get(i + 1, nx[i] - 1);
    }
    
    for (int j = 1; j < 20; j++){
        for (int i = 0; i <= n + 1; i++){
            jump[j][i] = jump[j - 1][jump[j - 1][i]];
            f[j][i] = f[j - 1][i] + f[j - 1][jump[j - 1][i]];
        }
    }
    
    while (q--){
        int l, r; cin >> l >> r;
        l--;
        
        int ans = 0;
        
        for (int i = 19; i >= 0; i--){
            if (jump[i][l] <= r){
                ans += f[i][l];
                l = jump[i][l];
            }
        }
        
        if (l < r) ans += get(l + 1, r);
        
        cout << ans << "\n";
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
