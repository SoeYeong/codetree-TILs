#include <iostream>
#include <vector>
#include <queue>
#include <string.h>

#define MAX 21
#define WALL 1
using namespace std;

struct info {
    int sx, sy;
    int ex, ey;
    bool done = false;
};
struct info2 {
    int idx, dist;
};

int N, M, C;
bool finish = false, stop = false;
int carx, cary;
int map[MAX][MAX];
int visit[MAX][MAX];
int dx[4] = { 0,0,-1,1 };
int dy[4] = { -1,1,0,0 };
vector<info> customer;

struct cmp {
    bool operator()(info2 a, info2 b) {
        if (a.dist != b.dist) {
            return a.dist > b.dist;
        }
        if (customer[a.idx].sx != customer[b.idx].sx) {
            return customer[a.idx].sx > customer[b.idx].sx;
        }
        return customer[a.idx].sy > customer[b.idx].sy;
    }
};
priority_queue<info2, vector<info2>, cmp> pq;

void bfs(int ex, int ey) {
    memset(visit, 0, sizeof(visit));

    queue<pair<int, int>> q;
    q.push({ carx, cary });
    visit[carx][cary] = 1;

    while (!q.empty()) {
        pair<int, int> now;
        now = q.front();
        q.pop();

        if (now.first == ex && now.second == ey) {
            break;
        }

        for (int i = 0; i < 4; i++) {
            int nextx = now.first + dx[i];
            int nexty = now.second + dy[i];

            if (nextx < 1 || nextx > N || nexty < 1 || nexty > N) continue;
            if (map[nextx][nexty] == WALL) continue;
            if (visit[nextx][nexty] > 0) continue;

            visit[nextx][nexty] = visit[now.first][now.second] + 1;
            q.push({ nextx, nexty });
        }
    }
}
void findCustomer() {
    finish = true;
    bfs(0,0);

    while (!pq.empty()) {
        pq.pop();
    }

    for (int i = 0; i < M; i++) {
        if (customer[i].done) continue;
        finish = false;
        pq.push({ i, visit[customer[i].sx][customer[i].sy] });
    }
}
void move() {
    int idx = pq.top().idx;
    int sx = customer[idx].sx;
    int sy = customer[idx].sy;
    int ex = customer[idx].ex;
    int ey = customer[idx].ey;

    // 승객 거리 확인
    // 못 가는 경우 || 가다가 멈추는 경우
    if (visit[sx][sy] == 0 || C < visit[sx][sy] - 1) {
        stop = true;
        return;
    }

    // 승객 태움
    carx = sx;
    cary = sy;
    C -= visit[sx][sy] - 1;
    bfs(ex, ey);

    // 승객 목적지 거리 확인
    // 못 가는 경우 || 가다가 멈추는 경우
    if (visit[ex][ey] == 0 || C < visit[ex][ey] - 1) {
        stop = true;
        return;
    }

    // 승객 목적지로 이동
    carx = ex;
    cary = ey;
    C += visit[ex][ey] - 1;
    customer[idx].done = true;
}
void solution() {

    while (1) {
        findCustomer();
        if (finish) {
            cout << C;
            return;
        }

        move();
        if (stop) {
            cout << -1;
            return;
        }
    }

}
void input() {
    cin >> N >> M >> C;
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            cin >> map[i][j];
        }
    }
    cin >> carx >> cary;

    info temp;
    for (int i = 0; i < M; i++) {
        cin >> temp.sx >> temp.sy >> temp.ex >> temp.ey;
        customer.push_back(temp);
    }
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    input();
    solution();

    return 0;
}