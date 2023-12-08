#include <iostream>
#include <cmath>
#include <queue>
#include <cstring>
#include <algorithm>

using namespace std;
#define DUMMY 213456789

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
int dy[8] = { -1,0,1,0,-1,-1,1,1 };
int dx[8] = { 0,1,0,-1,-1,1,-1,1 };
int cur_turn;
bool end_flag;

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
    for (int i = 1; i <= P; ++i) {
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

bool cmp(pair<pair<POINT, int>, int> left, pair<pair<POINT, int>, int> right) {
    if (left.second > right.second) return false;
    if (left.second < right.second) return true;

    if (left.first.second > right.first.second) return false;
    if (left.first.second < right.first.second) return true;

    if (left.first.first.y > right.first.first.y) return true;
    if (left.first.first.y < right.first.first.y) return false;

    if (left.first.first.x > right.first.first.x) return true;
    if (left.first.first.x < right.first.first.x) return false;

    return false;
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
    vector<pair<pair<POINT, int>,int>> v; // 좌표, 거리

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
                v.push_back({ { {ny,nx},used[ny][nx] }, get_dist(rudolph.y, rudolph.x, ny, nx)});
            }

            q.push({ ny,nx });
        }
    }

    sort(v.begin(), v.end(), cmp);
    //POINT goal = pq.top().first; // 루돌프가 이동할 가장 가까운 산타위치
    POINT goal = v[0].first.first; // 루돌프가 이동할 가장 가까운 산타위치
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
    int min_dist = get_dist(santa[num].y, santa[num].x, rudolph.y, rudolph.x);;
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
            
        if (direct_num == 0) direct_num = 2;
        else if (direct_num == 2) direct_num = 0;
        else if (direct_num == 1) direct_num = 3;
        else if (direct_num == 3) direct_num = 1;

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

bool isEndGame() {
    for (int i = 1; i <= P; ++i) {
        if (santa[i].isLive == true) return false;
    }
    return true;
}

void move() {
    //cout << "[루돌프 이동]\n";
    move_rudolph();
    if (isEndGame()) end_flag = true;
    //omap();
    //cout << "[산타 이동]\n";
    move_santa();
    //omap();
    if (isEndGame()) end_flag = true;
    //int de = 1;
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
        if (end_flag) break;
        add_score();
        //cout << "생존 산타 +1\n";
        //output();
        //cout << "\n";
        //cout << "\n";
    }

    output();

    return 0;
}