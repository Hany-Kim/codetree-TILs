#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

#define PII pair<int,int>
#define N_MAX (12 + 1)
#define K_MAX (10 + 1)

int n, k;
int map[N_MAX][N_MAX];
vector<vector<int>> flatten; // 맵을 평평하게
struct NODE {
	int y, x;
	int d;
};
NODE horseList[K_MAX];
int dy[5] = { 0,0,0,-1,1 };
int dx[5] = { 0,1,-1,0,0 };
int gameTurn;
bool endFlag;

int pointToIdx(int y, int x) {
	return ((y - 1) * n) + x;
}
PII idxToPoint(int idx) {
	int y = (idx / n) + 1;
	int x = (idx % n);
	return make_pair(y, x);
}

void input() {
	cin >> n >> k;
	flatten = vector<vector<int>>((n*n) + 1, vector<int>());
	for (int y = 1; y <= n; ++y) {
		for (int x = 1; x <= n; ++x) {
			cin >> map[y][x];
		}
	}

	for (int i = 1; i <= k; ++i) {
		cin >> horseList[i].y >> horseList[i].x >> horseList[i].d;
		int idx = pointToIdx(horseList[i].y, horseList[i].x);
		flatten[idx].push_back(i);
	}
}

bool isWhite(int ny, int nx) {
	if (ny<1 || ny>n || nx<1 || nx>n) return false;
	if (map[ny][nx] == 0) return true;
	return false;
}
bool isBlue(int ny, int nx) {
	if (ny<1 || ny>n || nx<1 || nx>n) return false;
	if (map[ny][nx] == 2) return true;
	return false;
}
bool isRed(int ny, int nx) {
	if (ny<1 || ny>n || nx<1 || nx>n) return false;
	if (map[ny][nx] == 1) return true;
	return false;
}
bool isOutOfRange(int ny, int nx) {
	if (ny<1 || ny>n || nx<1 || nx>n) return true;
	return false;
}

void changeDir(NODE* now) {
	if (now->d == 1) now->d = 2;
	else if (now->d == 2) now->d = 1;
	else if (now->d == 3) now->d = 4;
	else if (now->d == 4) now->d = 3;
}

void moveToWhite(NODE* now, int ny, int nx, int num) {
	int curIdx = pointToIdx(now->y, now->x);
	int nextIdx = pointToIdx(ny, nx);

	int horseFloor = 0;
	for (int i = 0; i < flatten[curIdx].size(); ++i) {
		if (flatten[curIdx][i] == num) {
			horseFloor = i;
			break;
		}
	}

	for (int i = horseFloor; i < flatten[curIdx].size(); ++i) {
		horseList[flatten[curIdx][i]].y = ny;
		horseList[flatten[curIdx][i]].x = nx;
	}

	flatten[nextIdx].insert(flatten[nextIdx].end(), flatten[curIdx].begin() + horseFloor, flatten[curIdx].end());
	flatten[curIdx].erase(flatten[curIdx].begin() + horseFloor, flatten[curIdx].end());
}

void moveToRed(NODE* now, int ny, int nx, int num) {
	int curIdx = pointToIdx(now->y, now->x);
	int nextIdx = pointToIdx(ny, nx);

	int horseFloor = 0;
	for (int i = 0; i < flatten[curIdx].size(); ++i) {
		if (flatten[curIdx][i] == num) {
			horseFloor = i;
			break;
		}
	}

	reverse(flatten[curIdx].begin() + horseFloor, flatten[curIdx].end());

	for (int i = horseFloor; i < flatten[curIdx].size(); ++i) {
		horseList[flatten[curIdx][i]].y = ny;
		horseList[flatten[curIdx][i]].x = nx;
	}

	flatten[nextIdx].insert(flatten[nextIdx].end(), flatten[curIdx].begin() + horseFloor, flatten[curIdx].end());
	flatten[curIdx].erase(flatten[curIdx].begin() + horseFloor, flatten[curIdx].end());
}

void moveAllHorse() {
	for (int i = 1; i <= k; ++i) {
		NODE* now = &horseList[i];

		int ny = now->y + dy[now->d];
		int nx = now->x + dx[now->d];

		if (isWhite(ny, nx)) {
			moveToWhite(now, ny, nx, i);
		}
		else if (isBlue(ny, nx) || isOutOfRange(ny,nx)) {
			changeDir(now);
			ny = now->y + dy[now->d];
			nx = now->x + dx[now->d];

			if (isWhite(ny, nx)) {
				moveToWhite(now, ny, nx, i);
			}
			else if (isBlue(ny, nx) || isOutOfRange(ny, nx)) {
				//changeDir(now);
				ny = now->y;
				nx = now->x;
			}
			else if (isRed(ny, nx)) {
				moveToRed(now, ny, nx, i);
			}
		}
		else if (isRed(ny, nx)) {
			moveToRed(now, ny, nx, i);
		}

		int nextIdx = pointToIdx(ny, nx);
		if (flatten[nextIdx].size() >= 4) {
			endFlag = true;
			return;
		}
	}
}

void sol() {
	while (!endFlag) {
		++gameTurn;

		moveAllHorse();
		if (gameTurn > 1000) {
			gameTurn = -1;
			break;
		}
	}
	cout << gameTurn;
}

int main() {
	//freopen("input.txt", "r", stdin);
	input();

	sol();

	return 0;
}