#include <iostream>
#include <vector>
#include <queue>
#include <string.h>

using namespace std;

struct mon {
    int d;
    int dead = -1;
};
struct pos {
    int r, c;
};

int M, T, turn;
pos pm;
vector<mon> map[5][5];
vector<mon> egg[5][5];
vector<mon> dead[5][5];
int maxx[4][4][4];
int visit[5][5];
int dr[9] = { 0,-1,-1,0,1,1,1,0,-1 };
int dc[9] = { 0,0,-1,-1,-1,0,1,1,1 };

void moveCheck(int r, int c, int& d) {
    int origind = d;
    int cnt = 0;
    while (1) {
        d %= 9;
        if (d == 0) d++;

        int nr = r + dr[d];
        int nc = c + dc[d];
        cnt++;

        if (cnt == 8) {
            d = origind;
            break;
        }

        // out of range
        if (nr < 1 || nr > 4 || nc < 1 || nc > 4) {
            d++;
            continue;
        }

        // pacman
        if (nr == pm.r && nc == pm.c) {
            d++;
            continue;
        }

        // dead
        if (dead[nr][nc].size() > 0) {
            d++;
            continue;
        }

        break;
    }
}
void monsterMove() {
    vector<mon> newmap[5][5];

    for (int i = 1; i < 5; i++) {
        for (int j = 1; j < 5; j++) {
            for (int k = 0; k < map[i][j].size(); k++) {
                mon temp = map[i][j][k];

                moveCheck(i, j, temp.d);
                int nr = i + dr[temp.d];
                int nc = j + dc[temp.d];

                newmap[nr][nc].push_back(temp);
            }
        }
    }

    for (int i = 1; i < 5; i++) {
        for (int j = 1; j < 5; j++) {
            map[i][j].swap(newmap[i][j]);
        }
    }
}
void monsterCopy() {
    for (int i = 1; i < 5; i++) {
        for (int j = 1; j < 5; j++) {
            if (map[i][j].size() == 0) continue;
            
            for (int k = 0; k < map[i][j].size(); k++) {
                egg[i][j].push_back({ map[i][j][k].d, -1 });
            }
        }
    }
}
void removeMonster(int d1, int d2, int d3) {
    int nr = pm.r + dr[d1];
    int nc = pm.c + dc[d1];

    while (!map[nr][nc].empty()) {
        mon temp = map[nr][nc].back();
        map[nr][nc].pop_back();

        temp.dead = turn;
        dead[nr][nc].push_back(temp);
    }

    nr += dr[d2];
    nc += dc[d2];
    while (!map[nr][nc].empty()) {
        mon temp = map[nr][nc].back();
        map[nr][nc].pop_back();

        temp.dead = turn;
        dead[nr][nc].push_back(temp);
    }

    nr += dr[d3];
    nc += dc[d3];
    while (!map[nr][nc].empty()) {
        mon temp = map[nr][nc].back();
        map[nr][nc].pop_back();

        temp.dead = turn;
        dead[nr][nc].push_back(temp);
    }

    pm.r = nr;
    pm.c = nc;
}
void dfs(int r, int c, int cnt, int val, int d1, int d2, int d3) {
    if (cnt == 3) {
        maxx[d1][d2][d3] = val > maxx[d1][d2][d3] ? val : maxx[d1][d2][d3];
        return;
    }

    for (int i = 1; i < 9; i += 2) {
        int nr = r + dr[i];
        int nc = c + dc[i];
        if (nr < 1 || nr > 5 || nc < 1 || nc > 5) continue;
        if (visit[nr][nc]) continue;

        visit[nr][nc] = 1;
        int s = map[nr][nc].size();
        int d = i / 2;

        if (cnt == 0) {
            d1 = d;
        }
        else if (cnt == 1) {
            d2 = d;
        }
        else {
            d3 = d;
        }
        dfs(nr, nc, cnt + 1, val + s, d1, d2, d3);
        visit[nr][nc] = 0;
    }
}
void pacmanMove() {
    memset(maxx, 0, sizeof(maxx));
    memset(visit, 0, sizeof(visit));

    dfs(pm.r, pm.c, 0, 0, 0, 0, 0);
    int d1 = 0, d2 = 0, d3 = 0;
    int temp = 0;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                if (temp < maxx[i][j][k]) {
                    temp = maxx[i][j][k];
                    d1 = i, d2 = j, d3 = k;
                }
            }
        }
    }

    d1 = d1 * 2 + 1;
    d2 = d2 * 2 + 1;
    d3 = d3 * 2 + 1;
    removeMonster(d1, d2, d3);
}
void monsterComplete() {
    for (int i = 1; i < 5; i++) {
        for (int j = 1; j < 5; j++) {
            if (egg[i][j].size() == 0) continue;

            while (!egg[i][j].empty()) {
                mon temp = egg[i][j].back();
                egg[i][j].pop_back();
                map[i][j].push_back(temp);
            }
        }
    }
}
void deadCheck() {
    for (int i = 1; i < 5; i++) {
        for (int j = 1; j < 5; j++) {
            for (int k = 0; k < dead[i][j].size(); k++) {
                if (turn - dead[i][j][k].dead > 2) {
                    dead[i][j].erase(dead[i][j].begin() + k);
                    k--;
                }
            }
        }
    }
}
void countMonster() {
    int cnt = 0;
    for (int i = 1; i < 5; i++) {
        for (int j = 1; j < 5; j++) {
            cnt += map[i][j].size();
        }
    }

    cout << cnt;
}
void solution() {
    turn = 1;
    while (1) {
        if (turn > T) {
            break;
        }
        deadCheck();
        monsterCopy();
        monsterMove();
        pacmanMove();
        monsterComplete();

        turn++;
    }

    countMonster();
}
void input() {
    cin >> M >> T;
    cin >> pm.r >> pm.c;

    mon temp;
    int r, c;
    for (int i = 0; i < M; i++) {
        cin >> r >> c >> temp.d;
        map[r][c].push_back(temp);
    }

}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    input();
    solution();


    return 0;
}