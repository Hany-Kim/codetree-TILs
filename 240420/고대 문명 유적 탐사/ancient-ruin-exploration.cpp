#include <iostream>
#include <queue>
#include <algorithm>
#include <cstring>
using namespace std;

#define MAP_MAX (5)
#define K_MAX (10)
#define M_MAX (300 + 1)
#define FOR(i,s,e) for(int i=(s); i<(e); ++i)
#define PII pair<int,int>

int K, M;
int map[MAP_MAX][MAP_MAX];
int tmap[MAP_MAX][MAP_MAX];
int subBlockList[M_MAX];
int blockIdx;
int used[MAP_MAX][MAP_MAX];
int dUsed[MAP_MAX][MAP_MAX];
int dy[4] = { -1,1,0,0 };
int dx[4] = { 0,0,-1,1 };
int score;

void mrotate(int sy, int sx) {
    int rmap[3][3] = { 0, };
    vector<int> tmp;

    FOR(x, sx - 1, sx + 1) {
        tmp.push_back(tmap[sy - 1][x]);
    }

    int xIdx = 1;
    FOR(y, sy, sy + 2) {
        rmap[0][xIdx--] = tmap[y][sx - 1];
    }

    int yIdx = 1;
    FOR(x, sx, sx + 2) {
        rmap[yIdx++][0] = tmap[sy + 1][x];
    }

    xIdx = 2;
    FOR(y, sy - 1, sy + 1) {
        rmap[2][xIdx--] = tmap[y][sx + 1];
    }

    yIdx = 0;
    FOR(i, 0, tmp.size()) {
        rmap[yIdx++][2] = tmp[i];
    }

    rmap[1][1] = tmap[sy][sx];

    FOR(y, sy - 1, sy + 2) {
        FOR(x, sx - 1, sx + 2) {
            tmap[y][x] = rmap[y - (sy - 1)][x - (sx - 1)];
        }
    }
}

void mset() {
    FOR(y, 0, MAP_MAX) {
        FOR(x, 0, MAP_MAX) {
            tmap[y][x] = (y * MAP_MAX) + x;
        }
    }
}

int bfs(int sy, int sx) {
    queue<PII> q;
    memset(used, 0, sizeof(used));
    int cnt = 1;

    q.push(make_pair(sy, sx));
    used[sy][sx] = 1;

    while (!q.empty()) {
        PII now = q.front();
        q.pop();

        FOR(i, 0, 4) {
            int ny = now.first + dy[i];
            int nx = now.second + dx[i];

            if (ny < 0 || ny >= MAP_MAX || nx < 0 || nx >= MAP_MAX) continue;
            if (used[ny][nx] == 1) continue;
            if (tmap[ny][nx] != tmap[sy][sx]) continue;

            cnt += 1;
            used[ny][nx] = 1;
            q.push(make_pair(ny, nx));
        }
    }

    if (cnt >= 3) {
        return cnt;
    }
    return 0;
}

int getScore() {
    int curScore = 0;

    FOR(y, 0, MAP_MAX) {
        FOR(x, 0, MAP_MAX) {
            if (dUsed[y][x] == 1) continue;
            curScore += bfs(y, x);
        }
    }

    return curScore;
}

bool cmp(pair<PII, PII> left, pair<PII, PII> right) {
    if (left.first.second > right.first.second) return true;
    if (left.first.second < right.first.second) return false;

    if (left.first.first < right.first.first) return true;
    if (left.first.first > right.first.first) return false;

    if (left.first.second < right.first.second) return true;
    if (left.first.second > right.first.second) return false;
    // 위아래 바꿔서 예외있나확인
    if (left.first.first < right.first.first) return true;
    if (left.first.first > right.first.first) return false;

    return false;
}

int mbfs(int sy, int sx) {
    queue<PII> q;
    memset(used, 0, sizeof(used));
    int cnt = 1;

    q.push(make_pair(sy, sx));
    used[sy][sx] = 1;

    while (!q.empty()) {
        PII now = q.front();
        q.pop();

        FOR(i, 0, 4) {
            int ny = now.first + dy[i];
            int nx = now.second + dx[i];

            if (ny < 0 || ny >= MAP_MAX || nx < 0 || nx >= MAP_MAX) continue;
            if (used[ny][nx] == 1) continue;
            if (tmap[ny][nx] != tmap[sy][sx]) continue;

            cnt += 1;
            used[ny][nx] = 1;
            q.push(make_pair(ny, nx));
        }
    }

    if (cnt >= 3) {
        FOR(y, 0, MAP_MAX) {
            FOR(x, 0, MAP_MAX) {
                if (used[y][x] == 1) dUsed[y][x] = 1;
            }
        }
        return cnt;
    }
    return 0;
}

bool endGame() {
    FOR(y, 0, MAP_MAX) {
        FOR(x, 0, MAP_MAX) {
            if (dUsed[y][x] == 1) return false;
        }
    }

    return true;
}

void sol() {
    FOR(turn, 1, (K + 1)) {
        vector<pair<PII, PII>> v; // 회전 각도, 얻은 점수, 중심좌표 (y,x)
        FOR(y, 1, 4) {
            FOR(x, 1, 4) {
                memcpy(tmap, map, sizeof(map));
                //mset(); // 더미값 넣어서 사용하는 회전테스트 용
                FOR(r, 1, 4) {
                    mrotate(y, x);
                    int curScore = getScore();

                    v.push_back(make_pair(make_pair(r, curScore), make_pair(y, x)));
                }
            }
        }

        if (v.size() == 0) break;
        sort(v.begin(), v.end(), cmp);
        if (v[0].first.second == 0) break;

        memcpy(tmap, map, sizeof(map));
        FOR(r, 0, v[0].first.first) {
            mrotate(v[0].second.first, v[0].second.second);
        }

        while (1) {
            memset(dUsed, 0, sizeof(dUsed));
            int cScore = 0;
            FOR(y, 0, MAP_MAX) {
                FOR(x, 0, MAP_MAX) {
                    if (dUsed[y][x] == 1) continue;
                    cScore += mbfs(y, x);
                }
            }
            score += cScore;

            if (endGame() == true) break;

            FOR(y, 0, MAP_MAX) {
                FOR(x, 0, MAP_MAX) {
                    if (dUsed[y][x] == 1) {
                        tmap[y][x] = 0;
                    }
                }
            }

            FOR(x, 0, MAP_MAX) {
                for (int y = (MAP_MAX - 1); y >= 0; y--) {
                    if (tmap[y][x] == 0) {
                        tmap[y][x] = subBlockList[blockIdx++];
                    }
                }
            }

            memcpy(map, tmap, sizeof(map));
        }

        cout << score << " ";
        score = 0;
    }

}

void input() {
    cin >> K >> M;
    FOR(y, 0, MAP_MAX) {
        FOR(x, 0, MAP_MAX) {
            cin >> map[y][x];
        }
    }

    FOR(i, 0, M) {
        cin >> subBlockList[i];
    }

    blockIdx = 0;
}

int main() {
    input();
    sol();
    return 0;
}