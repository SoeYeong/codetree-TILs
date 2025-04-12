#include <iostream>
#include <vector>
#include <deque>
#include <string.h>

#define MAX 102
using namespace std;

int N, K;
int pizza[MAX][MAX];
int dx[4] = { 0,-1,0,1 };
int dy[4] = { -1,0,1,0 };

void findSmall() {
    int minn = 10000;
    vector<int> v;

    for (int i = 1; i <= N; i++) {
        if (minn > pizza[N][i]) {
            v.clear();
            minn = pizza[N][i];
            v.push_back(i);
        }
        else if (minn == pizza[N][i]) {
            v.push_back(i);
        }
    }

    for (int i = 0; i < v.size(); i++) {
        int idx = v[i];
        pizza[N][idx]++;
    }
}
int findDifference() {
    int minn = 10000;
    int maxx = 0;

    for (int i = 1; i <= N; i++) {
        if (minn > pizza[N][i]) {
            minn = pizza[N][i];
        }
        if (maxx < pizza[N][i]) {
            maxx = pizza[N][i];
        }
    }

    return maxx - minn;
}

void foldHalf() {
    int tempPizza[MAX][MAX];
    memset(tempPizza, 0, sizeof(tempPizza));

    int r = N - 3;
    int idx = 1;
    for (int c = 0; c < N / 4; c++) {
        tempPizza[r + 2][N - c] = pizza[N][idx];
        idx++;
    }
    for (int c = 0; c < N / 4; c++) {
        tempPizza[r + 1][N / 4 * 3 + 1 + c] = pizza[N][idx];
        idx++;
    }
    for (int c = 0; c < N / 4; c++) {
        tempPizza[r][N - c] = pizza[N][idx];
        idx++;
    }
    for (int c = 0; c < N / 4; c++) {
        tempPizza[N][idx] = pizza[N][idx];
        idx++;
    }

    for (int i = r; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            pizza[i][j] = tempPizza[i][j];
        }
    }
}
void pressPizza() {
    int tempPizza[MAX][MAX];
    memset(tempPizza, 0, sizeof(tempPizza));

    // d 계산 -> tempPizza에 저장
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (pizza[i][j] == 0) continue;

            for (int k = 0; k < 4; k++) {
                int nx = i + dx[k];
                int ny = j + dy[k];
                if (nx < 1 || nx > N || ny < 1 || ny > N) continue;
                if (pizza[nx][ny] == 0) continue;

                if (pizza[i][j] > pizza[nx][ny]) {
                    int d = (pizza[i][j] - pizza[nx][ny]) / 5;
                    tempPizza[i][j] -= d;
                }
                else {
                    int d = (pizza[nx][ny] - pizza[i][j]) / 5;
                    tempPizza[i][j] += d;
                }
            }
        }
    }

    // d 계산값 더해줌
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (tempPizza[i][j] == 0) continue;
            pizza[i][j] += tempPizza[i][j];
            tempPizza[i][j] = 0;
        }
    }

    // 도우 펼치기
    int idx = 1;
    for (int col = 1; col <= N; col++) {
        if (pizza[N][col] == 0) continue;

        for (int row = N; row > 0; row--) {
            if (pizza[row][col] == 0) break;
            tempPizza[N][idx] = pizza[row][col];
            idx++;
        }
    }

    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            pizza[i][j] = tempPizza[i][j];
        }
    }
}
void rollPizza() {
    vector<int> tempPizza;
    for (int i = 1; i <= N; i++) {
        tempPizza.push_back(pizza[N][i]);
    }
    memset(pizza, 0, sizeof(pizza));

    vector<int> v;
    int sum = 0;
    for (int i = 1; i < N; i++) {
        if (sum + i > N) {
            v.push_back(N - sum);
            break;
        }
        sum += i;
        v.push_back(i);

        if (sum + i > N) {
            v.push_back(N - sum);
            break;
        }
        sum += i;
        v.push_back(i);
    }

    int r = N, c = N + 1, d = 0;
    int last = v.size() - 1;
    for (int i = 0; i < v[last]; i++) {
        r += dx[0];
        c += dy[0];
        pizza[r][c] = tempPizza.back();
        tempPizza.pop_back();
    }
    for (int i = last - 1; i >= 0; i--, d++) {
        d %= 4;
        for (int j = 0; j < v[i]; j++) {
            r += dx[d];
            c += dy[d];
            pizza[r][c] = tempPizza.back();
            tempPizza.pop_back();
        }
    }
}
void solution() {
    int cnt = 1;

    while (1) {
        findSmall();

        rollPizza();
        pressPizza();
        foldHalf();
        pressPizza();

        int diff = findDifference();
        if (diff <= K) {
            break;
        }
        cnt++;
    }
    cout << cnt;
}
void input() {
    cin >> N >> K;
    for (int i = 1; i <= N; i++) {
        cin >> pizza[N][i];
    }
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    input();
    solution();

    return 0;
}