#include <iostream>

#define MAX 16
using namespace std;

int N;
int work[MAX][2];
int ans = 0;

void dfs(int idx, int val, int i) {
    if (idx > N) {
        val -= work[i][1];
        ans = (val > ans) ? val : ans;
        return;
    }
    if (idx == N) {
        ans = (val > ans) ? val : ans;
        return;
    }

    for (i = idx + 1; i <= N; i++) {
        dfs(i + work[i][0] - 1, val + work[i][1], i);
    }
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    // input
    cin >> N;
    for (int i = 1; i <= N; i++) {
        cin >> work[i][0] >> work[i][1];
    }
    
    dfs(0, 0, 0);
    cout << ans;

    return 0;
}