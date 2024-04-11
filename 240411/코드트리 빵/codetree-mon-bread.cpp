#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int N, M;
struct pos {
    int r, c;
    bool pass;  // 지나갈 수 있는지
};
struct person {
    int r = 0, c = 0;
    bool in = false;    // 격자 안에 있는지
    bool mart = false;    // 편의점 도착 여부
};

int map[15][15];
vector<pos> mart;
vector<pos> camp;
person p[226];
int dx[4] = { -1,0,0,1 };   // 위, 왼, 오, 아래
int dy[4] = { 0,-1,1,0 };

struct info {
    int idx, d, r, c;
};
struct compare {
    bool operator ()(info& a, info& b) {
        if (a.d != b.d) return a.d > b.d;
        if (a.r != b.r) return a.r > b.r;
        if (a.c != b.c) return a.c > b.c;
    }
};

void findRoute(int r, int c, int idx) {
    pair<int, int> route[15][15];

    queue<pair<int, int>> q;
    q.push({ r, c });

    int visit[15][15];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            visit[i][j] = 0;
        }
    }
    visit[r][c] = 1;

    while (!q.empty()) {
        pair<int, int> now = q.front();
        q.pop();

        if (now.first == mart[idx].r && now.second == mart[idx].c) {
            break;
        }

        for (int i = 0; i < 4; i++) {
            int nr = now.first + dx[i];
            int nc = now.second + dy[i];

            if (nr < 0 || nr >= N || nc < 0 || nc >= N) continue;
            if (visit[nr][nc]) continue;
            if (map[nr][nc] == 500) continue;

            visit[nr][nc] = visit[now.first][now.second] + 1;
            q.push({ nr, nc });
            route[nr][nc] = { now.first, now.second };
        }
    }

    // 최단거리 방향으로 한 칸 움직이기
    pair<int, int> now = { mart[idx].r, mart[idx].c };

    while (1) {
        pair<int, int> next = route[now.first][now.second];
        if (next.first == r && next.second == c) {
            break;
        }
        now = next;
    }

    p[idx].r = now.first;
    p[idx].c = now.second;
}
void gotoMart() {
    for (int i = 1; i <= M; i++) {
        // 격자 안에 없거나 편의점에 도착한 경우
        if (!p[i].in) continue;
        if (p[i].mart) continue;

        // 최단거리 찾기 (루트 저장)
        findRoute(p[i].r, p[i].c, i);

        // 편의점에 도착한 경우
        if (p[i].r == mart[i].r && p[i].c == mart[i].c) {
            p[i].mart = true;
            mart[i].pass = false;
        }
    }
}
void gotoCamp(int t) {
    priority_queue<info, vector<info>, compare> pq;
    queue<pair<int, int>> q;
    q.push({ mart[t].r, mart[t].c });

    int visit[15][15];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            visit[i][j] = 0;
        }
    }
    visit[mart[t].r][mart[t].c] = 1;

    while (!q.empty()) {
        pair<int, int> now = q.front();
        q.pop();

        for (int i = 0; i < 4; i++) {
            int nr = now.first + dx[i];
            int nc = now.second + dy[i];

            if (nr < 0 || nr >= N || nc < 0 || nc >= N) continue;
            if (map[nr][nc] == 500) continue;
            if (visit[nr][nc]) continue;

            visit[nr][nc] = visit[now.first][now.second] + 1;
            if (map[nr][nc]) {
                pq.push({ map[nr][nc], visit[nr][nc], nr, nc });
            }
            q.push({ nr,nc });
        }
    }

    int idx = pq.top().idx;
    p[t].r = camp[idx].r;
    p[t].c = camp[idx].c;
    p[t].in = true;
    camp[idx].pass = false;
}
void cannotPass() {
    for (int i = 1; i < mart.size(); i++) {
        if (mart[i].pass) continue;
        map[mart[i].r][mart[i].c] = 500;
    }
    for (int i = 1; i < camp.size(); i++) {
        if (camp[i].pass) continue;
        map[camp[i].r][camp[i].c] = 500;
    }
}
bool check() {
    for (int i = 1; i <= M; i++) {
        if (!p[i].mart) {
            return false;
        }
    }
    return true;
}
void solve() {
    int t = 1;
    while (1) {
        gotoMart();
        if (t <= M) {
            gotoCamp(t);
        }
        cannotPass();

        if (check()) {
            break;
        }
        t++;
    }

    cout << t;
}
void input() {
    int r, c, n = 1;

    // 0번 인덱스 처리
    camp.push_back({ 0,0,0 });
    mart.push_back({ 0,0,0 });

    // 입력
    cin >> N >> M;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> map[i][j];

            if (map[i][j]) {
                map[i][j] = n;
                n++;
                camp.push_back({ i,j,true });
            }
        }
    }

    for (int i = 0; i < M; i++) {
        cin >> r >> c;
        mart.push_back({ r - 1,c - 1,true });
    }
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    input();
    solve();

    return 0;
}