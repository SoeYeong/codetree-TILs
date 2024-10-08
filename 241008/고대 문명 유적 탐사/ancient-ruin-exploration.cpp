#include <iostream>
#include <cstring>
#include <queue>
#include <vector>
#include <algorithm>
using namespace std;

int K, M;
int map[5][5];
int dy[4] = { 0,0,-1,1 };
int dx[4] = { -1,1,0,0 };
queue<int> piece;
int ans;
struct info {
    int r, c, angle, value;
};
vector<info> v;

bool cmp(info A, info B) {
    if (A.value == B.value) {
        if (A.angle == B.angle) {
            if (A.c == B.c) 
                return A.r < B.r;
            return A.c < B.c;
        }
        return A.angle < B.angle;
    }
    return A.value > B.value;
}
void rotate(int y, int x) {
    int tempm[3][3];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            tempm[i][j] = map[y + i][x + j];
        }
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            map[y + i][x + j] = tempm[2 - j][i];
        }
    }
}
int bfs() {
    bool visit[5][5];
    memset(visit, 0, sizeof(visit));

    int sum = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (visit[i][j]) continue;

            queue<pair<int, int>> q;
            q.push({ i,j });
            visit[i][j] = true;
            int cnt = 1;

            while (!q.empty()) {
                int nowy = q.front().first;
                int nowx = q.front().second;
                q.pop();

                for (int k = 0; k < 4; k++) {
                    int ny = nowy + dy[k];
                    int nx = nowx + dx[k];

                    if (ny < 0 || ny>4 || nx < 0 || nx>4) continue;
                    if (visit[ny][nx] || map[i][j] != map[ny][nx]) continue;
                    cnt++;
                    visit[ny][nx] = true;
                    q.push({ ny,nx });
                }
            }

            if (cnt >= 3) sum += cnt;
        }
    }
    return sum;
}
int getPiece() {
    bool visit[5][5];
    memset(visit, 0, sizeof(visit));
    vector<pair<int, int>> tempv;
    int sum = 0;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (visit[i][j]) continue;

            queue<pair<int, int>> q;
            q.push({ i,j });
            tempv.push_back({ i,j });
            visit[i][j] = true;

            while (!q.empty()) {
                int nowy = q.front().first;
                int nowx = q.front().second;
                q.pop();

                for (int k = 0; k < 4; k++) {
                    int ny = nowy + dy[k];
                    int nx = nowx + dx[k];

                    if (ny < 0 || ny > 4 || nx < 0 || nx > 4) continue;
                    if (visit[ny][nx] || map[i][j] != map[ny][nx]) continue;
                    visit[ny][nx] = true;
                    q.push({ ny,nx });
                    tempv.push_back({ ny,nx });
                }
            }

            if (tempv.size() >= 3) {
                sum += tempv.size();
                for (int k = 0; k < tempv.size(); k++) {
                    map[tempv[k].first][tempv[k].second] = 0;
                }
            }
            tempv.clear();
        }
    }
    return sum;
}
void find(int y, int x) {
    int tempm[3][3];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            tempm[i][j] = map[y + i][x + j];
        }
    }

    rotate(y, x);
    int n = bfs();
    v.push_back({ y,x,1,n });

    rotate(y, x);
    n = bfs();
    v.push_back({ y,x,2,n });

    rotate(y, x);
    n = bfs();
    v.push_back({ y,x,3,n });

    // 원래대로
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            map[y + i][x + j] = tempm[i][j];
        }
    }
}
bool findMax() {
    bool flag = false;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            find(i, j);
        }
    }
    sort(v.begin(), v.end(), cmp);

    int y = v.front().r;
    int x = v.front().c;
    for (int i = 0; i < v.front().angle; i++) {
        rotate(y, x);
    }
    ans += v.front().value;
    getPiece();
    v.clear();

    if (ans) flag = true;
    return flag;
}
void fillMap() {
    for (int i = 0; i < 5; i++) {
        for (int j = 4; j >= 0; j--) {
            if (map[j][i] > 0) continue;
            map[j][i] = piece.front();
            piece.pop();
        }
    }
}
void solution() {
    while (K--) {
        bool flag = findMax();
        if (!flag) break;

        fillMap();

        int n = bfs();
        while (n > 0) {
            ans += n;
            getPiece();
            fillMap();
            n = bfs();
        }
        if (ans > 0) {
            cout << ans << " ";
            ans = 0;
        }
        else {
            break;
        }
    }
}
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);
    
    cin >> K >> M;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            cin >> map[i][j];
        }
    }
    int temp;
    for (int i = 0; i < M; i++) {
        cin >> temp;
        piece.push(temp);
    }

    solution();
    return 0;
}