#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

#define MAX 21
#define NONE -2
#define STONE -1
#define RED 0
using namespace std;

struct bomb {
    int r, c;
    int isRed;
};

int N, M;
int score = 0;
int map[MAX][MAX];
int visit[MAX][MAX];
int dr[4] = { 0,0,-1,1 };
int dc[4] = { -1,1,0,0 };
// <폭탄 정보, 빨간 폭탄 개수>
vector<pair<vector<bomb>, int>> bombs;  

bool cmp1(bomb a, bomb b) {
    if (a.isRed != b.isRed) {
        return a.isRed < b.isRed;
    }
    if (a.r != b.r) {
        return a.r > b.r;
    }
    return a.c < b.c;
}
bool cmp2(pair<vector<bomb>, int> a, pair<vector<bomb>, int> b) {
    if (a.first.size() != b.first.size()) {
        return a.first.size() > b.first.size();
    }
    if (a.second != b.second) {
        return a.second < b.second;
    }
    if (a.first[0].r != b.first[0].r) {
        return a.first[0].r > b.first[0].r;
    }
    return a.first[0].c < b.first[0].c;
}

void bfs(int r, int c) {
    queue<pair<int, int>> q;
    vector<bomb> v;
    vector<bomb> red;
    int color = map[r][c];

    q.push({ r,c });
    v.push_back({ r,c,0 });
    visit[r][c] = 1;

    while (!q.empty()) {
        int nowr = q.front().first;
        int nowc = q.front().second;
        q.pop();

        for (int i = 0; i < 4; i++) {
            int nextr = nowr + dr[i];
            int nextc = nowc + dc[i];

            if (nextr < 0 || nextr >= N || nextc < 0 || nextc >= N) continue;
            if (map[nextr][nextc] == RED && visit[nextr][nextc] == 0) {
                visit[nextr][nextc] = 1;
                q.push({ nextr,nextc });
                v.push_back({ nextr,nextc,1 });
                red.push_back({ nextr,nextc,1 });
            }
            if (map[nextr][nextc] != color) continue;
            if (visit[nextr][nextc]) continue;

            visit[nextr][nextc] = 1;
            q.push({ nextr,nextc });
            v.push_back({ nextr,nextc,0 });
        }
    }

    // 빨간 폭탄 비방문 처리
    for (int i = 0; i < red.size(); i++) {
        int rr = red[i].r;
        int rc = red[i].c;
        visit[rr][rc] = 0;
    }

    // 폭탄 묶음 개수 확인
    if (v.size() < 2) {
        return;
    }
    
    sort(v.begin(), v.end(), cmp1);
    bombs.push_back({ v, red.size()});
}
void find() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            visit[i][j] = 0;
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (map[i][j] == NONE || map[i][j] == STONE || map[i][j] == RED) continue;
            if (visit[i][j]) continue;
            bfs(i, j);
        }
    }
}
void remove() {
    int C = bombs[0].first.size();
    score += C * C;

    for (int i = 0; i < bombs[0].first.size(); i++) {
        map[bombs[0].first[i].r][bombs[0].first[i].c] = NONE;
    }
}
void gravity() {
    for (int i = N - 2; i >= 0; i--) {
        for (int j = 0; j < N; j++) {
            if (map[i][j] == NONE || map[i][j] == STONE) continue;

            for (int k = i + 1; k < N; k++) {
                if (map[k][j] == NONE) {
                    map[k][j] = map[k - 1][j];
                    map[k - 1][j] = NONE;
                }
                else
                    break;
            }
        }
    }
}
void rotate() {
    int temp[MAX][MAX];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            temp[i][j] = map[i][j];
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            map[i][j] = temp[j][N - 1 - i];
        }
    }
}
void solution() {
    while (1) {
        // find bombs
        bombs.clear();
        find();

        if (bombs.size() == 0)
            break;

        sort(bombs.begin(), bombs.end(), cmp2);

        // remove bombs
        remove();
        gravity();
        
        // rotate 90 degree
        rotate();
        gravity();
    }

    cout << score;
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    // input
    cin >> N >> M;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> map[i][j];
        }
    }

    solution();

    return 0;
}