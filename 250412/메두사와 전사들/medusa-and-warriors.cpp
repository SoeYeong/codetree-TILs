#include <iostream>
#include <vector>
#include <queue>
#include <string.h>

#define MAX 51
#define SIGHT 3
#define STONE 4
#define HIDE 5
using namespace std;

struct pos {
    int r, c;
};
struct soldier {
    int r, c;
    bool stone, die;
};

int N, M;
pos medusa, home, park;
vector<pos> route;
vector<soldier> soldiers;
vector<int> sdMap[MAX][MAX];
int map[MAX][MAX];
int visit[MAX][MAX];
int sight[MAX][MAX];
int dist, stone, attack;

// 상, 하, 좌, 우
int dr[4] = { -1,1,0,0 };
int dc[4] = { 0,0,-1,1 };

void init() {
    dist = stone = attack = 0;

    for (int i = 0; i < M; i++) {
        if (soldiers[i].die) continue;
        if (soldiers[i].stone) {
            soldiers[i].stone = false;
        }
    }
}
bool outOfBound(int r, int c) {
    if (r < 0 || r >= N || c < 0 || c >= N) {
        return false;
    }
    return true;
}
void findRoute() {
    pos tmpRoute[MAX][MAX];
    queue<pos> q;
    q.push(home);
    visit[home.r][home.c] = 1;

    while (!q.empty()) {
        pos now = q.front();
        q.pop();

        for (int i = 0; i < 4; i++) {
            int nr = now.r + dr[i];
            int nc = now.c + dc[i];

            if (outOfBound(nr, nc) == false) continue;
            if (map[nr][nc]) continue;
            if (visit[nr][nc]) continue;
            visit[nr][nc] = visit[now.r][now.c] + 1;
            q.push({ nr,nc });
            tmpRoute[nr][nc] = now;
        }
    }

    // 공원 못 가는 경우
    if (visit[park.r][park.c] == 0) {
        return;
    }

    // 집 -> 공원 경로 저장
    route.push_back(park);
    while (1) {
        pos now = route.back();
        pos next = tmpRoute[now.r][now.c];

        if (next.r == home.r && next.c == home.c) {
            break;
        }
        route.push_back(next);
    }
}
void medusaMove() {
    pos next = route.back();
    route.pop_back();
    medusa = next;

    // 움직인 곳에 전사 있는 경우
    if (sdMap[medusa.r][medusa.c].size()) {
        for (int i = 0; i < sdMap[medusa.r][medusa.c].size(); i++) {
            int idx = sdMap[medusa.r][medusa.c][i];
            soldiers[idx].die = true;
        }
        sdMap[medusa.r][medusa.c].clear();
    }
}
void meetSoldier(int r, int c, int dir) {
    // 상
    if (dir == 0) {
        if (c == medusa.c) {
            for (int nr = r - 1; r >= 0; r--) {
                sight[nr][c] = HIDE;
            }
        }
        else if (c < medusa.c) {
            int i = 1;
            for (int nr = r - i; nr >= 0; nr--) {
                for (int nc = c - i; nc <= c; nc++) {
                    if (nc < 0) continue;
                    sight[nr][nc] = HIDE;
                }
                i++;
            }
        }
        else {
            int i = 1;
            for (int nr = r - i; nr >= 0; nr--) {
                for (int nc = c; nc <= c + i; nc++) {
                    if (nc >= N) continue;
                    sight[nr][nc] = HIDE;
                }
                i++;
            }
        }
    }
    // 하
    else if (dir == 1) {
        if (c == medusa.c) {
            for (int nr = r + 1; r < N; r++) {
                sight[nr][c] = HIDE;
            }
        }
        else if (c < medusa.c) {
            int i = 1;
            for (int nr = r + i; nr < N; nr++) {
                for (int nc = c - i; nc <= c; nc++) {
                    if (nc < 0) continue;
                    sight[nr][nc] = HIDE;
                }
                i++;
            }
        }
        else {
            int i = 1;
            for (int nr = r + i; nr < N; nr++) {
                for (int nc = c; nc <= c + i; nc++) {
                    if (nc >= N) continue;
                    sight[nr][nc] = HIDE;
                }
                i++;
            }
        }
    }
    // 좌
    else if (dir == 2) {
        if (r == medusa.r) {
            for (int nc = c - 1; c >= 0; c--) {
                sight[r][nc] = HIDE;
            }
        }
        else if (r < medusa.r) {
            int i = 1;
            for (int nc = c - i; nc >= 0; nc--) {
                for (int nr = r - i; nr <= r; nr++) {
                    if (nr < 0) continue;
                    sight[nr][nc] = HIDE;
                }
                i++;
            }
        }
        else {
            int i = 1;
            for (int nc = c - i; nc >= 0; nc--) {
                for (int nr = r; nr <= r + i; nr++) {
                    if (nr >= N) continue;
                    sight[nr][nc] = HIDE;
                }
                i++;
            }
        }
    }
    // 우
    else {
        if (r == medusa.r) {
            for (int nc = c + 1; c < N; c++) {
                sight[r][nc] = HIDE;
            }
        }
        else if (r < medusa.r) {
            int i = 1;
            for (int nc = c + i; nc < N; nc++) {
                for (int nr = r - i; nr <= r; nr++) {
                    if (nr < 0) continue;
                    sight[nr][nc] = HIDE;
                }
                i++;
            }
        }
        else {
            int i = 1;
            for (int nc = c + i; nc < N; nc++) {
                for (int nr = r; nr <= r + i; nr++) {
                    if (nr >= N) continue;
                    sight[nr][nc] = HIDE;
                }
                i++;
            }
        }
    }

}
int medusaSee(int dir) {
    memset(sight, 0, sizeof(sight));
    int cnt = 0;

    // 상
    if (dir == 0) {
        for (int i = 1; medusa.r + dr[dir] * i >= 0; i++) {
            int r = medusa.r + dr[dir] * i;
            for (int c = medusa.c - i; c <= medusa.c + i; c++) {
                if (c < 0) continue;
                if (c >= N) continue;
                if (sight[r][c] == HIDE) continue;

                if (sdMap[r][c].size()) {
                    sight[r][c] = STONE;
                    meetSoldier(r, c, dir);
                    cnt += sdMap[r][c].size();
                    continue;
                }

                sight[r][c] = SIGHT;
            }
        }
    }
    // 하
    else if (dir == 1) {
        for (int i = 1; medusa.r + dr[dir] * i < N; i++) {
            int r = medusa.r + dr[dir] * i;
            for (int c = medusa.c - i; c <= medusa.c + i; c++) {
                if (c < 0) continue;
                if (c >= N) continue;
                if (sight[r][c] == HIDE) continue;

                if (sdMap[r][c].size()) {
                    sight[r][c] = STONE;
                    meetSoldier(r, c, dir);
                    cnt += sdMap[r][c].size();
                    continue;
                }

                sight[r][c] = SIGHT;
            }
        }
    }
    // 좌
    else if (dir == 2) {
        for (int i = 1; medusa.c + dc[dir] * i >= 0; i++) {
            int c = medusa.c + dc[dir] * i;
            for (int r = medusa.r - i; r <= medusa.r + i; r++) {
                if (r < 0) continue;
                if (r >= N) continue;
                if (sight[r][c] == HIDE) continue;

                if (sdMap[r][c].size()) {
                    sight[r][c] = STONE;
                    meetSoldier(r, c, dir);
                    cnt += sdMap[r][c].size();
                    continue;
                }

                sight[r][c] = SIGHT;
            }
        }
    }
    // 우
    else {
        for (int i = 1; medusa.c + dc[dir] * i < N; i++) {
            int c = medusa.c + dc[dir] * i;
            for (int r = medusa.r - i; r <= medusa.r + i; r++) {
                if (r < 0) continue;
                if (r >= N) continue;
                if (sight[r][c] == HIDE) continue;

                if (sdMap[r][c].size()) {
                    sight[r][c] = STONE;
                    meetSoldier(r, c, dir);
                    cnt += sdMap[r][c].size();
                    continue;
                }

                sight[r][c] = SIGHT;
            }
        }
    }

    return cnt;
}
void findSight() {
    int maxx = 0, dir = 0;

    for (int i = 0; i < 4; i++) {
        int cnt = medusaSee(i);
        if (maxx < cnt) {
            maxx = cnt;
            dir = i;
        }
    }

    medusaSee(dir);
    stone += maxx;

    // 돌로 변한 전사
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (sight[i][j] == STONE) {
                for (int k = 0; k < sdMap[i][j].size(); k++) {
                    int idx = sdMap[i][j][k];
                    soldiers[idx].stone = true;
                }
            }
        }
    }
}
int getDistance(pos a, pos b) {
    int dist = 0;
    if (a.r > b.r) {
        dist += a.r - b.r;
    }
    else {
        dist += b.r - a.r;
    }

    if (a.c > b.c) {
        dist += a.c - b.c;
    }
    else {
        dist += b.c - a.c;
    }

    return dist;
}
int soldierMoveCheck(soldier s) {
    int curDist = getDistance(medusa, {s.r, s.c});
    int dir = 5;

    for (int i = 0; i < 4; i++) {
        int nr = s.r + dr[i];
        int nc = s.c + dc[i];

        if (outOfBound(nr, nc) == false) continue;
        int newDist = getDistance(medusa, { nr,nc });
        if (curDist <= newDist) continue;
        if (sight[nr][nc] == SIGHT || sight[nr][nc] == STONE) continue;

        dir = i;
        break;
    }

    return dir;
}
void soldiersMove() {
    for (int i = 0; i < M; i++) {
        if (soldiers[i].die) continue;
        if (soldiers[i].stone) continue;

        // sdMap에서 삭제
        int r = soldiers[i].r, c = soldiers[i].c;
        for (int j = 0; j < sdMap[r][c].size(); j++) {
            if (sdMap[r][c][j] == i) {
                sdMap[r][c].erase(sdMap[r][c].begin() + j);
                break;
            }
        }

        // 이동 가능한지 확인
        int dir = soldierMoveCheck(soldiers[i]);
        if (dir == 5) { // 이동 불가
            sdMap[r][c].push_back(i);
            continue;
        }
        soldiers[i].r += dr[dir];
        soldiers[i].c += dc[dir];
        r += dr[dir];
        c += dc[dir];
        dist++;

        // 공격
        if (soldiers[i].r == medusa.r && soldiers[i].c == medusa.c) {
            soldiers[i].die = true;
            attack++;
            continue;
        }

        dir = soldierMoveCheck(soldiers[i]);
        if (dir == 5) { // 이동 불가
            sdMap[r][c].push_back(i);
            continue;
        }
        soldiers[i].r += dr[dir];
        soldiers[i].c += dc[dir];
        r += dr[dir];
        c += dc[dir];
        dist++;

        // 공격
        if (soldiers[i].r == medusa.r && soldiers[i].c == medusa.c) {
            soldiers[i].die = true;
            attack++;
            continue;
        }

        sdMap[r][c].push_back(i);
    }
}
void solution() {
    // 집 - 공원 거리 계산
    findRoute();
    if (visit[park.r][park.c] == 0) {
        cout << -1;
        return;
    }

    while (1) {
        init();

        // 메두사 이동
        medusaMove();
        if (medusa.r == park.r && medusa.c == park.c) {
            cout << 0;
            break;
        }

        // 메두사 시선
        findSight();

        // 전사 이동
        soldiersMove();
        
        // 출력
        cout << dist << " " << stone << " " << attack << "\n";
    }
}
void input() {
    cin >> N >> M;
    cin >> home.r >> home.c >> park.r >> park.c;
    medusa = home;

    int r, c;
    for (int i = 0; i < M; i++) {
        cin >> r >> c;
        soldiers.push_back({ r,c,false,false });
        sdMap[r][c].push_back(i);
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> map[i][j];
        }
    }
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    input();
    solution();

    return 0;
}