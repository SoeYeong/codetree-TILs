#include <iostream>
#include <queue>

using namespace std;

int N, M;
char map[10][10];
int visit[10][10][10][10];
int dx[4] = { 0,0,-1,1 };
int dy[4] = { -1,1,0,0 };

struct step {
    int rx, ry;
    int bx, by;
    int cnt;
};
queue<step> q;


void move(int& x, int& y, int& distance, int i) {
    while (map[x + dx[i]][y + dy[i]] != '#' && map[x][y] != 'O') {
        x += dx[i];
        y += dy[i];
        distance += 1;
    }
}
void bfs(int rx, int ry, int bx, int by) {
    q.push({ rx, ry, bx, by, 0 });
    visit[rx][ry][bx][by] = 1;

    while (!q.empty()) {
        step now = q.front();
        q.pop();

        if (now.cnt >= 10) break;

        for (int i = 0; i < 4; i++) {
            step next;
            next.rx = now.rx;
            next.ry = now.ry;
            next.bx = now.bx;
            next.by = now.by;
            next.cnt = now.cnt + 1;

            int rd = 0, bd = 0;

            move(next.rx, next.ry, rd, i);
            move(next.bx, next.by, bd, i);

            if (map[next.bx][next.by] == 'O') continue;
            if (map[next.rx][next.ry] == 'O') {
                cout << next.cnt;
                return;
            }

            if (next.rx == next.bx && next.ry == next.by) {
                if (rd > bd) {
                    next.rx -= dx[i];
                    next.ry -= dy[i];
                }
                else {
                    next.bx -= dx[i];
                    next.by -= dy[i];
                }
            }

            if (visit[next.rx][next.ry][next.bx][next.by]) continue;
            visit[next.rx][next.ry][next.bx][next.by] = 1;
            q.push(next);
        }
    }

    cout << -1;
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int Rx = 0, Ry = 0, Bx = 0, By = 0;
    cin >> N >> M;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            cin >> map[i][j];

            if (map[i][j] == 'R') {
                Rx = i, Ry = j;
            }
            else if (map[i][j] == 'B') {
                Bx = i, By = j;
            }
        }
    }

    bfs(Rx, Ry, Bx, By);

    return 0;
}