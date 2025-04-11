#include <iostream>
#include <vector>
#include <queue>
#include <string.h>

#define MAX 21
using namespace std;

struct pos {
    int x, y;
};
int N, M, K;
int map[MAX][MAX];
int route[MAX][MAX];
bool visit[MAX][MAX];
vector<pos> team[6];
bool dir[6];
long long score;
int Round;
int dx[4] = { 0,-1,0,1 };
int dy[4] = { 1,0,-1,0 };

void bfs(int x, int y, int teamNum) {
    queue<pos> q;
    q.push({ x,y });
    visit[x][y] = true;
    team[teamNum].push_back({ x,y });

    while (!q.empty()) {
        pos now = q.front();
        q.pop();

        for (int i = 0; i < 4; i++) {
            int nx = now.x + dx[i];
            int ny = now.y + dy[i];

            if (nx < 0 || nx >= N || ny < 0 || ny >= N) continue;
            if (map[nx][ny] == 0) continue;
            if (visit[nx][ny]) continue;

            if (map[now.x][now.y] == 1 && (map[nx][ny] == 2 || map[nx][ny] == 3)) {
                visit[nx][ny] = true;
                q.push({ nx, ny });
                team[teamNum].push_back({ nx, ny });
            }
            else if (map[now.x][now.y] == 2 && (map[nx][ny] == 2 || map[nx][ny] == 3)) {
                visit[nx][ny] = true;
                q.push({ nx, ny });
                team[teamNum].push_back({ nx, ny });
            }
            else if ((map[now.x][now.y] == 3 || map[now.x][now.y] == 4) && map[nx][ny] == 4) {
                visit[nx][ny] = true;
                q.push({ nx, ny });
                team[teamNum].push_back({ nx, ny });
            }
        }
    }
}
void separateTeam() {
    int teamNum = 1;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (map[i][j] == 1) {
                bfs(i, j, teamNum);
                teamNum++;
            }
        }
    }

    for (int i = 1; i <= M; i++) {
        for (int j = 0; j < team[i].size(); j++) {
            // 이동경로 표시
            route[team[i][j].x][team[i][j].y] = i;

            // 사람 위치 1로 변경
            if (map[team[i][j].x][team[i][j].y] < 4 && map[team[i][j].x][team[i][j].y] > 0) {
                map[team[i][j].x][team[i][j].y] = 1;
            }
        }
        // map에서 4를 0으로 변경
        for (int j = 0; j < team[i].size(); j++) {
            if (map[team[i][j].x][team[i][j].y] == 4) {
                map[team[i][j].x][team[i][j].y] = 0;
                team[i].erase(team[i].begin() + j);
                j--;
            }
        }
    }
}
void teamMove() {
    for (int i = 1; i <= M; i++) {
        // 정방향
        if (dir[i] == false) {
            pos next;
            for (int j = 0; j < 4; j++) {
                next.x = team[i][0].x + dx[j];
                next.y = team[i][0].y + dy[j];

                if (next.x < 0 || next.x >= N || next.y < 0 || next.y >= N) continue;
                if (route[next.x][next.y] == i) {
                    if ((next.x != team[i][1].x) || (next.y != team[i][1].y)) {
                        break;
                    }
                }
            }

            int last = team[i].size() - 1;
            map[team[i][last].x][team[i][last].y] = 0;
            for (int j = last; j > 0; j--) {
                team[i][j] = team[i][j - 1];
            }
            team[i][0] = next;
            map[next.x][next.y] = 1;
        }
        // 역방향
        else {
            pos next;
            int first = team[i].size() - 1;
            for (int j = 0; j < 4; j++) {
                next.x = team[i][first].x + dx[j];
                next.y = team[i][first].y + dy[j];

                if (next.x < 0 || next.x >= N || next.y < 0 || next.y >= N) continue;
                if (route[next.x][next.y] == i) {
                    if ((next.x != team[i][first - 1].x) || (next.y != team[i][first - 1].y)) {
                        break;
                    }
                }
            }

            map[team[i][0].x][team[i][0].y] = 0;
            for (int j = 0; j < first; j++) {
                team[i][j] = team[i][j + 1];
            }
            team[i][first] = next;
            map[next.x][next.y] = 1;
        }
    }
}
void changeDirection(int teamNum) {
    dir[teamNum] = !dir[teamNum];
}
void getScore(int r, int c) {
    int teamNum = route[r][c];
    int idx = 1;

    // 정방향
    if (dir[teamNum] == false) {
        for (int i = 0; i < team[teamNum].size(); i++) {
            if (team[teamNum][i].x == r && team[teamNum][i].y == c) {
                break;
            }
            idx++;
        }
    }
    // 역방향
    else {
        for (int i = team[teamNum].size() - 1; i >= 0; i--) {
            if (team[teamNum][i].x == r && team[teamNum][i].y == c) {
                break;
            }
            idx++;
        }
    }

    score += (long long) (idx * idx);
    changeDirection(teamNum);
}
void throwBall() {
    int ballDir = (Round / 4) % 4;
    int row, col;

    if (ballDir == 0) {
        row = Round % N;
        for (col = 0; col < N; col++) {
            if (map[row][col] == 1) {
                getScore(row, col);
                break;
            }
        }
    }
    else if (ballDir == 1) {
        col = Round % N;
        for (row = N - 1; row >= 0; row--) {
            if (map[row][col] == 1) {
                getScore(row, col);
                break;
            }
        }
    }
    else if (ballDir == 2) {
        row = N - 1 - (Round % N);
        for (col = N - 1; col >= 0; col--) {
            if (map[row][col] == 1) {
                getScore(row, col);
                break;
            }
        }
    }
    else {
        col = N - 1 - (Round % N);
        for (row = 0; row < N; row++) {
            if (map[row][col] == 1) {
                getScore(row, col);
                break;
            }
        }
    }

}
void solution() {
    separateTeam();

    while (Round < K) {
        teamMove();
        throwBall();
        Round++;
    }

    cout << score;
}
void input() {
    cin >> N >> M >> K;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> map[i][j];
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