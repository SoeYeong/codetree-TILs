#include <iostream>
#include <vector>
#include <cstdlib>
using namespace std;

struct info {
    int r, c, d;
    int power, gun = 0;
    int score = 0;
};

int N, M, K;
vector<int> gunMap[21][21]; // 총 위치 정보
int map[21][21];            // 플레이어 위치
info player[31];
int dx[4] = { -1,0,1,0 };
int dy[4] = { 0,1,0,-1 };

void chooseGun(int idx) {
    int r = player[idx].r;
    int c = player[idx].c;

    // 총 없는 경우
    if (gunMap[r][c].size() == 0) {
        return;
    }

    // 총 있는 경우
    int maxx = player[idx].gun;
    int max_i = 0;
    for (int i = 0; i < gunMap[r][c].size(); i++) {
        if (maxx < gunMap[r][c][i]) {
            maxx = gunMap[r][c][i];
            max_i = i;
        }
    }

    if (maxx > player[idx].gun) {
        if (player[idx].gun) {
            gunMap[r][c].push_back(player[idx].gun);
        }
        player[idx].gun = maxx;
        gunMap[r][c].erase(gunMap[r][c].begin() + max_i);
    }

}
void fight(int n1, int n2) {
    int sum1 = player[n1].power + player[n1].gun;
    int sum2 = player[n2].power + player[n2].gun;
    int r = player[n1].r, c = player[n1].c;
    int win, lose;

    // winner 찾기
    if (sum1 > sum2) {
        win = n1, lose = n2;
    }
    else if (sum1 == sum2) {
        if (player[n1].power > player[n2].power) {
            win = n1, lose = n2;
        }
        else {
            win = n2, lose = n1;
        }
    }
    else {
        win = n2, lose = n1;
    }
    player[win].score += abs(sum1 - sum2);
    map[r][c] = win;

    // 진 사람
    if (player[lose].gun) {
        gunMap[r][c].push_back(player[lose].gun);
        player[lose].gun = 0;
    }

    int nr, nc;
    while (1) {
        nr = r + dx[player[lose].d];
        nc = c + dy[player[lose].d];

        if (nr < 1 || nr > N || nc < 1 || nc > N || map[nr][nc]) {
            player[lose].d++;
            player[lose].d %= 4;
            continue;
        }
        break;
    }
    player[lose].r = nr;
    player[lose].c = nc;
    chooseGun(lose);

    // 이긴 사람
    chooseGun(win);
}
void move() {
    for (int i = 1; i <= M; i++) {
        int nr = player[i].r + dx[player[i].d];
        int nc = player[i].c + dy[player[i].d];

        // 격자 벗어날 경우 방향 바꾸기
        if (nr < 1 || nr > N || nc < 1 || nc > N) {
            player[i].d += 2;
            player[i].d %= 4;
        }

        // 한칸 이동
        map[player[i].r][player[i].c] = 0;
        player[i].r += dx[player[i].d];
        player[i].c += dy[player[i].d];

        // 플레이어 없는 경우
        if (map[player[i].r][player[i].c] == 0) {
            map[player[i].r][player[i].c] = i;

            // 총 선택
            chooseGun(i);

            continue;
        }

        // 플레이어 있는 경우
        int n1 = map[player[i].r][player[i].c];
        int n2 = i;
        fight(n1, n2);
    }
}
void printScore() {
    for (int i = 1; i <= M; i++) {
        cout << player[i].score << " ";
    }
}
void solve() {
    while (K--) {
        move();
    }

    printScore();
}
void input() {
    int n;

    cin >> N >> M >> K;
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            cin >> n;
            if (n) {
                gunMap[i][j].push_back(n);
            }
        }
    }

    for (int i = 1; i <= M; i++) {
        cin >> player[i].r >> player[i].c >> player[i].d >> player[i].power;
        map[player[i].r][player[i].c] = i;
    }
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    input();
    solve();

    return 0;
}