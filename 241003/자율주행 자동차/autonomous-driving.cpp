#include <iostream>
#include <algorithm>
using namespace std;

#define N_MAX 50
#define M_MAX 50

struct CAR {
	int y, x, d;
};
int N, M;
int mmap[N_MAX][M_MAX];
int visited[N_MAX][M_MAX];
CAR car;
int dy[4] = { -1, 0, 1, 0 };
int dx[4] = { 0, 1, 0, -1 };
int maxArea;
bool endFlag;

int isVisitedLeftSide(CAR car) {
	int dir = car.d;

	for (int i = 0; i < 4; ++i) {
		dir -= 1;
		if (dir < 0) dir = 3;
		int ny = car.y + dy[dir];
		int nx = car.x + dx[dir];

		if (ny < 0 || ny >= N || nx < 0 || nx >= M) continue;
		if (visited[ny][nx] == 1) continue;
		if (mmap[ny][nx] == 1) continue;

		return dir;
	}

	return dir;
}

bool isEnd(CAR car, int dir) {
	bool isPersonRoad = false;

	int d = 0;
	if (dir == 0) d = 2;
	else if (dir == 1) d = 3;
	else if (dir == 2) d = 0;
	else if (dir == 3) d = 1;

	int ny = car.y + dy[d];
	int nx = car.x + dx[d];

	if (ny < 0 || ny >= N || nx < 0 || nx >= M) {}
	else {
		if (mmap[ny][nx] == 1) { isPersonRoad = true; }
	}

	bool canMove = false;
	for (int i = 0; i < 4; ++i) {
		if (d == i) continue;

		ny = car.y + dy[i];
		nx = car.x + dx[i];

		if (ny < 0 || ny >= N || nx < 0 || nx >= M) continue;
		if (visited[ny][nx] == 1) continue;
		if (mmap[ny][nx] == 1) continue;

		canMove = true;
		break;
	}

	if (isPersonRoad && !canMove) return true;
	return false;
}

int getArea() {
	int cnt = 0;
	for (int y = 0; y < N; ++y) {
		for (int x = 0; x < M; ++x) {
			if (visited[y][x] == 1) cnt++;
		}
	}
	return cnt;
}

void dfs(int area, CAR pos) {
	if (endFlag) return;
	int ar = getArea();
	maxArea = max(ar, maxArea);

	int dir = isVisitedLeftSide(pos);
	int ny = pos.y + dy[dir];
	int nx = pos.x + dx[dir];

	if (ny < 0 || ny >= N || nx < 0 || nx >= M) return;
	if (visited[ny][nx] == 1) return;
	if (mmap[ny][nx] == 1) return;

	// 이동
	CAR next;
	next.y = ny;
	next.x = nx;
	next.d = dir;
	visited[ny][nx] = 1;
	dfs(area + 1, next);

	// 후진
	int d = 0;
	if (dir == 0) d = 2;
	else if (dir == 1) d = 3;
	else if (dir == 2) d = 0;
	else if (dir == 3) d = 1;
	
	int by = pos.y + dy[d];
	int bx = pos.x + dx[d];

	if (by < 0 || by >= N || bx < 0 || bx >= M) return;
	if (visited[by][bx] == 1) return;
	if (mmap[by][bx] == 1) return;

	CAR back;
	back.y = by;
	back.x = bx;
	back.d = d;
	visited[by][bx] = 1;
	dfs(area + 1, back);


	if (isEnd(pos, dir)) endFlag = true;
	if (endFlag) return;
	int de = 1;
}

void sol() {
	visited[car.y][car.x] = 1;
	dfs(1, car);
	cout << maxArea;
}

void in() {
	cin >> N >> M;
	cin >> car.y >> car.x >> car.d;
	for (int y = 0; y < N; ++y) {
		for (int x = 0; x < M; ++x) {
			cin >> mmap[y][x];
		}
	}
}

int main() {
	in();
	sol();
	return 0;
}