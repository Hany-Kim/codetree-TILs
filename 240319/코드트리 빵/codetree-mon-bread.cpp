#include <iostream>
#include <queue>
#include <algorithm>
#include <climits>
using namespace std;

#define FOR(i,s,e) for(int i=(s); i<(e); ++i)
#define N_MAX (15+1)
#define M_MAX (30+1)

int n, m;
struct NODE
{
    int y, x;
    bool isInside; // 사람이 내부에 있는지
    bool isGoal; // 사람이 편의점에 도착했는지
};
int basecamp[N_MAX][N_MAX]; // 베이스캠프 위치
NODE market_pos[M_MAX]; // 편의점 위치 리스트
NODE basecamp_pos[(N_MAX * N_MAX) - M_MAX]; // 베이스캠프 위치 리스트
int wall[N_MAX][N_MAX]; // 벽
NODE person[M_MAX]; // 사람
int t;
int dy[4] = { -1,0,0,1 };
int dx[4] = { 0,-1,1,0 };


void input() {
    cin >> n >> m;
    int idx = 0;
    FOR(y, 1, (n + 1)) {
        FOR(x, 1, (n + 1)) {
            cin >> basecamp[y][x];

            if (basecamp[y][x] == 1) {
                basecamp_pos[idx].y = y;
                basecamp_pos[idx++].x = x;
            }
        }
    }

    FOR(i, 1, (m + 1)) {
        // 편의점 초기화
        NODE* now = &market_pos[i];
        cin >> now->y >> now->x;

        // 사람 초기화
        person[i].y = -1;
        person[i].x = -1;
        person[i].isInside = false;
        person[i].isGoal = false;
    }
}

bool isPersonInside() {
    FOR(i, 1, (m + 1)) {
        if (person[i].isInside == true) return true;
    }
    return false;
}

void select_next_pos(int pNum) {
    /*
    편의점 -> 사람까지 몇칸 필요한지 계산
    다음 이동할 위치 선정
    */
    queue<pair<int, int>> q;
    int dist[N_MAX][N_MAX] = { 0, };
    NODE start = market_pos[pNum];

    q.push(make_pair(start.y, start.x));
    dist[start.y][start.x] = 1;

    while (!q.empty()) {
        pair<int, int> now = q.front();
        q.pop();

        FOR(i, 0, 4) {
            int ny = now.first + dy[i];
            int nx = now.second + dx[i];

            if (ny<1 || ny>n || nx<1 || nx>n) continue; // 격자 밖
            if (dist[ny][nx] != 0) continue; // 이미 지나온 곳
            if (wall[ny][nx] == 1) continue; // 지나갈 수 없는곳

            dist[ny][nx] = dist[now.first][now.second] + 1;
            q.push(make_pair(ny, nx));
        }
    }


    // 상 좌 우 하 우선순위로 선택 => 의심(?)
    NODE* now_person = &person[pNum];
    int my = INT_MAX;
    int mx = INT_MAX;
    int mdist = INT_MAX;

    // 최단 거리
    FOR(i, 0, 4) {
        int ny = now_person->y + dy[i];
        int nx = now_person->x + dx[i];

        if (ny<1 || ny>n || nx<1 || nx>n) continue; // 격자 밖
        if (dist[ny][nx] == 0) continue; // 기록에 없는 곳

        mdist = min(mdist, dist[ny][nx]);
    }

    // 다음 위치
    FOR(i, 0, 4) {
        int ny = now_person->y + dy[i];
        int nx = now_person->x + dx[i];

        if (ny<1 || ny>n || nx<1 || nx>n) continue; // 격자 밖
        if (mdist != dist[ny][nx]) continue; // 최단거리랑 다르면

        if (my > ny) {
            my = ny;
            mx = nx;
        }
        if (my == ny) {
            if (mx > nx) {
                my = ny;
                mx = nx;
            }
        }
    }

    // 사람 이동
    now_person->y = my;
    now_person->x = mx;
}

void all_person_move() {
    /*
    모든 사람 이동
    */

    FOR(pNum, 1, (m + 1)) {
        if (!person[pNum].isInside) continue; //격자내 있는 사람아니면 pass

        select_next_pos(pNum);
    }
}

void check_person_arrive_market() {
    /*
    편의점에 도착한 사람 있는지
    */

    FOR(pNum, 1, (m + 1)) {
        NODE* now_market = &market_pos[pNum];
        NODE* now_person = &person[pNum];

        if (now_market->y == now_person->y && now_market->x == now_person->x) {
            now_person->isGoal = true;
        }
    }

    FOR(pNum, 1, (m + 1)) {
        NODE* now_person = &person[pNum];

        if (now_person->isGoal == true) { // 도착했다면
            now_person->isInside = false; // 내보내고

            wall[now_person->y][now_person->x] = 1; // 벽세움
        }
    }
}

bool find_basecamp(pair<pair<int, int>, int> left, pair<pair<int, int>, int> right) {
    if (left.second < right.second) return true;
    if (left.second > right.second) return false;

    if (left.first.first < right.first.first) return true;
    if (left.first.first > right.first.first) return false;
    
    if (left.first.second < right.first.second) return true;
    if (left.first.second > right.first.second) return false;

    return false;
}

pair<int, int> select_basecamp(int pNum) {
    /* 
    bfs 탐색으로 
    편의점 -> 베이스캠프 찾고
    찾은 베이스캠프 중 정렬해서 가장 가깝고 row작고 col작은 베이스캠프 리턴
    */

    queue<pair<int, int>> q;
    int dist[N_MAX][N_MAX] = { 0, };
    NODE start = market_pos[pNum];
    vector<pair<pair<int, int>, int>> v;

    q.push(make_pair(start.y, start.x));
    dist[start.y][start.x] = 1;

    while (!q.empty()) {
        pair<int, int> now = q.front();
        q.pop();

        FOR(i, 0, 4) {
            int ny = now.first + dy[i];
            int nx = now.second + dx[i];

            if (ny<1 || ny>n || nx<1 || nx>n) continue; // 격자 밖
            if (dist[ny][nx] != 0) continue; // 이미 지나온 곳
            if (wall[ny][nx] == 1) continue; // 지나갈 수 없는곳

            dist[ny][nx] = dist[now.first][now.second] + 1;
            if (basecamp[ny][nx] == 1) {
                v.push_back(make_pair(make_pair(ny, nx), dist[ny][nx]));
            }
            q.push(make_pair(ny, nx));
        }
    }

    sort(v.begin(), v.end(), find_basecamp);

    return make_pair(v[0].first.first, v[0].first.second);
}

void person_enter_basecamp(int pNum) {
    /*
    t번째 사람 베이스캠프로 들어가기
    */
    pair<int,int> p = select_basecamp(pNum); // 선택한 베이스캠프
    person[pNum].y = p.first;
    person[pNum].x = p.second;
    person[pNum].isInside = true; // 들어갔다.

    wall[p.first][p.second] = 1; // 벽 세워짐
}

bool all_pass() {
    FOR(i, 1, (m + 1)) {
        NODE* now = &person[i];

        if (now->isGoal == true && now->isInside == false) {

        }
        else {
            return false;
        }
    }
    return true;
}

void sol() {
    t = 1;
    while (1) {
        if (isPersonInside() == true) {
            all_person_move();
            check_person_arrive_market();
        }
        if (t <= m) {
            person_enter_basecamp(t);
        }

        if (all_pass() == true) break;
        ++t; // 1분 증가
    }
    cout << t;
}

int main() {
    // 여기에 코드를 작성해주세요.
    input();
    sol();

    return 0;
}