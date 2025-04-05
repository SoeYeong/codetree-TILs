#include <iostream>

#define MAX 1000001
using namespace std;

int N;
int store[MAX];
int tester[2];
int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    // input
    cin >> N;
    for (int i = 0; i < N; i++) {
        cin >> store[i];
    }
    cin >> tester[0] >> tester[1];

    // solve
    long long ans = 0;
    for (int i = 0; i < N; i++) {
        ans += 1;
        if (store[i] <= tester[0]) {
            continue;
        }
        store[i] -= tester[0];
        ans += (long long)(store[i] - 1) / tester[1] + 1;
    }

    cout << ans;

    return 0;
}