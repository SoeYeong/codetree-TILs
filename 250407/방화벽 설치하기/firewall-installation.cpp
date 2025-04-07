#include <iostream>
#include <queue>

#define MAX 9
#define EMPTY 0
#define WALL 1
#define FIRE 2
using namespace std;

int N, M, ans;
int map[MAX][MAX];
bool visit[MAX][MAX];
int dx[4] = { 0,0,-1,1 };
int dy[4] = { -1,1,0,0 };

int calculate(int tempMap[MAX][MAX]) {
    int cnt = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (tempMap[i][j] == EMPTY) {
                cnt++;
            }
        }
    }
    return cnt;
}
int bfs() {
    queue<pair<int, int>> q;
    int tempMap[MAX][MAX];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            tempMap[i][j] = map[i][j];
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (tempMap[i][j] == FIRE) {
                q.push({ i,j });
            }
        }
    }

    while (!q.empty()) {
        int nowx = q.front().first;
        int nowy = q.front().second;
        q.pop();

        for (int i = 0; i < 4; i++) {
            int nextx = nowx + dx[i];
            int nexty = nowy + dy[i];

            if (nextx < 0 || nextx >= N || nexty < 0 || nexty >= M) continue;
            if (tempMap[nextx][nexty] != EMPTY) continue;
            tempMap[nextx][nexty] = FIRE;
            q.push({ nextx, nexty });
        }
    }

    return calculate(tempMap);
}
void dfs(int cnt) {
    if (cnt == 3) {
        int temp = bfs();
        ans = temp > ans ? temp : ans;
        return;
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (map[i][j] != EMPTY) continue;
            if (visit[i][j]) continue;

            visit[i][j] = true;
            map[i][j] = WALL;
            dfs(cnt + 1);

            map[i][j] = EMPTY;
            visit[i][j] = false;
        }
    }
}
void solution() {
    dfs(0);
    cout << ans;
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    // input
    cin >> N >> M;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            cin >> map[i][j];
        }
    }

    solution();

    return 0;
}