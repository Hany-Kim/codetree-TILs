#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <queue>
using namespace std;

#define N_MAX (5+1)
#define FOR(i,s,e) for(int i=(s); i<(e); ++i)

int n, m, k;
int map[N_MAX][N_MAX];
int pmap[N_MAX][N_MAX];
priority_queue<int> pq[401];
struct Player {
    int y, x, d, pAbil, gAbil;
    int point;
};
Player player[31];
int dy[4] = { -1,0,1,0 };
int dx[4] = { 0,1,0,-1 };

int getIdx(int y, int x) {
    int idx = ((y - 1) * n) + x;
    return idx;
}
int getNextPosY(Player* now) {
    int ny = now->y + dy[now->d];

    if (ny < 1) { // 위로 벗어남
        ny = now->y + dy[2];
        now->d = 2;
    }
    if (ny > n) { // 아래로 벗어남
        ny = now->y + dy[0];
        now->d = 0;
    }

    return ny;
}
int getNextPosX(Player* now) {
    int nx = now->x + dx[now->d];

    if (nx < 1) { // 좌로 벗어남
        nx = now->x + dx[1];
        now->d = 1;
    }
    if (nx > n) { // 우로 벗어남
        nx = now->x + dx[3];
        now->d = 3;
    }

    return nx;
}
int checkGun(int idx) {
    return pq[idx].size();
}
bool isGun(Player* now) {
    if (now->gAbil > 0) return true;
    else return false;
}
void putGun(Player* now, int y, int x) {
    int idx = getIdx(y, x);
    pq[idx].push(now->gAbil);
    now->gAbil = 0;
}
void getGun(Player* now, int y, int x) {
    int idx = getIdx(y, x);
    now->gAbil = pq[idx].top();
    pq[idx].pop();
}

void getPoint(Player* winner, int score) {
    winner->point += score;
}
void moveLooser(Player* looser, int sy, int sx, int pNum) {
    int pnum = pmap[looser->y][looser->x];
    int ny = 0;
    int nx = 0;
    FOR(i, 0, 4) {
        ny = sy + dy[(looser->d + i) % 4];
        nx = sx + dx[(looser->d + i) % 4];
        if (ny<1 || ny>n || nx<1 || nx>n) continue;
        if (pmap[ny][nx] > 0) continue;
        looser->d = (looser->d + i) % 4;
        break;
    }
    pmap[ny][nx] = pNum;
    int idx = getIdx(ny, nx);
    if (checkGun(idx)) {
        if (isGun(looser)) {
            putGun(looser, ny, nx);
        }
        getGun(looser, ny, nx);
    }
    looser->y = ny;
    looser->x = nx;
}

void sol() {
    int round = 0;
    while (round < k) {
        FOR(pNum, 1, (m + 1)) {
            Player* now = &player[pNum];

            int ny = getNextPosY(now);
            int nx = getNextPosX(now);


            if (pmap[ny][nx] == 0) { // 이동할 위치에 타 플레이어 없음
                pmap[ny][nx] = pNum; // 다음 위치로 보냄
                pmap[now->y][now->x] = 0;
                int idx = getIdx(ny, nx);
                if (checkGun(idx) > 0) { // 해당 칸에 총이 있음
                    if (isGun(now)) { // 이미 총을 가지고 있음
                        putGun(now, ny, nx);
                    }
                    getGun(now, ny, nx);
                }

                // 좌표 갱신
                now->y = ny;
                now->x = nx;
            }
            else { // 이동할 위치에 플레이어 있음
                int nowPnum = pmap[now->y][now->x];
                pmap[now->y][now->x] = 0;

                Player* enemy = &player[pmap[ny][nx]]; // 다음 위치에 있던 사람이 적
                int ePnum = pmap[ny][nx];
                pmap[ny][nx] = 0;

                int nowAbil = now->pAbil + now->gAbil;
                int enemyAbil = enemy->pAbil + enemy->gAbil;
                int score = abs(nowAbil - enemyAbil);

                if (nowAbil > enemyAbil) { // now 승
                    getPoint(now, score);
                    if (isGun(enemy)) putGun(enemy, ny, nx);
                    moveLooser(enemy, ny, nx, ePnum);
                    if (isGun(now)) {
                        putGun(now, ny, nx);
                        getGun(now, ny, nx);
                    }
                    pmap[ny][nx] = nowPnum;
                    now->y = ny;
                    now->x = nx;
                }
                else if (nowAbil < enemyAbil) { // enemy 승
                    getPoint(enemy, score);
                    if (isGun(now)) putGun(now, ny, nx);
                    moveLooser(now, ny, nx, nowPnum);
                    if (isGun(enemy)) {
                        putGun(enemy, ny, nx);
                        getGun(enemy, ny, nx);
                    }
                    pmap[ny][nx] = ePnum;
                }
                else if (nowAbil == enemyAbil) { // 무승부
                    if (now->pAbil > enemy->pAbil) { // now 승
                        getPoint(now, score);
                        if (isGun(enemy)) putGun(enemy, ny, nx);
                        moveLooser(enemy, ny, nx, ePnum);
                        if (isGun(now)) {
                            putGun(now, ny, nx);
                            getGun(now, ny, nx);
                        }
                        pmap[ny][nx] = nowPnum;
                        now->y = ny;
                        now->x = nx;
                    }
                    if (now->pAbil < enemy->pAbil) { // enemy 승
                        getPoint(enemy, score);
                        if (isGun(now)) putGun(now, ny, nx);
                        moveLooser(now, ny, nx, nowPnum);
                        if (isGun(enemy)) {
                            putGun(enemy, ny, nx);
                            getGun(enemy, ny, nx);
                        }
                        pmap[ny][nx] = ePnum;
                    }
                }
            }
        }
        ++round; // 라운드 종료
    }
    FOR(i, 1, m + 1) {
        Player* now = &player[i];
        cout << now->point << ' ';
    }
}

void input() {
    cin >> n >> m >> k;
    FOR(y, 1, (n + 1)) {
        FOR(x, 1, (n + 1)) {
            cin >> map[y][x];
            if (map[y][x] > 0) {
                int idx = getIdx(y, x);
                pq[idx].push(map[y][x]);
            }
        }
    }
    FOR(i, 1, (m + 1)) {
        Player* now = &player[i];
        cin >> now->y >> now->x >> now->d >> now->pAbil;
        now->gAbil = 0;
        now->point = 0;
        pmap[now->y][now->x] = i;
    }
}

int main() {
    input();
    sol();
    return 0;
}