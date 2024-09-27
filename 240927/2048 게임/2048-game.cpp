#include <iostream>
#include <queue>
#include <algorithm>
using namespace std;

#define N_MAX 20
#define PIVVI pair<int, vector<vector<int>>>

int n;
int map[N_MAX][N_MAX];
int dy[4] = { -1,0,1,0 };
int dx[4] = { 0,-1,0,1 };
int ans;

void pushPuzzle(int dir, vector<vector<int>>& v) {
    vector<vector<int>> isAdd(v.size(), vector<int>(v[0].size(), 0));
    switch (dir)
    {
    case 0:
        for (int y = 0; y < n; ++y) {
            for (int x = 0; x < n; ++x) {
                if (isAdd[y][x] == 1) continue;

                int ny = y + dy[dir];
                int nx = x + dx[dir];

                if (ny < 0 || ny >= n || nx < 0 || nx >= n) continue;
                if (v[ny][nx] == -1) continue;
                if (isAdd[ny][nx] == 1) continue;

                if (v[ny][nx] == v[y][x] && v[y][x] != 0) {
                    isAdd[ny][nx] = 1;
                    isAdd[y][x] = 1;

                    v[ny][nx] += v[y][x];
                    v[y][x] = -1;
                }
            }
        }

        for (int y = 0; y < n; ++y) {
            for (int x = 0; x < n; ++x) {
                int nowY = y;
                int nowX = x;
                while (1) {
                    int ny = nowY + dy[dir];
                    int nx = nowX + dx[dir];

                    if (ny < 0 || ny >= n || nx < 0 || nx >= n) break;

                    if (v[ny][nx] == -1 || v[ny][nx] == 0) {
                        v[ny][nx] = v[nowY][nowX];
                        v[nowY][nowX] = 0;
                        nowY = ny;
                        nowX = nx;
                    }
                    else {
                        if (v[nowY][nowX] == -1) v[nowY][nowX] = 0;
                        break;
                    }

                }
            }
        }
        break;
    case 1:
        for (int x = 0; x < n; ++x) {
            for (int y = 0; y < n; ++y) {
                if (isAdd[y][x] == 1) continue;

                int ny = y + dy[dir];
                int nx = x + dx[dir];

                if (ny < 0 || ny >= n || nx < 0 || nx >= n) continue;
                if (v[ny][nx] == -1) continue;
                if (isAdd[ny][nx] == 1) continue;

                if (v[ny][nx] == v[y][x] && v[y][x] != 0) {
                    isAdd[ny][nx] = 1;
                    isAdd[y][x] = 1;

                    v[ny][nx] += v[y][x];
                    v[y][x] = -1;
                }
            }
        }

        for (int x = 0; x < n; ++x) {
            for (int y = 0; y < n; ++y) {
                int nowY = y;
                int nowX = x;
                while (1) {
                    int ny = nowY + dy[dir];
                    int nx = nowX + dx[dir];

                    if (ny < 0 || ny >= n || nx < 0 || nx >= n) break;

                    if (v[ny][nx] == -1 || v[ny][nx] == 0) {
                        v[ny][nx] = v[nowY][nowX];
                        v[nowY][nowX] = 0;
                        nowY = ny;
                        nowX = nx;
                    }
                    else {
                        if (v[nowY][nowX] == -1) v[nowY][nowX] = 0;
                        break;
                    }
                }
            }
        }
        break;
    case 2:
        for (int y = (n-1); y >= 0; y--) {
            for (int x = 0; x < n; ++x) {
                if (isAdd[y][x] == 1) continue;

                int ny = y + dy[dir];
                int nx = x + dx[dir];

                if (ny < 0 || ny >= n || nx < 0 || nx >= n) continue;
                if (v[ny][nx] == -1) continue;
                if (isAdd[ny][nx] == 1) continue;

                if (v[ny][nx] == v[y][x] && v[y][x] != 0) {
                    isAdd[ny][nx] = 1;
                    isAdd[y][x] = 1;

                    v[ny][nx] += v[y][x];
                    v[y][x] = -1;
                }
            }
        }

        for (int y = (n - 1); y >= 0; y--) {
            for (int x = 0; x < n; ++x) {
                int nowY = y;
                int nowX = x;
                while (1) {
                    int ny = nowY + dy[dir];
                    int nx = nowX + dx[dir];

                    if (ny < 0 || ny >= n || nx < 0 || nx >= n) break;

                    if (v[ny][nx] == -1 || v[ny][nx] == 0) {
                        v[ny][nx] = v[nowY][nowX];
                        v[nowY][nowX] = 0;
                        nowY = ny;
                        nowX = nx;
                    }
                    else {
                        if (v[nowY][nowX] == -1) v[nowY][nowX] = 0;
                        break;
                    }
                }
            }
        }
        break;
    case 3:
        for (int x = (n-1); x >= 0; x--) {
            for (int y = 0; y < n; ++y) {
                if (isAdd[y][x] == 1) continue;

                int ny = y + dy[dir];
                int nx = x + dx[dir];

                if (ny < 0 || ny >= n || nx < 0 || nx >= n) continue;
                if (v[ny][nx] == -1) continue;
                if (isAdd[ny][nx] == 1) continue;

                if (v[ny][nx] == v[y][x] && v[y][x] != 0) {
                    isAdd[ny][nx] = 1;
                    isAdd[y][x] = 1;

                    v[ny][nx] += v[y][x];
                    v[y][x] = -1;
                }
            }
        }

        for (int x = (n - 1); x >= 0; x--) {
            for (int y = 0; y < n; ++y) {
                int nowY = y;
                int nowX = x;
                while (1) {
                    int ny = nowY + dy[dir];
                    int nx = nowX + dx[dir];

                    if (ny < 0 || ny >= n || nx < 0 || nx >= n) break;

                    if (v[ny][nx] == -1 || v[ny][nx] == 0) {
                        v[ny][nx] = v[nowY][nowX];
                        v[nowY][nowX] = 0;
                        nowY = ny;
                        nowX = nx;
                    }
                    else {
                        if (v[nowY][nowX] == -1) v[nowY][nowX] = 0;
                        break;
                    }
                }
            }
        }
        break;
    }
}

void out(vector<vector<int>>& v) {
    for (int y = 0; y < n; ++y) {
        for (int x = 0; x < n; ++x) {
            cout << v[y][x] << ' ';
        }
        cout << '\n';
    }
    cout << "-----------\n";
}

int getMaxBlock(vector<vector<int>>& v) {
    int ret = 0;
    for (int y = 0; y < n; ++y) {
        for (int x = 0; x < n; ++x) {
            ret = max(ret, v[y][x]);
        }
    }
    return ret;
}

void bfs() {
    queue<PIVVI> q;
    
    PIVVI start;
    start.first = 0;
    start.second = vector<vector<int>>(n, vector<int>(n, 0));
    
    for (int y = 0; y < n; ++y) {
        for (int x = 0; x < n; ++x) {
            start.second[y][x] = map[y][x];
        }
    }

    q.push(start);
    while (!q.empty()) {
        PIVVI now = q.front();
        q.pop();

        for (int i = 0; i < 4; ++i) {
            vector<vector<int>> tmp = now.second;
            pushPuzzle(i, tmp);
            ans = max(ans, getMaxBlock(tmp));


            if((now.first + 1) < 5) q.push(make_pair(now.first + 1, tmp));
        }
    }
}

void sol() {
    bfs();
    cout << ans;
}

void in() {
    cin >> n;
    for (int y = 0; y < n; ++y) {
        for (int x = 0; x < n; ++x) {
            cin >> map[y][x];
        }
    }
}

int main() {
    //freopen("input.txt", "r", stdin);
    in();
    sol();
    return 0;
}