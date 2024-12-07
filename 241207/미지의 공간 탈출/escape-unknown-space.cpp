#include <iostream>
#include <queue>

#define MAX 21

#define EAST 0
#define WEST 1
#define SOUTH 2
#define NORTH 3
#define TOP 4
#define BOTTOM 5

#define RIGHT 0
#define LEFT 1
#define DOWN 2
#define UP 3
using namespace std;

struct pos {
    int p, r, c;
};
struct info {
    int r, c, d, v, p;
};

int N, M, F;
int map[6][MAX][MAX];
int anomaly_time[6][MAX][MAX];
int visit[6][MAX][MAX];

info anomaly[10];
pos exitt, current, cube;
pos nextt[6][MAX][MAX][4];

// 동, 서 남, 북
int dr[4] = { 0,0,1,-1 };
int dc[4] = { 1,-1,0,0 };

void input() {
    cin >> N >> M >> F;

    // map
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            cin >> map[BOTTOM][i][j];

            if (map[BOTTOM][i][j] == 4) {
                exitt = { BOTTOM, i, j };
            }
            else if (map[BOTTOM][i][j] == 3) {
                cube = { BOTTOM, i - M + 1, j - M + 1 };
            }
        }
    }

    // time wall
    for (int i = 0; i < 5; i++) {
        for (int j = 1; j <= M; j++) {
            for (int k = 1; k <= M; k++) {
                cin >> map[i][j][k];

                if (map[i][j][k] == 2) {
                    current = { i, j, k };
                }
            }
        }
    }

    // anomaly
    for (int i = 0; i < F; i++) {
        cin >> anomaly[i].r >> anomaly[i].c >> anomaly[i].d >> anomaly[i].v;
        anomaly[i].r++;
        anomaly[i].c++;
        anomaly[i].p = BOTTOM;
    }
}
void next_on_cube() {
    for (int i = 1; i <= M; i++) {
        // TOP -> EAST
        nextt[TOP][i][M][RIGHT].p = EAST;
        nextt[TOP][i][M][RIGHT].r = 1;
        nextt[TOP][i][M][RIGHT].c = M + 1 - i;

        // TOP -> WEST
        nextt[TOP][i][1][LEFT].p = WEST;
        nextt[TOP][i][1][LEFT].r = 1;
        nextt[TOP][i][1][LEFT].c = i;

        // TOP -> SOUTH
        nextt[TOP][M][i][DOWN].p = SOUTH;
        nextt[TOP][M][i][DOWN].r = 1;
        nextt[TOP][M][i][DOWN].c = i;

        // TOP -> NORTH
        nextt[TOP][1][i][UP].p = NORTH;
        nextt[TOP][1][i][UP].r = 1;
        nextt[TOP][1][i][UP].c = M + 1 - i;



        // EAST -> TOP
        nextt[EAST][1][i][UP].p = TOP;
        nextt[EAST][1][i][UP].r = M + 1 - i;
        nextt[EAST][1][i][UP].c = M;

        // WEST -> TOP
        nextt[WEST][1][i][UP].p = TOP;
        nextt[WEST][1][i][UP].r = i;
        nextt[WEST][1][i][UP].c = 1;

        // SOUTH -> TOP
        nextt[SOUTH][1][i][UP].p = TOP;
        nextt[SOUTH][1][i][UP].r = M;
        nextt[SOUTH][1][i][UP].c = i;

        // NORTH -> TOP
        nextt[NORTH][1][i][UP].p = TOP;
        nextt[NORTH][1][i][UP].r = 1;
        nextt[NORTH][1][i][UP].c = M + 1 - i;


        // ->
        //WEST -> SOUTH
        nextt[WEST][i][M][RIGHT].p = SOUTH;
        nextt[WEST][i][M][RIGHT].r = i;
        nextt[WEST][i][M][RIGHT].c = 1;

        // SOUTH -> EAST
        nextt[SOUTH][i][M][RIGHT].p = EAST;
        nextt[SOUTH][i][M][RIGHT].r = i;
        nextt[SOUTH][i][M][RIGHT].c = 1;

        // EAST -> NORTH
        nextt[EAST][i][M][RIGHT].p = NORTH;
        nextt[EAST][i][M][RIGHT].r = i;
        nextt[EAST][i][M][RIGHT].c = 1;

        // NORTH -> WEST
        nextt[NORTH][i][M][RIGHT].p = WEST;
        nextt[NORTH][i][M][RIGHT].r = i;
        nextt[NORTH][i][M][RIGHT].c = 1;


        // <-
        // WEST -> NORTH
        nextt[WEST][i][1][LEFT].p = NORTH;
        nextt[WEST][i][1][LEFT].r = i;
        nextt[WEST][i][1][LEFT].c = M;

        // NORTH -> EAST
        nextt[NORTH][i][1][LEFT].p = EAST;
        nextt[NORTH][i][1][LEFT].r = i;
        nextt[NORTH][i][1][LEFT].c = M;

        // EAST -> SOUTH
        nextt[EAST][i][1][LEFT].p = SOUTH;
        nextt[EAST][i][1][LEFT].r = i;
        nextt[EAST][i][1][LEFT].c = M;

        // SOUTH -> WEST
        nextt[SOUTH][i][1][LEFT].p = WEST;
        nextt[SOUTH][i][1][LEFT].r = i;
        nextt[SOUTH][i][1][LEFT].c = M;
    }

    for (int i = 1; i <= M; i++) {
        // EAST -> BOTTOM
        nextt[EAST][M][i][DOWN].p = BOTTOM;
        nextt[EAST][M][i][DOWN].r = cube.r + M - i;
        nextt[EAST][M][i][DOWN].c = cube.c + M;

        // WEST -> BOTTOM
        nextt[WEST][M][i][DOWN].p = BOTTOM;
        nextt[WEST][M][i][DOWN].r = cube.r + i - 1;
        nextt[WEST][M][i][DOWN].c = cube.c - 1;

        // SOUTH -> BOTTOM
        nextt[SOUTH][M][i][DOWN].p = BOTTOM;
        nextt[SOUTH][M][i][DOWN].r = cube.r + M;
        nextt[SOUTH][M][i][DOWN].c = cube.c + i - 1;

        // NORTH -> BOTTOM
        nextt[NORTH][M][i][DOWN].p = BOTTOM;
        nextt[NORTH][M][i][DOWN].r = cube.r - 1;
        nextt[NORTH][M][i][DOWN].c = cube.c + M - i;


        // BOTTOM -> EAST
        nextt[BOTTOM][cube.r + M - i][cube.c + M][LEFT].p = EAST;
        nextt[BOTTOM][cube.r + M - i][cube.c + M][LEFT].r = M;
        nextt[BOTTOM][cube.r + M - i][cube.c + M][LEFT].c = i;

        // BOTTOM -> WEST
        nextt[BOTTOM][cube.r + i - 1][cube.c - 1][RIGHT].p = WEST;
        nextt[BOTTOM][cube.r + i - 1][cube.c - 1][RIGHT].r = M;
        nextt[BOTTOM][cube.r + i - 1][cube.c - 1][RIGHT].c = i;

        // BOTTOM -> SOUTH
        nextt[BOTTOM][cube.r + M][cube.c + i - 1][UP].p = SOUTH;
        nextt[BOTTOM][cube.r + M][cube.c + i - 1][UP].r = M;
        nextt[BOTTOM][cube.r + M][cube.c + i - 1][UP].c = i;

        // BOTTOM -> NORTH
        nextt[BOTTOM][cube.r - 1][cube.c + M - i][DOWN].p = NORTH;
        nextt[BOTTOM][cube.r - 1][cube.c + M - i][DOWN].r = M;
        nextt[BOTTOM][cube.r - 1][cube.c + M - i][DOWN].c = i;
    }
}
void anomaly_check() {
    for (int i = 0; i < F; i++) {
        int p = anomaly[i].p;
        int r = anomaly[i].r;
        int c = anomaly[i].c;
        int v = anomaly[i].v;
        int d = anomaly[i].d;

        anomaly_time[p][r][c] = 1;
        
        while (1) {
            int np = p;
            int nr = r + dr[d];
            int nc = c + dc[d];

            if (p == BOTTOM && map[p][nr][nc] == 3) {
                np = nextt[p][r][c][d].p;
                nr = nextt[p][r][c][d].r;
                nc = nextt[p][r][c][d].c;
            }
            else if (p != BOTTOM && (nr < 1 || nr > M || nc < 1 || nc > M)) {
                np = nextt[p][r][c][d].p;
                nr = nextt[p][r][c][d].r;
                nc = nextt[p][r][c][d].c;
            }

            if (np == BOTTOM && (nr < 1 || nr > N || nc < 1 || nc > N)) break;
            if (map[np][nr][nc] == 1 || map[np][nr][nc] == 4) break;

            anomaly_time[np][nr][nc] = anomaly_time[p][r][c] + v;

            p = np;
            r = nr;
            c = nc;
        }
    }
}
void bfs() {
    queue<pos> q;
    q.push(current);
    visit[current.p][current.r][current.c] = 1;

    while (!q.empty()) {
        pos now = q.front();
        q.pop();

        if (map[now.p][now.r][now.c] == 4) {
            break;
        }

        for (int i = 0; i < 4; i++) {
            int np = now.p;
            int nr = now.r + dr[i];
            int nc = now.c + dc[i];

            if (np == BOTTOM && (nr < 1 || nr > N || nc < 1 || nc > N)) continue;
            if (np == BOTTOM && map[np][nr][nc] == 3) continue;
            if (np != BOTTOM && (nr < 1 || nr > M || nc < 1 || nc > M)) {
                np = nextt[now.p][now.r][now.c][i].p;
                nr = nextt[now.p][now.r][now.c][i].r;
                nc = nextt[now.p][now.r][now.c][i].c;
            }

            // 장애물 & 방문 & 이상 현상
            if (map[np][nr][nc] == 1) continue;
            if (visit[np][nr][nc]) continue;
            if (anomaly_time[now.p][now.r][now.c] > 0 && 
                visit[now.p][now.r][now.c] + 1 >= anomaly_time[now.p][now.r][now.c]) continue;

            q.push({ np, nr, nc });
            visit[np][nr][nc] = visit[now.p][now.r][now.c] + 1;
        }
    }
}
void solution() {
    input();
    next_on_cube();
    anomaly_check();
    bfs();

    cout << visit[exitt.p][exitt.r][exitt.c] - 1;
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    solution();

    return 0;
}