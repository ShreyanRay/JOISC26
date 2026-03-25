#include "Bordeaux.h"
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

std::string Bordeaux(int N, int L, std::vector<std::vector<int>> T) {
    if (!precomp){
        precomp = true;
        init();
    }
    
    long long x = 0;
    int n = N;
    int lst = 0;

    for (int diag = 2 * (n - 1); diag >= 0; diag--){
        int len = min(diag + 1, 2 * (n - 1) - diag + 1);
        int sz = use[len].size();
        x *= sz;

        int v1, v2;
        if (len == 1){
            v1 = 0, v2 = 0;
        } else {
            if (diag >= (n - 1)){
                v1 = lst;
                v2 = min(len - 1, lst + 1);
            } else {
                v1 = min(lst, len - 1);
                v2 = max(0, lst - 1);
            }
        }

        int got = 0;
        int cnt = 0;
        for (int i = 0; i < n; i++){
            for (int j = 0; j < n; j++){
                if (i + j == diag){
                    got += T[i][j] * (1LL << cnt);
                    cnt++;
                }
            }
        }

        int actual = -1;
        for (auto x : use[len]){
            if (__builtin_popcount(got ^ x) == 1){
                int idx = 0;
                while ((got ^ x) != (1LL << idx)){
                    idx++;
                }

                if (idx != v1 && idx != v2) continue;
                actual = x;
            }
        }

        if (actual == -1){
            string bad;
            for (int i = 0; i < L; i++){
                bad += "A";
            }
            return bad;
        }
        int idx = 0;
        while ((got ^ actual) != (1LL << idx)){
            idx++;
        }
        lst = idx;

        int ptr = 0;
        while (use[len][ptr] != actual) ptr++;

        x += ptr;
    }

    string ans = "";
    for (int i = 0; i < L; i++){
        if (x >> i & 1){
            ans += "B";
        } else {
            ans += "A";
        }
    }
    return ans;
}
