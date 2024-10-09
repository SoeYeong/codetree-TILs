#include <iostream>
#include <cstring>
#include <algorithm>
#include <queue>
using namespace std;

int R, C, K;
int dy[4] = { -1,0,1,0 };   // 북, 동, 남, 서
int dx[4] = { 0,1,0,-1 };
int map[73][71];
bool exitt[73][71];
struct info {
    int y, x;
    int exitd;
};
info golem[1001];
int ans;

void init() {
    for (int i = 3; i <= R + 2; i++) {
        for (int j = 1; j <= C; j++) {
            map[i][j] = 0;
            exitt[i][j] = false;
        }
    }
}
bool moveNorth(int idx) {
    int y1 = golem[idx].y + 1;
    int y2 = golem[idx].y + 2;
    int x1 = golem[idx].x - 1;
    int x2 = golem[idx].x + 1;

    if (y2 > R+2) return false;
    if (map[y1][x1] == 0 && map[y1][x2] == 0 && map[y2][golem[idx].x] == 0) {
        golem[idx].y++;
        return true;
    }
    return false;
}
bool moveEast(int idx) {
    int y1 = golem[idx].y - 1;
    int y2 = golem[idx].y + 1;
    int y3 = golem[idx].y + 2;
    int x1 = golem[idx].x + 1;
    int x2 = golem[idx].x + 2;

    if (x2 > C || y3 > R+2) return false;
    if (map[y1][x1] == 0 && map[y2][x1] == 0 && map[golem[idx].y][x2] == 0
        && map[y2][x2] == 0 && map[y3][x1] == 0) {

        golem[idx].x++;
        golem[idx].exitd++;
        golem[idx].exitd %= 4;
        moveNorth(idx);
        return true;
    }
    return false;
}
bool moveWest(int idx) {
    int y1 = golem[idx].y - 1;
    int y2 = golem[idx].y + 1;
    int y3 = golem[idx].y + 2;
    int x1 = golem[idx].x - 1;
    int x2 = golem[idx].x - 2;

    if (x2 < 1 || y3 > R+2) return false;
    if (map[y1][x1] == 0 && map[y2][x1] == 0 && map[golem[idx].y][x2] == 0
        && map[y2][x2] == 0 && map[y3][x1] == 0) {
        
        golem[idx].x--;
        golem[idx].exitd--;
        if (golem[idx].exitd < 0) {
            golem[idx].exitd = 3;
        }
        moveNorth(idx);
        return true;
    }
    return false;
}
void bfs(int idx) {
    int rowM = golem[idx].y;
    bool visit[73][71];
    memset(visit, false, sizeof(visit));

    queue<pair<int, int>> q;
    q.push({ golem[idx].y,golem[idx].x });
    visit[golem[idx].y][golem[idx].x] = true;

    while (!q.empty()) {
        pair<int, int> now = q.front();
        q.pop();

        for (int d = 0; d < 4; d++) {
            int ny = now.first + dy[d];
            int nx = now.second + dx[d];

            if (ny < 3 || ny > R+2 || nx < 1 || nx > C) continue;
            if (visit[ny][nx] || map[ny][nx] == 0) continue;
            if (map[ny][nx] == map[now.first][now.second]) {
                visit[ny][nx] = true;
                q.push({ ny,nx });
                rowM = max(rowM, ny);
            }
            if (map[ny][nx] != map[now.first][now.second] && exitt[now.first][now.second]) {
                visit[ny][nx] = true;
                q.push({ ny,nx });
                rowM = max(rowM, ny);
            }
        }
    }
    rowM -= 2;
    ans += rowM;
}
void print(int idx) {
    cout << "#" << idx << " \n";
    for (int i = 3; i <= R+2; i++) {
        for (int j = 1; j <= C; j++) {
            cout << map[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}
void solution() {
    for (int i = 1; i <= K; i++) {
        // 골렘 이동
        while (1) {
            bool flag = moveNorth(i);
            if (flag) {
                continue;
            }
            flag = moveWest(i);
            if (flag) {
                continue;
            }
            flag = moveEast(i);
            if (flag) {
                continue;
            }
            break;
        }
        
        if (golem[i].y < 4) {
            init();
            continue;
        }

        // 맵에 표시
        map[golem[i].y][golem[i].x] = i;
        for (int d = 0; d < 4; d++) {
            int ny = golem[i].y + dy[d];
            int nx = golem[i].x + dx[d];
            map[ny][nx] = i;
        }
        exitt[golem[i].y + dy[golem[i].exitd]][golem[i].x + dx[golem[i].exitd]] = true;

        // 정령 이동
        bfs(i);
    }
    cout << ans;
}
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);
    
    cin >> R >> C >> K;
    for (int i = 1; i <= K; i++) {
        cin >> golem[i].x >> golem[i].exitd;
        golem[i].y = 1;
    }
    
    solution();

    return 0;
}