#include <iostream>
#include <cmath>
#include <queue>
#include <cstring>

using namespace std;
#define DUMMY 213456789


/*
상황
1번부터 P번까지 P 명의 산타들이 크리스마스 이브를 준비하던 중, 
산타의 주요 수송수단인 루돌프가 반란을 일으켰습니다. 
루돌프는 산타들을 박치기하여 산타의 배달을 방해
산타들은 루돌프를 잡아는다.
*/

// 게임 구성
// N*N 격자 (r,c) 좌상단(1,1)
// M개의 턴동안 루돌프와 이동 가능한 모든 산타 1번씩 이동
// 두칸 사이의 거리 (r1 - r2)^2 + (c1 - c2)^2

// 루돌프 움직임
// 루돌프는 가장 가까운 탈락하지 않은 산타로 1칸 돌진
// if 가까운 산타 2명 r좌표가 큰 산타, r좌표 동일하다면 c좌표 큰 산타
// 루돌프는 상하좌우, 대각성 8방향이동가능

// 산타의 움직임
// 1~P번까지 순서대로 움직임
// 기절했거나 이미 게임에서 탈락했다면 움직일 수 없음
// 산타는 루돌프에게 거리가 가장 가까워지는 방향으로 1칸 이동
// 산타는 다른 산타가 있는 칸이나 게임판 밖으로 움직일 수 없다.
// 움직일 수 있는 칸이 없다면 산타는 움직이지 않는다.
// 움직일 수 있는 칸이 있더라도 만약 루돌프로부터 가까워질 수 있는 방법이 없다면 산타는 움직이지 않는다.
// 산타는 상하좌우 4방향 이동 가능, 이동 가능 방향 여러개라면 상하좌우 우선순위에 맞춰 움직

// 충돌
// """산타와 루돌프가 같은 칸"""
// 루돌프가 움직여서 충돌 발생 경우, 해당 산타는 C만큼의 점수를 얻는다. 동시에 산타는 루돌프가 이동해온 방향으로 C칸만큼 밀린다.
// 산타가 움직여서 충돌 발생 경우, 해당 산타는 D만큼의 점수를 얻는다. 동시에 자신이 이동해온 반대방향으로 D칸만큼 밀린다.
// 밀려나는 것은 포물선 모양을 그리며 밀리기 때문에, "도중에 충돌이 일어나지 않는다."
// 밀려난 위치가 게임판 밖이라면 산타는 탈락
// 밀려난 위치에 다른 산타가 있다면 상호작용 발생

// 상호작용
// """루돌프와 충돌후 착지한 칸에서만""" 상호작용 발생
// 다른 산타 있다면, 그 산타는 1칸 해당 방향으로 밀림
// 그 옆에 산타가 있다면 연쇄적으로 1칸씩 밀림
// 게임판 밖으로 밀리면 탈락

// 기절
// 산타는 루돌프와 충돌 후 기절
// 현재 k턴이라면 (k+1)턴까지 기절, (k+2)턴부터 다시 정상
// 기절한 산타는 움직일 수 없음
// 루돌프는 기절한 산타를 돌진대상으로 선택할 수 있음

// 게임종료
// M턴이 지남
// P명의 산타 모두 탈락
// 매턴 이후 아직 탈락하지 않은 산타들에게 1점씩 부여

int N, M, P, C, D;
struct POINT {
    int y;
    int x;
};
POINT rudolph;

struct INFO {
    int y;
    int x;
    bool isLive;
    int isSleep;
    int score;
};
INFO santa[31];
int map[51][51];
int dy[8] = { -1,1,0,0,-1,-1,1,1 };
int dx[8] = { 0,0,-1,1,-1,1,-1,1 };
int cur_turn;

void omap() {
    for (int i = 1; i <= P; ++i) {
        cout << i << "번 산타 : " << santa[i].score << "점 / 기절 시간 : " << santa[i].isSleep << '\n';
    }
    for (int y = 1; y <= N; ++y) {
        for (int x = 1; x <= N; ++x) {
            cout << map[y][x] << ' ';
        }
        cout << '\n';
    }
    cout << '\n';
}

void input() {
    cin >> N >> M >> P >> C >> D;
    cin >> rudolph.y >> rudolph.x;
    map[rudolph.y][rudolph.x] = -1; // 루돌프 맵상 -1로 표시
    for (register int i = 1; i <= P; ++i) {
        int num = 0;
        cin >> num;
        cin >> santa[num].y >> santa[num].x;
        map[santa[num].y][santa[num].x] = num; // 산타는 정수로 표시
        santa[num].isLive = true;
        santa[num].isSleep = 0;
        santa[num].score = 0;
    }
}

int get_dist(int r1, int c1, int r2, int c2) {
    int dist = pow(r1 - r2, 2) + pow(c1 - c2, 2);
    return dist;
}

bool operator<(pair<POINT, int> left, pair<POINT, int> right) {
    if (left.second < right.second) return false;
    if (left.second > right.second) return true;

    if (left.first.y < right.first.y) return true;
    if (left.first.y > right.first.y) return false;

    if (left.first.x < right.first.x) return true;
    if (left.first.x > right.first.x) return false;

    return false;
}

void interaction(int y, int x, int direct_num, int times) {
    int num = map[y][x];
    int ny = y + (dy[direct_num]);
    int nx = x + (dx[direct_num]);

    if (ny < 1 || ny > N || nx < 1 || nx > N) {
        santa[num].isLive = false;
        map[santa[num].y][santa[num].x] = 0;
        return;
    }

    if ((map[ny][nx] > 0) && (map[ny][nx] != num)) {
        // 상호작용
        interaction(ny, nx, direct_num, times);
    }

    map[santa[num].y][santa[num].x] = 0;
    santa[num].y = ny;
    santa[num].x = nx;
    map[ny][nx] = num;

    return;
}

void bfs_rudolph_to_santa() {
    int used[51][51] = { 0, };
    for (int y = 1; y <= N; ++y) {
        for (int x = 1; x <= N; ++x) {
            used[y][x] = DUMMY;
        }
    }

    queue<POINT> q;
    priority_queue<pair<POINT, int>> pq; // 좌표, 거리

    q.push(rudolph);
    used[rudolph.y][rudolph.x] = 0;

    while (!q.empty()) {
        POINT now = q.front();
        q.pop();

        for (int i = 0; i < 8; ++i) {
            int ny = now.y + dy[i];
            int nx = now.x + dx[i];

            if (ny < 1 || ny > N || nx < 1 || nx > N) continue;

            if (used[ny][nx] < (used[now.y][now.x] + 1)) continue;
            used[ny][nx] = used[now.y][now.x] + 1;
            if ((map[ny][nx] > 0) && (santa[map[ny][nx]].isLive == true)) { // 산타와 만났다.
                pq.push({ {ny,nx},used[ny][nx] });
            }

            q.push({ ny,nx });
        }
    }

    POINT goal = pq.top().first; // 루돌프가 이동할 가장 가까운 산타위치
    POINT next = { 0,0 }; // 루돌프가 이동할 위치
    int min_dist = DUMMY;
    int direct_num = 0;
    for (int i = 0; i < 8; ++i) { 
        int ny = rudolph.y + dy[i];
        int nx = rudolph.x + dx[i];

        if (ny < 1 || ny > N || nx < 1 || nx > N) continue;
        if (used[ny][nx] != 1)continue;

        int cur_dist = get_dist(ny, nx, goal.y, goal.x);
        if (min_dist > cur_dist) {
            min_dist = cur_dist;
            next.y = ny;
            next.x = nx;
            direct_num = i;
        }
    }

    if (map[next.y][next.x] > 0) { // 루 -> 산 충돌
        int num = map[next.y][next.x];
        santa[num].score += C;
        
        int ny = santa[num].y + (dy[direct_num] * C);
        int nx = santa[num].x + (dx[direct_num] * C);
        if (ny < 1 || ny > N || nx < 1 || nx > N) {
            santa[num].isLive = false;
            map[santa[num].y][santa[num].x] = 0;
        }
        else {
            if ((map[ny][nx] > 0) && (map[ny][nx] != num)) {
                // 상호작용
                interaction(ny, nx, direct_num, C);
            }
            map[santa[num].y][santa[num].x] = 0;
            santa[num].y = ny;
            santa[num].x = nx;
            map[ny][nx] = num;
        }

        santa[num].isSleep = cur_turn + 2;
    }

    map[rudolph.y][rudolph.x] = 0;
    map[next.y][next.x] = -1;
    rudolph.y = next.y;
    rudolph.x = next.x;
}

void move_rudolph() {
    bfs_rudolph_to_santa();
}

void santa_to_rudolph(int num) {
    POINT next = { santa[num].y, santa[num].x }; // 산타가 이동할 위치
    int min_dist = DUMMY;
    int direct_num = 0;

    for (int i = 0; i < 4; ++i) {
        int ny = santa[num].y + dy[i];
        int nx = santa[num].x + dx[i];
        if (ny < 1 || ny > N || nx < 1 || nx > N) continue;
        if ((map[ny][nx] > 0) && (map[ny][nx] != num)) continue;

        int cur_dist = get_dist(ny, nx, rudolph.y, rudolph.x);
        if (min_dist > cur_dist) {
            min_dist = cur_dist;
            next.y = ny;
            next.x = nx;
            direct_num = i;
        }
    }

    if ((rudolph.y == next.y) && (rudolph.x == next.x)) { // 산 -> 루 충돌
        // 충돌
        santa[num].score += D;
            
        if (direct_num == 0) direct_num = 1;
        else if (direct_num == 1) direct_num = 0;
        else if (direct_num == 2) direct_num = 3;
        else if (direct_num == 3) direct_num = 2;

        int ny = next.y + (dy[direct_num] * D);
        int nx = next.x + (dx[direct_num] * D);
        if (ny < 1 || ny > N || nx < 1 || nx > N) {
            santa[num].isLive = false;
            map[santa[num].y][santa[num].x] = 0;
        }
        else {
            if ((map[ny][nx] > 0) && (map[ny][nx] != num)) {
                // 상호작용
                interaction(ny, nx, direct_num, D);
            }
            map[santa[num].y][santa[num].x] = 0;
            santa[num].y = ny;
            santa[num].x = nx;
            map[ny][nx] = num;
        }

        santa[num].isSleep = cur_turn + 2;
    }
    else if ((map[next.y][next.x] > 0) && (map[next.y][next.x] != num)) { // 다른 산타만나면 무시
        
    }
    else {
        map[santa[num].y][santa[num].x] = 0;
        santa[num].y = next.y;
        santa[num].x = next.x;
        map[next.y][next.x] = num;
    }



    int dde = 1;
}

void move_santa() {
    for (int i = 1; i <= P; ++i) {
        bool isSleep = true;
        if (santa[i].isSleep == cur_turn) {
            santa[i].isSleep = 0;
            isSleep = false;
        }
        else if (santa[i].isSleep == 0) isSleep = false;

        if ((santa[i].isLive == false) || (isSleep == true)) continue;
        santa_to_rudolph(i);
    }
}

void move() {
    //cout << "[루돌프 이동]\n";
    move_rudolph();
    //omap();
    //cout << "[산타 이동]\n";
    move_santa();
    //omap();
    int de = 1;
}

void add_score() {
    for (int i = 1; i <= P; ++i) {
        if (santa[i].isLive == false) continue;
        santa[i].score += 1;
    }
}

void output() {
    for (int i = 1; i <= P; ++i) {
        cout << santa[i].score << " ";
    }
}

int main() {
    // 여기에 코드를 작성해주세요.
    //freopen("input.txt", "r", stdin);
    input();
    //omap();
    for (int i = 1; i <= M; ++i) {
        cur_turn = i;
        //cout << "현재 턴 : " << i << "\n";
        move();
        add_score();
    }

    output();

    return 0;
}