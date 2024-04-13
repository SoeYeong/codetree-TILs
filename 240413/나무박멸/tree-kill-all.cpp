#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int N, M, K, C, ans;
int map[21][21];
int jecho[21][21];
int dx[8] = { -1,1,0,0, -1,-1,1,1 };
int dy[8] = { 0,0,-1,1, -1,1,-1,1 };

struct info {
    int r, c, cnt;
    vector<pair<int, int>> v;
};
struct compare {
    bool operator()(info& a, info& b) {
        if (a.cnt != b.cnt) return a.cnt < b.cnt;
        if (a.r != b.r) return a.r > b.r;
        return a.c > b.c;
    }
};

void grow() {
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (map[i][j] <= 0) continue;

            int cnt = 0;
            for (int d = 0; d < 4; d++) {
                int nr = i + dx[d];
                int nc = j + dy[d];

                if (nr < 1 || nr > N || nc < 1 || nc > N) continue;
                if (map[nr][nc] <= 0) continue;
                cnt++;
            }
            map[i][j] += cnt;
        }
    }
}
void reproduce() {
    int temp[21][21];
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            temp[i][j] = 0;
        }
    }

    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (map[i][j] <= 0) continue;
            
            vector<pair<int, int>> v;
            for (int d = 0; d < 4; d++) {
                int nr = i + dx[d];
                int nc = j + dy[d];

                if (nr < 1 || nr > N || nc < 1 || nc > N) continue;
                if (map[nr][nc]) continue;
                if (jecho[nr][nc]) continue;
                v.push_back({ nr,nc });
            }

            for (int k = 0; k < v.size(); k++) {
                int nr = v[k].first;
                int nc = v[k].second;

                temp[nr][nc] += map[i][j] / v.size();
            }
        }
    }

    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            map[i][j] += temp[i][j];
        }
    }
}
void remove() {
    priority_queue<info, vector<info>, compare> pq;
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (map[i][j] <= 0) continue;

            int cnt = map[i][j];
            vector<pair<int, int>> temp;
            temp.push_back({ i,j });

            for (int d = 4; d < 8; d++) {
                for (int k = 1; k <= K; k++) {
                    bool flag = false;
                    int nr = i + k * dx[d];
                    int nc = j + k * dy[d];
                    if (nr < 1 || nr > N || nc < 1 || nc > N) break;
                    if (map[nr][nc] == -1) break;
                    if (flag) break;
                    if (map[nr][nc] == 0) {
                        flag = true;
                    }

                    cnt += map[nr][nc];
                    temp.push_back({ nr,nc });
                }
            }
            pq.push({ i, j, cnt, temp });
        }
    }

    if (pq.size() == 0) return;

    info tree = pq.top();
    ans += tree.cnt;
    for (int i = 0; i < tree.v.size(); i++) {
        jecho[tree.v[i].first][tree.v[i].second] = C + 1;
        map[tree.v[i].first][tree.v[i].second] = 0;
    }
}
void reduce() {
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (jecho[i][j] > 0) {
                jecho[i][j] -= 1;
            }
        }
    }
}
void solve() {
    while (M--) {
        grow();
        reproduce();
        remove();
        reduce();
    }
    cout << ans;
}
void input() {
    cin >> N >> M >> K >> C;
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            cin >> map[i][j];
        }
    }
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    input();
    solve();

    return 0;
}