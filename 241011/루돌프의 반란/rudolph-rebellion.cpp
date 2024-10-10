#include <iostream>
#include <cmath>
#include <vector>
#include <queue>
using namespace std;

int N, M, P, C, D;
int rr, rc; // 루돌프 위치
int turn, dir;
int map[51][51];
int dr[8] = { -1,0,1,0, -1,-1,1,1 };    // 상,우,하,좌,대각선
int dc[8] = { 0,1,0,-1, -1,1,-1,1 };
struct santaInfo {
    int r, c;
    int down = -1;
    bool out = false;
    int score = 0;
};
santaInfo santa[31];

struct distInfo {
    int r, c;
    int dist;
};
struct cmp {
    bool operator()(distInfo a, distInfo b) {
        if (a.dist != b.dist) return a.dist > b.dist;
        if (a.r != b.r) return a.r < b.r;
        return a.c < b.c;
    }
};

void input() {
    cin >> N >> M >> P >> C >> D;
    cin >> rr >> rc;
    int n;
    for (int i = 1; i <= P; i++) {
        cin >> n;
        cin >> santa[n].r >> santa[n].c;
        map[santa[n].r][santa[n].c] = n;
    }
}
void plus1() {
    for (int i = 1; i <= P; i++) {
        if (santa[i].out) continue;
        santa[i].score += 1;
    }
}
bool isFinish() {
    for (int i = 1; i <= P; i++) {
        if (!santa[i].out) return false;
    }
    return true;
}
void interact(int idx) {
    while (1) {
        int nr = santa[idx].r;
        int nc = santa[idx].c;

        // 범위 벗어난 경우
        if (nr < 1 || nr > N || nc < 1 || nc > N) {
            santa[idx].out = true;
            return;
        }

        // 다른 산타 없는 경우
        if (map[nr][nc] == 0) {
            map[nr][nc] = idx;
            return;
        }

        // 다른 산타 있는 경우
        int next = map[nr][nc];
        map[nr][nc] = idx;
        idx = next;
        
        santa[idx].r += dr[dir];
        santa[idx].c += dc[dir];
    }
}
void crush(int idx, bool flag) {
    int num = flag ? C : D;

    santa[idx].score += num;
    santa[idx].down = turn;

    map[santa[idx].r][santa[idx].c] = 0;
    santa[idx].r += dr[dir] * num;
    santa[idx].c += dc[dir] * num;

    interact(idx);
}
void moveRudolph() {
    priority_queue<distInfo, vector<distInfo>, cmp> pq;
    int sr, sc, d;

    for (int i = 1; i <= P; i++) {
        if (santa[i].out) continue;
        int diff1 = rr - santa[i].r;
        int diff2 = rc - santa[i].c;
        int dist = diff1 * diff1 + diff2 * diff2;
        pq.push({ santa[i].r, santa[i].c, dist });
    }

    // 한 칸 이동
    sr = pq.top().r;
    sc = pq.top().c;
    d = pq.top().dist;
    for (int i = 0; i < 8; i++) {
        int nr = rr + dr[i];
        int nc = rc + dc[i];

        if (nr < 1 || nr > N || nc < 1 || nc > N) continue;
        int temp = pow(nr - sr, 2) + pow(nc - sc, 2);
        if (temp < d) {
            d = temp;
            dir = i;
        }
    }
    rr += dr[dir];
    rc += dc[dir];

    // 움직인 곳에 산타 있는 경우
    if (map[rr][rc]) {
        int idx = map[rr][rc];
        crush(idx, true);
    }
}
void moveSanta() {
    for (int i = 1; i <= P; i++) {
        if (santa[i].out) continue;
        if (turn - santa[i].down < 2) continue;

        int dis = pow(rr - santa[i].r, 2) + pow(rc - santa[i].c, 2);
        dir = -1;
        for (int j = 0; j < 4; j++) {
            int nr = santa[i].r + dr[j];
            int nc = santa[i].c + dc[j];

            if (nr < 1 || nr > N || nc < 1 || nc > N) continue;
            if (map[nr][nc]) continue;
            if (pow(rr - nr, 2) + pow(rc - nc, 2) < dis) {
                dis = pow(rr - nr, 2) + pow(rc - nc, 2);
                dir = j;
            }
        }

        // 움직일 수 있는 경우
        if (dir >= 0) {
            map[santa[i].r][santa[i].c] = 0;
            santa[i].r += dr[dir];
            santa[i].c += dc[dir];
            map[santa[i].r][santa[i].c] = i;
        }

        // 루돌프 있는 경우
        if (santa[i].r == rr && santa[i].c == rc) {
            dir += 2;
            dir %= 4;
            crush(i, false);
        }
    }
}
void solution() {
    for (int i = 1; i <= M; i++) {
        turn = i;
        moveRudolph();
        moveSanta();

        if (isFinish()) break;

        plus1();
    }

    for (int i = 1; i <= P; i++) {
        cout << santa[i].score << " ";
    }
}
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    input();
    solution();

    return 0;
}