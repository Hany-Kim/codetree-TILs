#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <queue>
#include <math.h>
using namespace std;

#define N_MAX (5 + 1)
#define M_MAX (31)
#define FOR(i,s,e) for(int i=s; i<(e); ++i)

int n, m, k;
int map[N_MAX][N_MAX];
int pmap[N_MAX][N_MAX];
struct Player {
    int y, x;
    int dir; // 방향
    int pAbil; // 초기 능력치
    int gAbil; // 총 공격력
    int point;
};
Player player[M_MAX];
priority_queue<int> pq[400];
int dy[4] = { -1,0,1,0 };
int dx[4] = { 0,1,0,-1 };

int getIdx(int y, int x) {
    return ((y - 1) * n + x);
}
bool checkGun(int y, int x) {
    int idx = getIdx(y, x);
    if (pq[idx].size() > 0) return true; // 총있으면 true
    else return false; // 총 없으면 false
}

void input() {
    cin >> n >> m >> k;
    FOR(y, 1, n + 1) {
        FOR(x, 1, n + 1) {
            cin >> map[y][x];
            if (map[y][x] > 0) { // 총이 있으면
                int idx = getIdx(y, x);
                pq[idx].push(map[y][x]); // 총을 오름차순으로 정렬
            }
        }
    }
    FOR(i, 1, m + 1) {
        int x = 0, y = 0, d = 0, s = 0;
        cin >> y >> x >> d >> s;
        Player* now = &player[i];
        now->y = y;
        now->x = x;
        now->dir = d;
        now->gAbil = 0;
        now->pAbil = s;
        now->point = 0;
        pmap[y][x] = i;
    }
}

void getGun(Player* now, int y, int x) {
    int idx = getIdx(y, x);
    if (pq[idx].size() == 0) return;
    now->gAbil = pq[idx].top(); // 총 주움
    pq[idx].pop();
}

void putGun(Player* now, int y, int x) {
    if (now->gAbil == 0) return; // 내려둘 총이 없음
    int idx = getIdx(y, x);
    pq[idx].push(now->gAbil); // 총내려둠
    now->gAbil = 0;
}

void matchWinner(Player* winner, Player* loser, int point) {
    winner->point += point;
}

void movePlayer(Player* now, int cy, int cx) {
    int ny = cy;
    int nx = cx;
    int dir = 0;
    FOR(i, 0, 4) {
        dir = (now->dir + i) % 4;
        ny = cy + dy[dir];
        nx = cx + dx[dir];

        if (ny < 1 || nx < 1 || ny > n || nx > n || pmap[ny][nx] > 0) continue;
        break;
    }

    int idx = getIdx(ny, nx);
    if (now->gAbil > 0) { // 만약 총이 있다면
        putGun(now, ny, nx);
    }
    getGun(now, ny, nx);

    pmap[ny][nx] = pmap[now->y][now->x];
    pmap[now->y][now->x] = 0;
    now->y = ny;
    now->x = nx;
    now->dir = dir;
}

void moveAllPlayer() {
    FOR(i, 1, (m + 1)) {
        Player* now = &player[i];
        int playerNum = pmap[now->y][now->x];
        pmap[now->y][now->x] = 0;

        int dir = now->dir;
        int ny = now->y + dy[dir];
        int nx = now->x + dx[dir];
        if (ny < 1) {
            ny = now->y + dy[2];
            dir = 2;
        }
        if (nx < 1) {
            nx = now->x + dx[1];
            dir = 1;
        }
        if (ny > n) {
            ny = now->y + dy[0];
            dir = 0;
        }
        if (nx > n) {
            ny = now->y + dy[3];
            dir = 3;
        }

        int idx = getIdx(ny, nx);

        if (pmap[ny][nx] > 0) { // 타 플레이어 있으면,
            Player* enemy = &player[pmap[ny][nx]];

            int p1_abil = now->pAbil + now->gAbil;
            int p2_abil = enemy->pAbil + enemy->gAbil;
            int point = abs(p1_abil - p2_abil);

            if (p1_abil > p2_abil) { // now 승리
                matchWinner(now, enemy, point);
                if (enemy->gAbil > 0) { // 진사람 총 내려둠
                    putGun(enemy, ny, nx);
                }
                movePlayer(enemy, ny, nx); // 진사람 이동

                if (now->gAbil > 0) { // 이긴 사람, 들고 있는 총내려두고
                    putGun(now, ny, nx);
                }
                getGun(now, ny, nx); // 다시 주움
            }
            else if (p1_abil < p2_abil) { // enemy 승리
                matchWinner(enemy, now, point);
                if (now->gAbil > 0) { // 진사람 총 내려둠
                    putGun(now, ny, nx);
                }
                movePlayer(now, ny, nx); // 진사람 이동

                if (enemy->gAbil > 0) { // 이긴 사람, 들고 있는 총내려두고
                    putGun(enemy, ny, nx);
                }
                getGun(enemy, ny, nx); // 다시 주움
            }
            else if (p1_abil == p2_abil) { // 무승부
                if (now->pAbil > enemy->pAbil) { // now 승리
                    matchWinner(now, enemy, point);
                    if (enemy->gAbil > 0) { // 진사람 총 내려둠
                        putGun(enemy, ny, nx);
                    }
                    movePlayer(enemy, ny, nx); // 진사람 이동 -->

                    if (now->gAbil > 0) { // 이긴 사람, 들고 있는 총내려두고
                        putGun(now, ny, nx);
                    }
                    getGun(now, ny, nx); // 다시 주움
                }
                else if (now->pAbil < enemy->pAbil) { // enemy 승리
                    matchWinner(enemy, now, point);
                    if (now->gAbil > 0) { // 진사람 총 내려둠
                        putGun(now, ny, nx);
                    }
                    movePlayer(now, ny, nx); // 진사람 이동

                    if (enemy->gAbil > 0) { // 이긴 사람, 들고 있는 총내려두고
                        putGun(enemy, ny, nx);
                    }
                    getGun(enemy, ny, nx); // 다시 주움
                }
            }
        }
        else if (pmap[ny][nx] == 0) { // 타 플레이어 없으면,
            if (checkGun(ny, nx)) { // 이동할 위치에 총 있으면 true, 없으면 false
                if (now->gAbil > 0) { // 현재 플레이어가 총을 가지고 있으면
                    putGun(now, ny, nx); // 총내려둠
                }
                getGun(now, ny, nx);
            }
        }

        if (pmap[ny][nx] == 0) { // 아무도 없으면
            pmap[ny][nx] = playerNum;
            now->y = ny;
            now->x = nx;
            now->dir = dir;
        }
    }
}

void sol() {
    int round = 0;
    while (round < k) {
        ++round;
        moveAllPlayer();
    }

    FOR(i, 1, (m + 1)) {
        Player* now = &player[i];
        cout << now->point << ' ';
    }
}

int main() {
    input();
    sol();

    return 0;
}