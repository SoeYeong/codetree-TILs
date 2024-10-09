#include <iostream>
#include <vector>
#include <queue>

#define MAX 0x7fffffff
using namespace std;

int Q, cmd, N, M;
int start;
int dist[2001];
bool checkId[30001];
struct landInfo {
    int u, w;
};
struct packageInfo {
    int id, revenue, dest, cost;
};
struct cmp1 {
    bool operator()(landInfo a, landInfo b) {
        return a.w > b.w;
    }
};
struct cmp2 {
    bool operator()(packageInfo a, packageInfo b) {
        if (a.cost != b.cost) return a.cost < b.cost;
        return a.id > b.id;
    }
};

vector<landInfo> lands[2001];
priority_queue<packageInfo, vector<packageInfo>, cmp2> packages;

void init() {
    for (int i = 0; i < 2001; i++) {
        dist[i] = MAX;
    }
}
void dijkstra() {
    init();
    dist[start] = 0;
    priority_queue<landInfo, vector<landInfo>, cmp1> pq;
    pq.push({ start,0 });

    while (!pq.empty()) {
        landInfo now = pq.top();
        pq.pop();

        for (int i = 0; i < lands[now.u].size(); i++) {
            landInfo next = lands[now.u][i];
            if (dist[next.u] <= dist[now.u] + next.w) continue;
            pq.push(next);
            dist[next.u] = dist[now.u] + next.w;
        }
    }
}
void func1() {
    cin >> N >> M;

    int v, u, w;
    for (int i = 0; i < M; i++) {
        cin >> v >> u >> w;
        lands[v].push_back({ u,w });
        lands[u].push_back({ v,w });
    }
    dijkstra();
}
void func2() {
    int id, revenue, dest;
    int cost = 0;
    cin >> id >> revenue >> dest;
    if (dist[dest] == MAX) {
        cost = -1;
    }
    else {
        cost = revenue - dist[dest];
    }
    packages.push({ id,revenue,dest,cost });
    checkId[id] = true;
}
void func3() {
    int id;
    cin >> id;
    checkId[id] = false;
}
void func4() {
    if (packages.empty()) {
        cout << -1 << "\n";
        return;
    }
    while (!packages.empty()) {
        packageInfo now = packages.top();
        if (!checkId[now.id]) {
            packages.pop();
        }
        else {
            break;
        }
    }
    if (packages.empty()) {
        cout << -1 << "\n";
        return;
    }

    packageInfo now = packages.top();
    if (now.cost < 0) {
        cout << -1 << "\n";
    }
    else {
        packages.pop();
        checkId[now.id] = false;
        cout << now.id << "\n";
    }
}
void func5() {
    cin >> start;
    dijkstra();

    priority_queue<packageInfo, vector<packageInfo>, cmp2> temp;
    while (!packages.empty()) {
        packageInfo now = packages.top();
        packages.pop();
        if (!checkId[now.id]) continue;
        temp.push(now);
    }

    while (!temp.empty()) {
        packageInfo now = temp.top();
        int cost = 0;
        int dest = now.dest;
        int revenue = now.revenue;
        if (dist[dest] == MAX) {
            cost = -1;
        }
        else {
            cost = revenue - dist[dest];
        }
        now.cost = cost;
        packages.push(now);
        temp.pop();
    }
}
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);
    
    cin >> Q;
    while (Q--) {
        cin >> cmd;
        if (cmd == 100) {
            func1();
        }
        else if (cmd == 200) {
            func2();
        }
        else if (cmd == 300) {
            func3();
        }
        else if (cmd == 400) {
            func4();
        }
        else if (cmd == 500) {
            func5();
        }
    }

    return 0;
}