#include <iostream>
#include <vector>

#define MAX 16

using namespace std;

int N, M;
int d, p;
int map[MAX][MAX];
bool visit[MAX][MAX];
int dx[8] = { 0,-1,-1,-1,0,1,1,1 };
int dy[8] = { 1,1,0,-1,-1,-1,0,1 };
vector <pair<int, int>> v;

void preprocess() {
    v.push_back({ N - 2,0 });
    v.push_back({ N - 2,1 });
    v.push_back({ N - 1,0 });
    v.push_back({ N - 1,1 });
}
void arrange(int& x, int& y) {
    if (x < 0) {
        x += N;
    }
    else if (x >= N) {
        x -= N;
    }

    if (y < 0) {
        y += N;
    }
    else if (y >= N) {
        y -= N;
    }
}
void move() {
    for (int i = 0; i < v.size(); i++) {
        int nx = v[i].first + dx[d] * p;
        int ny = v[i].second + dy[d] * p;

        arrange(nx, ny);
        v[i].first = nx;
        v[i].second = ny;

        map[nx][ny] += 1;
        visit[nx][ny] = true;
    }
}
void grow() {
    vector<int> temp;
    for (int i = 0; i < v.size(); i++) {
        int cnt = 0;
        for (int j = 1; j < 8; j += 2) {
            int nx = v[i].first + dx[j];
            int ny = v[i].second + dy[j];

            if (nx < 0 || nx >= N || ny < 0 || ny >= N) continue;
            if (map[nx][ny] == 0) continue;
            cnt++;
        }
        temp.push_back(cnt);
    }

    for (int i = 0; i < v.size(); i++) {
        map[v[i].first][v[i].second] += temp[i];
    }
}
void find() {
    v.clear();

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (visit[i][j]) {
                visit[i][j] = false;
                continue;
            }
            if (map[i][j] >= 2) {
                v.push_back({ i,j });
                map[i][j] -= 2;
            }
        }
    }
}
void answer() {
    int ans = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            ans += map[i][j];
        }
    }
    cout << ans;
}
void solution() {
    preprocess();
    int year = 0;

    while (year < M) {
        cin >> d >> p;
        d--;
        move();
        grow();
        find();
        year++;
    }
    answer();
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