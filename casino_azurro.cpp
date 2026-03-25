#include "Azzurro.h"
using namespace std;
#include <bits/stdc++.h>

namespace {
    // グローバル変数と内部関数は無名名前空間内で宣言すること
    // All global variables and internal functions should be declared in an unnamed namespace
    bool precomp = false;
    vector<vector<int>> use(9);

    void init(){
        for (int n = 1; n <= 8; n++){
            vector <int> vec;

            auto check = [&](int x){
                for (int i = 0; i + 1 < n; i++){
                    int v = (1 << i) ^ (1 << (i + 1));
                    if (x == v) return false;
                }
                return true;
            };

            for (int i = 0; i < (1 << n); i++){
                bool good = true;
                for (auto x : vec){
                    good &= check(x ^ i);
                }

                if (good){
                    vec.push_back(i);
                }
            }
            use[n] = vec;
        }
    }
}

std::vector<std::vector<int>> Azzurro(int N, int L, std::string S) {
    if (!precomp){
        precomp = true;
        init();
    }

    long long x = 0;
    for (int i = 0; i < L; i++){
        if (S[i] == 'B'){
            x += 1LL << i;
        }
    }

    vector<vector<int>> mat(N, vector<int>(N, 0));
    int n = N;
    for (int diag = 0; diag <= 2 * (n - 1); diag++){
        int len = min(diag + 1, 2 * (n - 1) - diag + 1);
        int sz = use[len].size();
        int val = x % sz;
        x /= sz;
        val = use[len][val];
        int cnt = 0;
        for (int i = 0; i < n; i++) for (int j = 0; j < n; j++){
            if (i + j == diag){
                if (val >> cnt & 1){
                    mat[i][j] = 1;
                }
                cnt++;
            }
        }
    }

    // cout << "SENT\n";
    // for (int i = 0; i < N; i++){
    //     for (int j = 0; j < N; j++){
    //         cout << mat[i][j];
    //     }
    //     cout << "\n";
    // }

    return mat;
}
