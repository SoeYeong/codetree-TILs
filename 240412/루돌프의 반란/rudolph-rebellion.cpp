#include <iostream>
#include <cmath>
#include <queue>

#define INF 10000
using namespace std;

struct Santa {
    int r = 0, c = 0;
    int score = 0;
    int kijul = -1;
    bool in = false;
};
struct info {
    int dis, r, c;
};
struct compare {
    bool operator()(info& a, info& b) {
        if (a.dis != b.dis) return a.dis > b.dis;
        if (a.r != b.r) return a.r < b.r;
        return a.c < b.c;
    }
};

int N, M, P, C, D;
int Rr, Rc; // 루돌프 위치
Santa santa[31];
int map[51][51];    // 산타 위치 표시
int dr[8] = { -1,0,1,0, -1,-1,1,1 };    // 상, 우, 하, 좌, 대각선
int dc[8] = { 0,1,0,-1, -1,1,-1,1 };
int r_dir, s_dir;   // 루돌프, 산타 이동 방향

void printMap(int turn) {
    cout << "\nturn : " << turn;
    cout << "\n----------------\n";
    cout << "Rudolf : " << Rr << " " << Rc << "\n";
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            cout << map[i][j] << " ";
        }
        cout << "\n";
    }
    for (int i = 1; i <= P; i++) {
        cout << i << " - (r, c) : " << santa[i].r << ", " << santa[i].c << " ";
        cout << "score : " << santa[i].score << ", in : " << santa[i].in << "\n";
    }
}
void interact(int idx, int dir) {
    int nidx = map[santa[idx].r][santa[idx].c];
    map[santa[idx].r][santa[idx].c] = idx;

    int nr = santa[idx].r + dr[dir];
    int nc = santa[idx].c + dc[dir];

    santa[nidx].r = nr;
    santa[nidx].c = nc;

    if (nr < 1 || nr > N || nc < 1 || nc > N) {
        santa[nidx].in = false;
        return;
    }

    if (map[nr][nc] == 0) {
        map[nr][nc] = nidx;
        return;
    }
    interact(nidx, dir);
}
void crush(int idx, bool flag) {
    int dir = flag ? r_dir : ((s_dir + 2) % 4);
    int cnt = flag ? C : D;

    int nr = santa[idx].r + cnt * dr[dir];
    int nc = santa[idx].c + cnt * dc[dir];

    map[santa[idx].r][santa[idx].c] = 0;
    santa[idx].r = nr;
    santa[idx].c = nc;

    // 범위 벗어날 경우
    if (nr < 1 || nr > N || nc < 1 || nc > N) {
        santa[idx].in = false;
        return;
    }

    // 밀려난 칸에 산타 없는 경우
    if (map[nr][nc] == 0) {
        map[nr][nc] = idx;
        return;
    }

    // 밀려난 칸에 산타 있는 경우
    interact(idx, dir);
}
void moveRudolf(int turn) {
    int sr = 0, sc = 0, dis = INF;
    priority_queue<info, vector<info>, compare> pq;

    // 가장 가까운 산타 선택
    for (int i = 1; i <= P; i++) {
        if (!santa[i].in) continue;
        int temp = pow(Rr - santa[i].r, 2) + pow(Rc - santa[i].c, 2);
        pq.push({ temp, santa[i].r, santa[i].c });
    }
    sr = pq.top().r;
    sc = pq.top().c;
    dis = pq.top().dis;

    // 1칸 돌진
    for (int i = 0; i < 8; i++) {
        int nr = Rr + dr[i];
        int nc = Rc + dc[i];

        if (nr < 1 || nr > N || nc < 1 || nc > N) continue;
        
        int temp = pow(nr - sr, 2) + pow(nc - sc, 2);
        if (temp >= dis) continue;
        if (temp < dis) {
            dis = temp;
            r_dir = i;
        }
    }
    Rr += dr[r_dir];
    Rc += dc[r_dir];

    // 움직인 칸에 산타 있는 경우
    if (map[Rr][Rc]) {
        int idx = map[Rr][Rc];
        santa[idx].score += C;
        santa[idx].kijul = turn;
        crush(idx, true);
    }
}
void moveSanta(int turn) {
    for (int i = 1; i <= P; i++) {
        if (!santa[i].in) continue;
        if (turn - santa[i].kijul < 2) continue;

        bool flag = false;
        int dis = pow(Rr - santa[i].r, 2) + pow(Rc - santa[i].c, 2);
        for (int d = 0; d < 4; d++) {
            int nr = santa[i].r + dr[d];
            int nc = santa[i].c + dc[d];

            if (nr < 1 || nr > N || nc < 1 || nc > N) continue;
            if (map[nr][nc]) continue;

            int temp = pow(Rr - nr, 2) + pow(Rc - nc, 2);
            if (temp >= dis) continue;
            if (temp < dis) {
                dis = temp;
                s_dir = d;
                flag = true;
            }
        }

        // 움직일 수 있는 경우
        if (flag) {
            map[santa[i].r][santa[i].c] = 0;
            santa[i].r += dr[s_dir], santa[i].c += dc[s_dir];
            map[santa[i].r][santa[i].c] = i;
        }
        
        // 움직인 칸에 루돌프 있는 경우
        if (santa[i].r == Rr && santa[i].c == Rc) {
            santa[i].score += D;
            santa[i].kijul = turn;

            crush(i, false);
        }
    }
}
bool isFinish() {
    // 한명이라도 살아있으면 false
    for (int i = 1; i <= P; i++) {
        if (santa[i].in) {
            return false;
        }
    }
    return true;
}
void plusOne() {
    for (int i = 1; i <= P; i++) {
        if (!santa[i].in) continue;
        santa[i].score += 1;
    }
}
void printScore() {
    for (int i = 1; i <= P; i++) {
        cout << santa[i].score << " ";
    }
}
void solve() {
    for (int turn = 1; turn <= M; turn++) {
        moveRudolf(turn);
        moveSanta(turn);
        
        // 모두 탈락했는지
        if (isFinish()) {
            break;
        }

        // 1점 추가
        plusOne();
    }

    printScore();
}
void input() {
    cin >> N >> M >> P >> C >> D;
    cin >> Rr >> Rc;

    int n, r, c;
    for (int i = 1; i <= P; i++) {
        cin >> n >> r >> c;
        santa[n] = { r, c, 0, -1, true };
        map[r][c] = n;
    }
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    input();
    solve();

    return 0;
}