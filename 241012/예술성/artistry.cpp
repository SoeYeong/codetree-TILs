#include <iostream>
#include <cstring>
#include <vector>
#include <queue>

#define MAX 30
using namespace std;

int N;
int map[MAX][MAX];
int group[MAX][MAX];    // 그룹 인덱스 기록
pair<int, int> groupSize[MAX * MAX]; // 그룹 크기, 그룹 이루고 있는 숫자
int close[MAX * MAX][MAX * MAX];    // [from][to] from->to 닿아있는 변 개수
int dy[4] = { -1,1,0,0 };
int dx[4] = { 0,0,-1,1 };
int gcnt, ans;

void init() {
    gcnt = 0;
    memset(group, 0, sizeof(group));
    memset(groupSize, 0, sizeof(groupSize));
    memset(close, 0, sizeof(close));
}
void rotateCross() {
    int mid = N / 2;
    int temp[30];
    for (int i = 0; i < N; i++) {
        temp[i] = map[mid][i];
    }

    for (int i = 0; i < N; i++) {
        map[mid][i] = map[i][mid];
    }
    for (int i = 0; i < N; i++) {
        map[i][mid] = temp[N - 1 - i];
    }
}
void rotateSquare() {
    int mid = N / 2;
    int temp[30][30];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            temp[i][j] = map[i][j];
        }
    }

    for (int i = 0; i < mid; i++) {
        for (int j = 0; j < mid; j++) {
            map[i][j] = temp[mid - 1 - j][i];
        }
    }
    for (int i = mid + 1; i < N; i++) {
        for (int j = 0; j < mid; j++) {
            map[i][j] = temp[N - 1 - j][i - mid - 1];
        }
    }
    for (int i = 0; i < mid; i++) {
        for (int j = mid + 1; j < N; j++) {
            map[i][j] = temp[N - 1 - j][i + mid + 1];
        }
    }
    for (int i = mid + 1; i < N; i++) {
        for (int j = 1; j < N - mid; j++) {
            map[i][j + mid] = temp[N - j][i];
        }
    }
}
void bfs(int y, int x) {
    gcnt++;
    queue<pair<int, int>> q;
    q.push({ y,x });

    groupSize[gcnt].second = map[y][x];
    group[y][x] = gcnt;
    int cnt = 1;

    while (!q.empty()) {
        pair<int, int> now = q.front();
        q.pop();

        for (int i = 0; i < 4; i++) {
            int ny = now.first + dy[i];
            int nx = now.second + dx[i];
            if (ny < 0 || ny >= N || nx < 0 || nx >= N) continue;
            if (map[now.first][now.second] != map[ny][nx]) continue;
            if (group[ny][nx]) continue;

            group[ny][nx] = gcnt;
            q.push({ ny,nx });
            cnt++;
        }
    }
    groupSize[gcnt].first = cnt;
}
void findClose() {
    // 아래쪽 변
    for (int i = 0; i < N - 1; i++) {
        for (int j = 0; j < N; j++) {
            int from = group[i][j];

            int to = group[i + 1][j];
            if (from != to) close[from][to]++;
        }
    }
    // 오른쪽 변
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N - 1; j++) {
            int from = group[i][j];

            int to = group[i][j + 1];
            if (from != to) close[from][to]++;
        }
    }
}
void calScore() {
    for (int i = 1; i < gcnt; i++) {
        for (int j = i + 1; j <= gcnt; j++) {
            int score = groupSize[i].first + groupSize[j].first;
            score *= groupSize[i].second * groupSize[j].second;
            score *= (close[i][j] + close[j][i]);
            ans += score;
        }
    }
}
void findGroup() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (group[i][j]) continue;
            bfs(i, j);
        }
    }
    findClose();
    calScore();
}
void solution() {
    findGroup();
    for (int i = 0; i < 3; i++) {
        init();
        rotateCross();
        rotateSquare();
        findGroup();
    }
    cout << ans;
}
void input() {
    cin >> N;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> map[i][j];
        }
    }
}
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    input();
    solution();

    return 0;
}