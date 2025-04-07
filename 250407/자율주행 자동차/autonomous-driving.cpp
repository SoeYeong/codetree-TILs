#include <iostream>

#define MAX 51
#define ROAD 0
#define WALK 1
#define VISIT 2
using namespace std;

int N, M;
int map[MAX][MAX];
int dx[4] = { -1,0,1,0 };   // N, E, S, W
int dy[4] = { 0,1,0,-1 };
int cx, cy, cd, cnt = 1;

void changeDir(int& d) {
    d = (d + 3) % 4;
}
bool move() {
    bool possible = true;
    int temp = 0;
    int nowd = cd;

    while (1) {
        changeDir(cd);
        temp++;

        if (map[cx + dx[cd]][cy + dy[cd]] == ROAD) {
            break;
        }

        if (temp > 4) {
            possible = false;
            cd = nowd;
            break;
        }
    }

    return possible;
}
void moveForward() {
    cx += dx[cd];
    cy += dy[cd];
    map[cx][cy] = VISIT;
    cnt++;
}
bool moveBack() {
    bool possible = true;
    int tempx = cx - dx[cd];
    int tempy = cy - dy[cd];

    if (map[tempx][tempy] == WALK) {
        possible = false;
    }
    else if (map[tempx][tempy] == ROAD) {
        cx = tempx;
        cy = tempy;
        map[cx][cy] = VISIT;
        cnt++;
    }
    else {
        cx = tempx;
        cy = tempy;
    }

    return possible;
}
void solve() {
    bool flag;
    while (1) {
        flag = move();

        if (flag) {
            moveForward();
            continue;
        }
        else {
            flag = moveBack();
        }

        if (!flag) {
            break;
        }
    }

    cout << cnt;
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    // input
    cin >> N >> M;
    cin >> cx >> cy >> cd;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            cin >> map[i][j];
        }
    }
    map[cx][cy] = VISIT;

    solve();

    return 0;
}