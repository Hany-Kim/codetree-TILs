#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

using namespace std;

#define N_MAX (50)
#define M_MAX (50)
#define FOR(i,s,e) for(int i=(s); i<(e); ++i)
#define PII pair<int,int>

int n, m;
int map[N_MAX][M_MAX];
int used[N_MAX][M_MAX];
int dy[4] = { -1,0,1,0 };
int dx[4] = { 0,1,0,-1 };
struct CAR {
	PII pos; // 위치
	int dir; // 현재 방향
};
CAR car;
bool endFlag;

void input() {
	int y = 0, x = 0, d = 0;
	cin >> n >> m;
	cin >> y >> x >> d;
	FOR(y, 0, n) {
		FOR(x, 0, m) {
			cin >> map[y][x];
		}
	}

	car.pos = make_pair(y, x);
	car.dir = d;
}

void moveCar(PII pos, int dir) {
	// 자동차 이동
	car.pos.first = pos.first;
	car.pos.second = pos.second;
	car.dir = dir;
}

bool isMoveBack() {
	int d = 0;
	if (car.dir == 0) d = 2;
	else if (car.dir == 1) d = 3;
	else if (car.dir == 2) d = 0;
	else if (car.dir == 3) d = 1;

	int ny = car.pos.first + dy[d];
	int nx = car.pos.second + dx[d];

	if (ny < 0 || ny >= n || nx < 0 || nx >= m) return false; // 맵 밖
	if (map[ny][nx] == 1) return false; // 인도

	return true;
}

void dfs(int area) {
	//FOR(i, 0, 3) {
	for(int i = 0; i > -4; i--) {
		int d = i;
		if (d < 0) d += 4;

		int ny = car.pos.first + dy[d];
		int nx = car.pos.second + dx[d];

		if (ny < 0 || ny >= n || nx < 0 || nx >= m) continue; // 맵 밖
		if (map[ny][nx] == 1) continue; // 인도
		if (used[ny][nx] == 1) continue; // 지나온 길

		// 현재 위치 기록
		int sy = car.pos.first;
		int sx = car.pos.second;
		used[sy][sx] = 1;

		moveCar(make_pair(ny, nx), d);

		dfs(area + 1);
		if (endFlag) return;
	}

	if (isMoveBack()) {
		dfs(area);
		if (endFlag) return;
	}
	else {
		endFlag = true;
		cout << area;
		return;
	}
}

bool isLeft(int d) {
	int ny = car.pos.first + dy[d];
	int nx = car.pos.second + dx[d];

	if (ny < 0 || ny >= n || nx < 0 || nx >= m) return false; // 맵 밖
	if (map[ny][nx] == 1) return false; // 인도
	if (used[ny][nx] == 1) return false; // 지나온 길

	return true;
}

void sol() {
	//dfs(1);

	while (!endFlag) {
		// 현재 위치 기록
		int sy = car.pos.first;
		int sx = car.pos.second;
		used[sy][sx] = 1;

		bool isMove = false;
		for (int i = -1; i >= -4; --i) {
			if (isMove) continue;

			int d = car.dir + i;
			if (d < 0) d += 4;

			if (isLeft(d)) { // 갈 수 있냐? 
				// 왼쪽 간적 없으면 좌회전 후 이동

				int ny = car.pos.first + dy[d];
				int nx = car.pos.second + dx[d];

				if (ny < 0 || ny >= n || nx < 0 || nx >= m) continue; // 맵 밖
				if (map[ny][nx] == 1) continue; // 인도
				if (used[ny][nx] == 1) continue; // 지나온 길

				moveCar(make_pair(ny, nx), d);
				isMove = true;
			} 
		}
		if(!isMove) {
			if (isMoveBack()) {
				// 후진할 수 있냐
				int d = 0;
				if (car.dir == 0) d = 2;
				else if (car.dir == 1) d = 3;
				else if (car.dir == 2) d = 0;
				else if (car.dir == 3) d = 1;

				int ny = car.pos.first + dy[d];
				int nx = car.pos.second + dx[d];

				moveCar(make_pair(ny, nx), car.dir);
			}
			else {
				endFlag = true;
			}
		}

	}

	int area = 0;
	FOR(y, 0, n) {
		FOR(x, 0, m) {
			if (used[y][x] == 1) ++area;
		}
	}
	cout << area;
}

int main() {
	//freopen("input.txt", "r", stdin);
	input();
	sol();
	return 0;
}