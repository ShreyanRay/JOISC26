#include "voltage.h"
#include <bits/stdc++.h>
using namespace std;

bool solve(int N, int M) {
    int n = N;
    int m = M;

    auto check1 = [&](vector <int> S, int v){
        // does exist edge S -> y
        // vertices not in S are guaranteed to appear before in order 
        vector <int> x(n, 0), y(n, 0);
        for (int i = 0; i < n; i++){
            if (i == v){
                x[i] = 1, y[i] = 0;
            } else if (S[i]){
                x[i] = y[i] = 1;
            } else {
                x[i] = y[i] = 0;
            }
        }

        return query(x, y) != 0;
    };

    auto check2 = [&](int v, vector <int> S){
        vector <int> x(n, 0), y(n, 0);
        for (int i = 0; i < n; i++){
            if (i == v){
                x[i] = 1, y[i] = 0;
            } else if (S[i] == 0){
                // processed 
                x[i] = y[i] = 0;
            } else if (S[i] == 1){
                // test 
                x[i] = y[i] = 0; 
            } else if (S[i] == 2){
                // keep out 
                x[i] = y[i] = 1;
            } else {
                assert(false);
            }
        }

        return query(x, y) != 0;
    };

    queue <int> q;
    vector <int> S(n, 1);
    for (int i = 0; i < n; i++){
        if (!check1(S, i)){
            q.push(i);
        }
    }

    vector <pair<int, int>> e;
    while (!q.empty()){
        int u = q.front(); q.pop();
        S[u] = 0;

        vector <int> got;

      //  cout << "HI " << u << endl;

        while (true){
            auto T = S;
            for (auto x : got){
                T[x] = 2;
            }
            if (!check2(u, T)) break; 

            int lo = 0, hi = n - 1;
            while (lo != hi){
                auto T = S;
                int mid = (lo + hi) / 2;
                for (int i = mid + 1; i < n; i++) if (S[i] == 1){
                    T[i] = 2;
                }
                for (int x : got){
                    T[x] = 2;
                }

                if (check2(u, T)){
                    hi = mid;
                } else {
                    lo = mid + 1;
                } 
            }
            e.push_back({u, lo});
          //  cout << "GOT " << lo << endl;
            got.push_back(lo);
        }

        for (int x : got){
            if (!check1(S, x)){
                q.push(x);
            }
        }
    }

    for (auto x : S){
        if (x == 1){
            return false;
        }
    }

    for (auto [x, y] : e){
        answer(x, y);
    }
    return true;
}
