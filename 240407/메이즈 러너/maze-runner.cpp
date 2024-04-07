#include <iostream>
#include <algorithm>
#include <vector>
#include <math.h>
#include <cstring>
using namespace std;

#define N_MAX (10 + 1)
#define M_MAX (10)
#define FOR(i,s,e) for(int i=s; i<(e); ++i)

int N, M, K;
int miro[N_MAX][N_MAX];

pair<int, int> mExit;
int dy[4] = { -1,1,0,0 };
int dx[4] = { 0,0,-1,1 };
int distSum;

void input() {
    cin >> N >> M >> K;
    FOR(y, 1, N + 1) {
        FOR(x, 1, N + 1) {
            cin >> miro[y][x];
        }
    }
    FOR(i, 0, M) {
        int y, x;
        cin >> y >> x;

        miro[y][x] -= 1;
    }
    int ey = 0, ex = 0;
    cin >> ey >> ex;
    mExit = make_pair(ey, ex);
    miro[mExit.first][mExit.second] = -11;
}

int distToExit(int y, int x) {
    return abs(mExit.first - y) + abs(mExit.second - x);
}

bool nextCmp(pair<int, int> left, pair<int, int> right) {
    if (left.second < right.second) return true;
    if (left.second > right.second) return false;

    if (left.first < right.first) return true;
    if (left.first > right.first) return false;

    return false;
}

void update_exit() {
    FOR(y, 1, (N + 1)) {
        FOR(x, 1, (N + 1)) {
            if (miro[y][x] <= -1 && miro[y][x] >= -10) continue;
            else if (miro[y][x] == -11) {
                mExit = make_pair(y, x);
            }
        }
    }
}

void all_move() {
    int next_miro[N_MAX][N_MAX] = { 0, };
    FOR(y, 1, (N + 1)) {
        FOR(x, 1, (N + 1)) {
            if (miro[y][x] <= -1 && miro[y][x] >= -10) continue;
            next_miro[y][x] = miro[y][x];
        }
    }

    FOR(y, 1, N + 1) {
        FOR(x, 1, N + 1) {
            // 참가자 아니면 건너뜀
            if (miro[y][x] > -1 || miro[y][x] < -10) continue;

            int curDist = distToExit(y, x);
            vector<pair<int, int>> v;
            FOR(i, 0, 4) {
                int ny = y + dy[i];
                int nx = x + dx[i];

                if (ny<1 || ny>N || nx<1 || nx>N) continue; // 맵밖
                if (miro[ny][nx] > 0) continue; // 벽

                int nextDist = distToExit(ny, nx);

                // 출구 만나면
                if (nextDist == 0) {
                    distSum += (-1 * miro[y][x]);
                    //cout << "도착 : " << (-1 * miro[y][x]) << "\n";
                    miro[y][x] = 0;
                }
                else if (nextDist < curDist) {
                    v.push_back(make_pair(i, nextDist));
                }
            }
            if (v.size() == 0) {
                next_miro[y][x] += miro[y][x];
                continue;
            }
            sort(v.begin(), v.end(), nextCmp);
            int nextPosY = y + dy[v[0].first];
            int nextPosX = x + dx[v[0].first];
            next_miro[nextPosY][nextPosX] += miro[y][x];
            //cout << "이동 : " << (-1 * miro[y][x]) << "\n";
            distSum += (-1 * miro[y][x]);
        }
    }
    memcpy(miro, next_miro, sizeof(miro));
}

pair<pair<int, int>, int> find_square() {
    pair<pair<int, int>, int> lts; //left_top_size
    FOR(len, 2, 11) { // 그림 그릴 사각형 한변 길이
        // 사각형 좌상단 기준 잡기
        FOR(sy, mExit.first - len + 1, mExit.first + 1) {
            FOR(sx, mExit.second - len + 1, mExit.second + 1) {
                // 그린 사각형 안에 사람 있는지
                bool isOutOfMap = false;
                bool isFindSQ = false;
                FOR(y, sy, sy + len) {
                    FOR(x, sx, sx + len) {
                        // 사람 찾았으면
                        if (y < 1 || y > N || x < 1 || x > N) {
                            isOutOfMap = true;
                            break;
                        }
                        if (miro[y][x] <= -1 && miro[y][x] >= -10) { // 사람 있으면 현재 좌상단 기록
                            lts = make_pair(make_pair(sy, sx), len);
                            isFindSQ = true;
                        }
                    }
                    if (isOutOfMap == true) break;
                }
                if (isOutOfMap == false && isFindSQ == true) return lts; // 맵안에 그려진 사각형이면 리턴
            }
        }
    }
    lts.second = -1;
    return lts;
}

void rotate() {
    pair<pair<int, int>, int> lt = find_square();
    int sy = lt.first.first;
    int sx = lt.first.second;
    int len = lt.second;

    //vector<vector<int>> sq(len, vector<int>(len, 0));
    int sq[N_MAX][N_MAX] = { 0, };

    int cy = 0;
    int cx = len - 1;
    FOR(y, sy, sy + len) {
        FOR(x, sx, sx + len) {
            if (miro[y][x] > 0) miro[y][x] -= 1;
            sq[cy++][cx] = miro[y][x];
        }
        cy = 0;
        --cx;
    }

    cy = 0;
    cx = 0;
    FOR(y, sy, sy + len) {
        FOR(x, sx, sx + len) {
            miro[y][x] = sq[cy][cx++];
        }
        ++cy;
        cx = 0;
    }
}

void output() {
    FOR(y, 1, N + 1) {
        FOR(x, 1, N + 1) {
            if (miro[y][x] == -11) cout << "* ";
            else if (miro[y][x] < 0) cout << "& ";
            else cout << miro[y][x] << ' ';
        }
        cout << '\n';
    }
}

bool isEnd() {
    FOR(y, 1, N + 1) {
        FOR(x, 1, N + 1) {
            if (miro[y][x] <= -1 && miro[y][x] >= -10) return false;
        }
    }
    return true;
}

void sol() {
    int time = 0;
    while (time < K) {
        ++time;
        //cout << "시간 : " << time << "\n\n";
        all_move();
        //cout << "이번턴까지 이동거리 : " << distSum << "\n";
        /*cout << "이동 후\n";
        output();*/
        rotate();
        /*cout << "회전 후\n";
        output();*/
        update_exit();
        if (isEnd()) break;
    }
    cout << distSum << '\n';
    cout << mExit.first << " " << mExit.second;
}

int main() {
    input();
    sol();
    return 0;
}