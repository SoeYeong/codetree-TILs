#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;

struct info {
    int r, c, d;
    bool live;
};

int N, M, H, K, score;
int tree[100][100];
vector<int> map[100][100];
vector<info> player;
int dx[4] = { -1,0,1,0 };
int dy[4] = { 0,1,0,-1 };
int nextd[4] = { 2,3,0,1 };
int Sr, Sc, Sdir; // 술래 위치, 방향

bool changeDir[100][100];

void movePlayer() {
    for (int i = 1; i <= M; i++) {
        if (player[i].live == false) continue;

        int r = player[i].r;
        int c = player[i].c;
        int dis = abs(Sr - r) + abs(Sc - c);
        if (dis > 3) continue;

        int nr = r + dx[player[i].d];
        int nc = c + dy[player[i].d];

        // 격자 벗어난 경우
        if (nr < 1 || nr > N || nc < 1 || nc > N) {
            player[i].d = nextd[player[i].d];
        }
        nr = r + dx[player[i].d];
        nc = c + dy[player[i].d];

        // 움직이려는 칸에 술래 있는 경우
        if (nr == Sr && nc == Sc) continue;

        // 술래 없는 경우
        int j;
        for (j = 0; j < map[r][c].size(); j++) {
            if (map[r][c][j] == i) break;
        }
        map[r][c].erase(map[r][c].begin() + j);
        map[nr][nc].push_back(i);
        player[i].r = nr;
        player[i].c = nc;
    }
}
void moveSullae(int turn) {
    int cnt = 0;    // 잡힌 도망자 수

    // 술래 이동
    Sr += dx[Sdir];
    Sc += dy[Sdir];
    if (Sr == 1 && Sc == 1) {
        Sdir = 2;
    }
    else if (Sr == (N / 2 + 1) && Sc == (N / 2 + 1)) {
        Sdir = 0;
    }
    else if (changeDir[Sr][Sc]) {
        if (turn / (N * N - 1)) {
            Sdir--;
            if (Sdir < 0) {
                Sdir = 3;
            }
        }
        else {
            Sdir++;
            Sdir %= 4;
        }
    }

    // 도망자 잡기
    for (int k = 0; k < 3; k++) {
        int nr = Sr + k * dx[Sdir];
        int nc = Sc + k * dy[Sdir];

        if (nr < 1 || nr > N || nc < 1 || nc > N) continue;
        if (tree[nr][nc]) continue;

        cnt += map[nr][nc].size();
        for (int i = 0; i < map[nr][nc].size(); i++) {
            int idx = map[nr][nc][i];
            player[idx].live = false;
        }
        map[nr][nc].clear();
    }

    score += turn * cnt;
}
void init() {
    Sr = N / 2 + 1, Sc = N / 2 + 1;

    vector<int> dist;
    for (int i = 1; i < N; i++) {
        dist.push_back(i);
        dist.push_back(i);
    }
    dist.push_back(N - 1);

    int r = Sr, c = Sc, d = 0;
    changeDir[r][c] = true;
    for (int i = 0; i < dist.size(); i++) {
        r += dist[i] * dx[d];
        c += dist[i] * dy[d];
        changeDir[r][c] = true;
        d++;
        d %= 4;
    }
}
void solve() {
    init();
    for (int turn = 1; turn <= K; turn++) {
        movePlayer();
        moveSullae(turn);
    }
    cout << score;
}
void input() {
    int x, y, d;
    player.push_back({ 0,0,0,0 });

    cin >> N >> M >> H >> K;
    for (int i = 1; i <= M; i++) {
        cin >> x >> y >> d;
        player.push_back({ x,y,d,true });
        map[x][y].push_back(i);
    }
    for (int i = 0; i < H; i++) {
        cin >> x >> y;
        tree[x][y] = 1;
    }
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    input();
    solve();

    return 0;
}