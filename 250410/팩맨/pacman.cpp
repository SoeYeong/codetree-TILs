#include <iostream>
#include <vector>
#include <queue>
#include <string.h>

using namespace std;

struct mon {
    int d = 0;
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

int moveCheck(int r, int c, int& d) {
    int origind = d;
    int cnt = 0;
    while (1) {
        if (d == 9) d = 1;

        int nr = r + dr[d];
        int nc = c + dc[d];
        cnt++;

        if (cnt > 8) {
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
    return cnt;
}
void monsterMove() {
    vector<mon> newmap[5][5];

    for (int i = 1; i < 5; i++) {
        for (int j = 1; j < 5; j++) {
            for (int k = 0; k < map[i][j].size(); k++) {
                mon temp = map[i][j][k];

                int cnt = moveCheck(i, j, temp.d);
                if (cnt > 8) {
                    newmap[i][j].push_back(temp);
                    continue;
                }

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
void pacmanMoveCheck(int& d) {
    int nr, nc;

    while (1) {
        nr = pm.r + dr[d];
        nc = pm.c + dc[d];
        if (nr < 1 || nr > 4 || nc < 1 || nc > 4) {
            d += 2;
            d %= 9;
            if (d == 0) 
                d = 1;
        }
        else {
            break;
        }
    }
}
void removeMonster(int d1, int d2, int d3) {
    pacmanMoveCheck(d1);
    pm.r += dr[d1];
    pm.c += dc[d1];

    while (!map[pm.r][pm.c].empty()) {
        mon temp = map[pm.r][pm.c].back();
        map[pm.r][pm.c].pop_back();

        temp.dead = turn;
        dead[pm.r][pm.c].push_back(temp);
    }

    pacmanMoveCheck(d2);
    pm.r += dr[d2];
    pm.c += dc[d2];
    while (!map[pm.r][pm.c].empty()) {
        mon temp = map[pm.r][pm.c].back();
        map[pm.r][pm.c].pop_back();

        temp.dead = turn;
        dead[pm.r][pm.c].push_back(temp);
    }

    pacmanMoveCheck(d3);
    pm.r += dr[d3];
    pm.c += dc[d3];
    while (!map[pm.r][pm.c].empty()) {
        mon temp = map[pm.r][pm.c].back();
        map[pm.r][pm.c].pop_back();

        temp.dead = turn;
        dead[pm.r][pm.c].push_back(temp);
    }
}
void dfs(int r, int c, int cnt, int val, int d1, int d2, int d3) {
    if (cnt == 3) {
        maxx[d1][d2][d3] = val > maxx[d1][d2][d3] ? val : maxx[d1][d2][d3];
        return;
    }

    for (int i = 1; i < 9; i += 2) {
        int nr = r + dr[i];
        int nc = c + dc[i];
        if (nr < 1 || nr > 4 || nc < 1 || nc > 4) continue;

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

        if (visit[nr][nc]) {
            dfs(nr, nc, cnt + 1, val, d1, d2, d3);
        }
        else {
            visit[nr][nc] = 1;
            dfs(nr, nc, cnt + 1, val + s, d1, d2, d3);
            visit[nr][nc] = 0;
        }
    }
}
void pacmanMove() {
    memset(maxx, -1, sizeof(maxx));
    memset(visit, 0, sizeof(visit));
    visit[pm.r][pm.c] = 1;

    dfs(pm.r, pm.c, 0, 0, 0, 0, 0);
    int d1 = 0, d2 = 0, d3 = 0;
    int temp = -1;

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
    vector<mon> temp;
    for (int i = 1; i < 5; i++) {
        for (int j = 1; j < 5; j++) {
            for (int k = 0; k < dead[i][j].size(); k++) {
                if (turn - dead[i][j][k].dead <= 2) {
                    temp.push_back(dead[i][j][k]);
                }
            }
            dead[i][j].swap(temp);
            temp.clear();
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