#include <iostream>
#include <vector>

#define MAX(X, Y) (X > Y) ? X : Y
using namespace std;

struct info {
    int x, y, dir;
    bool dead = false;
};

int map[4][4];
int dx[8] = { -1,-1,0,1,1,1,0,-1 };
int dy[8] = { 0,-1,-1,-1,0,1,1,1 };
info sullae;
vector<info> thief(17);
int score, ans;

bool thiefMoveCheck(int idx) {
    int d = thief[idx].dir;
    bool flag = true;

    while (1) {
        d %= 8;
        int nx = thief[idx].x + dx[d];
        int ny = thief[idx].y + dy[d];

        if (d == thief[idx].dir && !flag) {
            break;
        }

        if (nx < 0 || nx > 3 || ny < 0 || ny > 3) {
            d++;
            flag = false;
            continue;
        }
        if (nx == sullae.x && ny == sullae.y) {
            d++;
            flag = false;
            continue;
        }

        break;
    }

    // 움직일 수 없는 경우
    if (d == thief[idx].dir && !flag) {
        return false;
    }

    // 움직일 수 있는 경우
    thief[idx].dir = d;
    return true;
}
void thiefMove() {
    for (int i = 1; i <= 16; i++) {
        if (thief[i].dead) continue;

        bool flag = thiefMoveCheck(i);
        // 움직일 수 없는 경우
        if (!flag) continue; 

        // 움직일 수 있는 경우
        int nx = thief[i].x + dx[thief[i].dir];
        int ny = thief[i].y + dy[thief[i].dir];
        
        // 움직인 곳이 빈칸
        if (map[nx][ny] == 0) {
            map[thief[i].x][thief[i].y] = 0;
            thief[i].x = nx;
            thief[i].y = ny;
            map[thief[i].x][thief[i].y] = i;
        }
        // 다른 말이 있는 경우
        else {
            int temp = map[nx][ny];
            thief[temp].x = thief[i].x;
            thief[temp].y = thief[i].y;
            map[thief[temp].x][thief[temp].y] = temp;

            map[nx][ny] = i;
            thief[i].x = nx;
            thief[i].y = ny;
        }
    }
}
void catchThief(int x, int y) {
    int idx = map[x][y];
    score += idx;
    sullae = { x, y, thief[idx].dir, false };
    thief[idx].dead = true;
    map[x][y] = 0;
}
void backtracking() {
    int tempMap[4][4];

    // 도둑말 이동
    thiefMove();

    // 임시저장
    info tempSullae = sullae;
    vector<info> tempThief(thief);
    int tempScore = score;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            tempMap[i][j] = map[i][j];
        }
    }

    // 술래 이동
    for (int i = 1; i <= 3; i++) {
        int nx = sullae.x + dx[sullae.dir] * i;
        int ny = sullae.y + dy[sullae.dir] * i;

        if (nx < 0 || nx > 3 || ny < 0 || ny > 3) {
            ans = MAX(ans, score);
            continue;
        }
        if (map[nx][ny] == 0) {
            ans = MAX(ans, score);
            continue;
        }
        catchThief(nx, ny);
        backtracking();

        // 복구
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                map[i][j] = tempMap[i][j];
            }
        }
        sullae = tempSullae;
        score = tempScore;
        thief = tempThief;
    }
}
void solution() {
    catchThief(0, 0);
    backtracking();
    cout << ans;
}
void input() {
    int idx, dir;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            cin >> idx >> dir;
            map[i][j] = idx;
            dir--;
            thief[idx] = { i, j, dir, false };
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