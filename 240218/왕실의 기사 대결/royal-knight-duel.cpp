#include <iostream>
#include <cstring>
using namespace std;

#define FOR(i,s,e) for(int i=s; i<e; ++i)
#define L_MAX (40 + 1)
#define KNIGHTS_MAX (30 + 1)
#define KING_CMD_NUM 100

int L, N, Q;
int map[L_MAX][L_MAX];
int knight_map[L_MAX][L_MAX];
int tmp_knight_map[L_MAX][L_MAX];
struct KNIGHT {
    int r, c, h, w, k;
    int damage_sum;
    bool isMove;
};
KNIGHT knights[KNIGHTS_MAX];
struct KING {
    int i, d;
};
KING commands[KING_CMD_NUM];
int dy[4] = { -1,0,1,0 };
int dx[4] = { 0,1,0,-1 };

void input() {
    cin >> L >> N >> Q;
    FOR(y, 1, (L + 1)) {
        FOR(x, 1, (L + 1)) {
            cin >> map[y][x];
        }
    }
    FOR(i, 1, (N + 1)) {
        KNIGHT* now = &knights[i];
        cin >> now->r >> now->c >> now->h >> now->w >> now->k;

        FOR(y, now->r, (now->r + now->h)) {
            FOR(x, now->c, (now->c + now->w)) {
                knight_map[y][x] = i;
            }
        }
    }
    FOR(i, 0, Q) {
        KING* now = &commands[i];
        cin >> now->i >> now->d;
    }
}

void move_knight(int num, int dir) {
    KNIGHT* now = &knights[num];
    int ny = now->r + dy[dir];
    int nx = now->c + dx[dir];

    FOR(y, now->r, (now->r + now->h)) {
        FOR(x, now->c, (now->c + now->w)) {
            if (tmp_knight_map[y][x] == num) {
                tmp_knight_map[y][x] = 0;
            }
        }
    }

    FOR(y, ny, (ny + now->h)) {
        FOR(x, nx, (nx + now->w)) {
            if (tmp_knight_map[y][x] == 0) {
                tmp_knight_map[y][x] = num;
            }
        }
    }
}

bool can_move_knight(int num, int dir) { // 기사번호, 방향
    KNIGHT* now = &knights[num];
    int ny = now->r + dy[dir];
    int nx = now->c + dx[dir];

    if ((ny < 1) || ((ny + now->h - 1) > L) || (nx < 1) || ((nx + now->w - 1) > L)) return false; // 영역을 벗어남

    FOR(y, ny, (ny + now->h)) {
        FOR(x, nx, (nx + now->w)) {
            if (map[y][x] == 2) return false; // 벽에 막힘
        }
    }

    FOR(y, ny, (ny + now->h)) {
        FOR(x, nx, (nx + now->w)) {
            if ((knight_map[y][x] != 0) && (knight_map[y][x] != num)) { // 자신이 아닌 다른 기사가 있음
                if (can_move_knight(knight_map[y][x], dir) == false) return false; // 다른 기사가 이동 못함
            }
        }
    }

    move_knight(num, dir);

    return true;
}

void update_knight_info(int num) {
    bool visit[KNIGHTS_MAX] = { 0, };

    FOR(y, 1, (L + 1)) { // 좌표 갱신
        FOR(x, 1, (L + 1)) {
            int now = knight_map[y][x];
            if (now == 0) continue;
            if (visit[now] == true) continue;
            visit[now] = true;


            if (knights[now].k <= 0) continue; // 이미 체스판에서 사라진 기사
            if (knights[now].r != y || knights[now].c != x) knights[now].isMove = true;
            knights[now].r = y;
            knights[now].c = x;
        }
    }

    FOR(i, 1, (N + 1)) { // 피해계산
        KNIGHT* now = &knights[i];
        if (i == num) continue; // 명령받은 기사
        if (now->k <= 0) continue; // 이미 체스판에서 사라진 기사
        if (now->isMove == false) continue; // 이동한적 없는 기사라면
        now->isMove = false;

        int trap_cnt = 0;
        FOR(y, now->r, (now->r + now->h)) {
            FOR(x, now->c, (now->c + now->w)) {
                if (map[y][x] == 1) ++trap_cnt;
            }
        }

        now->k -= trap_cnt;
        now->damage_sum += trap_cnt;

        if (now->k <= 0) {
            FOR(y, now->r, (now->r + now->h)) {
                FOR(x, now->c, (now->c + now->w)) {
                    knight_map[y][x] = 0;
                }
            }
        }
    }
}

void sol() {
    FOR(cmd_num, 0, Q) {
        if (knights[commands[cmd_num].i].k <= 0) continue; // 체스판에서 사라진 기사
        memcpy(tmp_knight_map, knight_map, sizeof(knight_map)); // 백업
        if (can_move_knight(commands[cmd_num].i, commands[cmd_num].d) == false) continue; // 명령 받은 기사가 움직일 수 있는지
        memcpy(knight_map, tmp_knight_map, sizeof(knight_map)); // 갱신
        update_knight_info(commands[cmd_num].i);
    }

    int ans = 0;
    FOR(i, 1, (N + 1)) {
        KNIGHT* now = &knights[i];
        if (now->k <= 0) continue;

        ans += now->damage_sum;
    }

    cout << ans;
    int de = 1;
}

int main() {
    // 여기에 코드를 작성해주세요.
    //freopen("input.txt", "r", stdin);

    input();
    sol();

    return 0;
}