#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <queue>
#include <climits>
#include <math.h>
using namespace std;

#define N_MAX (20 + 1)
#define M_MAX (20 + 1)
#define PII pair<int,int>
#define FOR(i,s,e) for(int i=(s); i<(e); ++i)

int N, M, K;
struct TOWER {
    int power;
    int lastAtkTurn;
};
TOWER map[N_MAX][M_MAX];
int dy[4] = { 0,1,0,-1 };
int dx[4] = { 1,0,-1,0 };
vector<PII> path;
int ty[8] = { -1,-1,-1,0,0,1,1,1 };
int tx[8] = { -1,0,1,-1,1,-1,0,1 };

PII selectAttacker() {
    PII ret;
    TOWER attacker;
    attacker.power = INT_MAX;
    attacker.lastAtkTurn = 0;

    FOR(y, 1, N + 1) {
        FOR(x, 1, M + 1) {
            if (map[y][x].power <= 0) continue;
            if (attacker.power > map[y][x].power) {
                attacker.power = map[y][x].power;
                attacker.lastAtkTurn = map[y][x].lastAtkTurn;
                ret = make_pair(y, x);
            }
            else if (attacker.power == map[y][x].power) {
                if (attacker.lastAtkTurn < map[y][x].lastAtkTurn) {
                    attacker.power = map[y][x].power;
                    attacker.lastAtkTurn = map[y][x].lastAtkTurn;
                    ret = make_pair(y, x);
                }
                else if (attacker.lastAtkTurn == map[y][x].lastAtkTurn) {
                    if ((ret.first + ret.second) < (y + x)) {
                        attacker.power = map[y][x].power;
                        attacker.lastAtkTurn = map[y][x].lastAtkTurn;
                        ret = make_pair(y, x);
                    }
                    else if ((ret.first + ret.second) == (y + x)) {
                        if (ret.second < x) {
                            attacker.power = map[y][x].power;
                            attacker.lastAtkTurn = map[y][x].lastAtkTurn;
                            ret = make_pair(y, x);
                        }
                    }
                }
            }
        }
    }

    return ret;
}
PII selectTarget(PII atk) {
    PII ret;
    TOWER target;
    target.power = 0;
    target.lastAtkTurn = INT_MAX;

    FOR(y, 1, N + 1) {
        FOR(x, 1, M + 1) {
            if (map[y][x].power <= 0) continue;
            if (y == atk.first && x == atk.second) continue;

            if (target.power < map[y][x].power) {
                target.power = map[y][x].power;
                target.lastAtkTurn = map[y][x].lastAtkTurn;
                ret = make_pair(y, x);
            }
            else if (target.power == map[y][x].power) {
                if (target.lastAtkTurn > map[y][x].lastAtkTurn) {
                    target.power = map[y][x].power;
                    target.lastAtkTurn = map[y][x].lastAtkTurn;
                    ret = make_pair(y, x);
                }
                else if (target.lastAtkTurn == map[y][x].lastAtkTurn) {
                    if ((ret.first + ret.second) > (y + x)) {
                        target.power = map[y][x].power;
                        target.lastAtkTurn = map[y][x].lastAtkTurn;
                        ret = make_pair(y, x);
                    }
                    else if ((ret.first + ret.second) == (y + x)) {
                        if (ret.second > x) {
                            target.power = map[y][x].power;
                            target.lastAtkTurn = map[y][x].lastAtkTurn;
                            ret = make_pair(y, x);
                        }
                    }
                }
            }
        }
    }

    return ret;
}

bool bfs(PII atk, PII tar) {
    queue<pair<PII, vector<PII>>> q; // pair(node(y,x), path(y,x))
    vector<PII> mpath;
    int used[N_MAX][M_MAX] = { 0, };

    mpath.push_back(atk);
    q.push(make_pair(atk, mpath));
    used[atk.first][atk.second] = 1;

    while (!q.empty()) {
        pair<PII, vector<PII>> now = q.front();
        q.pop();

        FOR(i, 0, 4) {
            int ny = now.first.first + dy[i];
            int nx = now.first.second + dx[i];
            vector<PII> npath = now.second;

            if (ny < 1) ny = N;
            if (ny > N) ny = 1;
            if (nx < 1) nx = M;
            if (nx > M) nx = 1;
            if (map[ny][nx].power <= 0) continue; // 파괴된 포탑
            if (used[ny][nx] > 0) continue; // 이미 방문

            used[ny][nx] = used[now.first.first][now.first.second] + 1;
            npath.push_back(make_pair(ny, nx));
            if (ny == tar.first && nx == tar.second) path = npath;
            q.push(make_pair(make_pair(ny, nx), npath));
        }
    }

    if (path.size() == 0) return false;
    if ((path[path.size() - 1].first == tar.first) && (path[path.size() - 1].second == tar.second)) return true;
    else return false;
}

void output(int mLog[][M_MAX]) {
    cout << "map\n";
    FOR(y, 1, (N + 1)) {
        FOR(x, 1, (M + 1)) {
            cout << map[y][x].power << "\t";
        }
        cout << "\n";
    }
    cout << "기록\n";
    FOR(y, 1, (N + 1)) {
        FOR(x, 1, (M + 1)) {
            cout << mLog[y][x] << "\t";
        }
        cout << "\n";
    }
}

bool isEnd() {
    int towerCnt = 0;
    FOR(y, 1, (N + 1)) {
        FOR(x, 1, (M + 1)) {
            if (map[y][x].power > 0) {
                if (towerCnt > 1) return false;
                towerCnt += 1;
            }
        }
    }
    if (towerCnt <= 1) return true;
    else return false;
}

void sol() {
    FOR(turn, 1, (K+1)) {
        int mLog[N_MAX][M_MAX] = { 0, };
        //cout << turn << "턴\n";
        // 공격자, 대상 찾기
        PII attacker = selectAttacker();
        PII target = selectTarget(attacker);
        mLog[attacker.first][attacker.second] = 1;
        mLog[target.first][target.second] = 1;

        // 공격자 준비
        map[attacker.first][attacker.second].power = map[attacker.first][attacker.second].power + N + M;
        map[attacker.first][attacker.second].lastAtkTurn = turn;
        int atkPower = map[attacker.first][attacker.second].power;
        /*cout << "공격자 (" << attacker.first << "," << attacker.second << ") 힘 : " << atkPower << "\n";
        cout << "타  겟 (" << target.first << "," << target.second << ") 힘 : " << map[target.first][target.second].power << "\n";*/

        // 공격
        if (bfs(attacker, target) == true) { // 레이저
            PII now;
            FOR(i, 1, (path.size() - 1)) {
                now = path[i];
                map[now.first][now.second].power = map[now.first][now.second].power - (atkPower >> 1);
                if (map[now.first][now.second].power <= 0) map[now.first][now.second].power = 0;
                mLog[now.first][now.second] = 1;
            }
            now = path[path.size() - 1];
            map[now.first][now.second].power = map[now.first][now.second].power - atkPower;
            if (map[now.first][now.second].power <= 0) map[now.first][now.second].power = 0;
            mLog[now.first][now.second] = 1;
        }
        else { // 폭탄던지기
            PII now = target;
            FOR(i, 0, 8) {
                int ny = now.first + ty[i];
                int nx = now.second + tx[i];

                if (ny < 1) ny = N;
                if (ny > N) ny = 1;
                if (nx < 1) nx = M;
                if (nx > M) nx = 1;
                if (ny == attacker.first && nx == attacker.second) continue;
                if (map[ny][nx].power <= 0) continue;

                map[ny][nx].power = map[ny][nx].power - (atkPower >> 1);
                if (map[ny][nx].power <= 0) map[ny][nx].power = 0;
                mLog[ny][nx] = 1;
            }
            map[now.first][now.second].power = map[now.first][now.second].power - atkPower;
            if (map[now.first][now.second].power <= 0) map[now.first][now.second].power = 0;
            mLog[now.first][now.second] = 1;
        }

        /*cout << "[공격 후]\n";
        output(mLog);
        cout << "\n";*/

        if (isEnd()) break;

        FOR(y, 1, (N + 1)) {
            FOR(x, 1, (M + 1)) {
                if (mLog[y][x] != 0) continue; // 공격과 무관한 포탑이아니면
                if (map[y][x].power <= 0) continue; //파괴된 포탑이면
                map[y][x].power += 1;
            }
        }
        /*cout << "[정비 후]\n";
        output(mLog);
        cout << "\n";*/

        path.clear();
    }

    int maxPower = 0;
    FOR(y, 1, (N + 1)) {
        FOR(x, 1, (M + 1)) {
            maxPower = max(maxPower, map[y][x].power);
        }
    }
    cout << maxPower;
}

void input() {
    cin >> N >> M >> K;
    FOR(y, 1, (N + 1)) {
        FOR(x, 1, (M + 1)) {
            cin >> map[y][x].power;
            map[y][x].lastAtkTurn = 0;
        }
    }
}

int main() {
    input();
    sol();
    return 0;
}