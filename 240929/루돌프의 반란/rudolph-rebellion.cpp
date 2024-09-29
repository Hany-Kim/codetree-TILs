#include <iostream>
#include <map>
#include <math.h>
#include <algorithm>
#include <vector>
#include <climits>
using namespace std;

#define N_MAX (50 + 1)
#define P_MAX (30 + 1)

struct SANTA {
	int y, x;
	int score;
	int isStun; // 기절했는지
	bool isDied; // 탈락했는지
};
struct NODE {
	int y, x;
	int dist;
	int num;
};
int N, M, P, C, D;
int mmap[N_MAX][N_MAX];
//NODE santas[P_MAX];
map<int, SANTA> santas;
NODE rudolph;
int ry[8] = { -1,-1,-1,0,0,1,1,1 };
int rx[8] = { -1,0,1,-1,1,-1,0,1 };
int maskedAllSanta;
int checkedDeadSanta;
int dy[4] = { -1, 0, 1, 0 };
int dx[4] = { 0, 1, 0, -1 };
int santaSeq[P_MAX];

int getDistBetweenBlocks(int y1, int x1, int y2, int x2) {
	int dist = pow(y1 - y2, 2) + pow(x1 - x2, 2);
	return dist;
}

bool santaSort(NODE left, NODE right) {
	if (left.dist < right.dist) return true;
	if (left.dist > right.dist) return false;

	if (left.y > right.y) return true;
	if (left.y < right.y) return false;

	if (left.x > right.x) return true;
	if (left.x < right.x) return false;

	return false;
}

int getTargetSantaNum() {
	vector<NODE> v;
	int dist = INT_MAX;
	int santaNum = 0;

	for (map<int, SANTA>::iterator itr = santas.begin(); itr != santas.end(); itr++) {
		if (itr->second.isDied) continue;
		int curDist = getDistBetweenBlocks(rudolph.y, rudolph.x, itr->second.y, itr->second.x);

		NODE now;
		now.y = itr->second.y;
		now.x = itr->second.x;
		now.dist = curDist;
		now.num = itr->first;

		v.push_back(now);
	}

	sort(v.begin(), v.end(), santaSort);

	if (v.size() > 0) santaNum = v[0].num;

	return santaNum;
}

int getMoveDirOfRudolph(int targetSantaNum, int dist) {
	int minDist = dist;
	int selectDir = -1;
	for (int i = 0; i < 8; ++i) {
		int ny = rudolph.y + ry[i];
		int nx = rudolph.x + rx[i];

		if (ny<1 || ny>N || nx<1 || nx>N) continue;

		int curDist = getDistBetweenBlocks(
			ny, nx,
			santas[targetSantaNum].y, santas[targetSantaNum].x);

		if (minDist > curDist) {
			minDist = min(minDist, curDist);
			selectDir = i;
		}
	}
	return selectDir;
}

void diedSanta(int num) {
	SANTA* now = &santas[num];

	now->isDied = true;
	mmap[now->y][now->x] = 0;

	checkedDeadSanta = checkedDeadSanta | (1 << num);
}

void updateSantaPos(int num, int ny, int nx) {
	SANTA* now = &santas[num];

	mmap[now->y][now->x] = 0;
	mmap[ny][nx] = num;
	now->y = ny;
	now->x = nx;
}

void pushAnotherSanta(int num, int dir) {
	SANTA* now = &santas[num];

	int ny = now->y + ry[dir];
	int nx = now->x + rx[dir];

	if (ny<1 || ny>N || nx<1 || nx>N) {
		diedSanta(num);
		return;
	}
	if (mmap[ny][nx] > 0) { // 다른 산타
		pushAnotherSanta(mmap[ny][nx], dir);
	}

	updateSantaPos(num, ny, nx);
}

void pushedSantaByRudolph(int num, int dir) {
	SANTA* now = &santas[num];

	int ny = now->y + (ry[dir] * C);
	int nx = now->x + (rx[dir] * C);

	if (ny<1 || ny>N || nx<1 || nx>N) {
		diedSanta(num);
		return;
	}
	if (mmap[ny][nx] > 0) { // 다른 산타
		pushAnotherSanta(mmap[ny][nx], dir);
	}

	updateSantaPos(num, ny, nx);
}

void conflictOfRudolph(int santaNum, int dir, int turn) {
	santas[santaNum].score += C;
	santas[santaNum].isStun = turn + 2;
	pushedSantaByRudolph(santaNum, dir);
}

void moveRudolph(int turn) {
	int targetSantaNum = getTargetSantaNum();
	int getDistRudolphToSanta = getDistBetweenBlocks(
		rudolph.y, rudolph.x,
		santas[targetSantaNum].y, santas[targetSantaNum].x);

	// 방향 선택
	int selectDir = getMoveDirOfRudolph(targetSantaNum, getDistRudolphToSanta);

	if (selectDir != -1) { // 가까워질 곳이 있다면
		int ny = rudolph.y + ry[selectDir];
		int nx = rudolph.x + rx[selectDir];

		if (mmap[ny][nx] != 0) conflictOfRudolph(mmap[ny][nx], selectDir, turn);
		// 루돌프 이동
		mmap[ny][nx] = -1;
		mmap[rudolph.y][rudolph.x] = 0;
		rudolph.y = ny;
		rudolph.x = nx;

	}
}

int getMoveDirOfSanta(int num, int dist) {
	SANTA* now = &santas[num];

	int minDist = dist;
	int selectDir = -1;

	for (int i = 0; i < 4; ++i) {
		int ny = now->y + dy[i];
		int nx = now->x + dx[i];

		if (ny<1 || ny>N || nx<1 || nx>N) continue;
		if (mmap[ny][nx] > 0)continue;

		int curDist = getDistBetweenBlocks(
			ny, nx,
			rudolph.y, rudolph.x);

		if (minDist > curDist) {
			minDist = min(minDist, curDist);
			selectDir = i;
		}
	}

	return selectDir;
}

int getReverseDir(int dir) {
	int d = -1;

	if (dir == 0) d = 2;
	else if (dir == 1) d = 3;
	else if (dir == 2) d = 0;
	else if (dir == 3) d = 1;

	return d;
}

void pushAnotherSantaBySanta(int num, int dir) {
	SANTA* now = &santas[num];

	int ny = now->y + dy[dir];
	int nx = now->x + dx[dir];

	if (ny<1 || ny>N || nx<1 || nx>N) {
		diedSanta(num);
		return;
	}
	if (mmap[ny][nx] > 0) { // 다른 산타
		pushAnotherSantaBySanta(mmap[ny][nx], dir);
	}

	updateSantaPos(num, ny, nx);
}

void pushedSantaBySelf(int num, int dir) {
	int d = getReverseDir(dir);
	SANTA* now = &santas[num];

	int ny = rudolph.y + (dy[d] * D);
	int nx = rudolph.x + (dx[d] * D);

	if (ny<1 || ny>N || nx<1 || nx>N) {
		diedSanta(num);
		return;
	}
	if (mmap[ny][nx] > 0 && mmap[ny][nx] != num) { // 다른 산타
		pushAnotherSantaBySanta(mmap[ny][nx], d);
	}

	updateSantaPos(num, ny, nx);
}

void conflictOfSanta(int num, int dir, int turn) {
	santas[num].score += D;
	santas[num].isStun = turn + 2;
	pushedSantaBySelf(num, dir);
}

void moveSanta(int turn) {
	for (map<int, SANTA>::iterator itr = santas.begin(); itr != santas.end(); itr++) {
		if (itr->second.isDied) continue;
		if (itr->second.isStun > turn) {
			continue;
		}

		int getDistSantaToRudolph = getDistBetweenBlocks(
			itr->second.y, itr->second.x,
			rudolph.y, rudolph.x
		);

		// 방향 선택
		int selectDir = getMoveDirOfSanta(itr->first, getDistSantaToRudolph);

		if (selectDir != -1) { // 가까워질 곳이 있다면
			int ny = itr->second.y + dy[selectDir];
			int nx = itr->second.x + dx[selectDir];

			if (mmap[ny][nx] != 0) conflictOfSanta(itr->first, selectDir, turn);
			else { // 산타 이동
				mmap[ny][nx] = itr->first;
				mmap[itr->second.y][itr->second.x] = 0;
				itr->second.y = ny;
				itr->second.x = nx;
			}
		}
	}
}

void updateScore() {
	for (map<int, SANTA>::iterator itr = santas.begin(); itr != santas.end(); itr++) {
		if (itr->second.isDied) continue;
		itr->second.score += 1;
	}
}

void getAns() {
	for (map<int, SANTA>::iterator itr = santas.begin(); itr != santas.end(); itr++) {
		cout << itr->second.score << ' ';
	}
}

void sol() {
	for (int turn = 1; turn <= M; ++turn) {
		moveRudolph(turn);
		if (maskedAllSanta == checkedDeadSanta) break;
		moveSanta(turn);
		if (maskedAllSanta == checkedDeadSanta) break;
		updateScore();
	}

	getAns();
}

SANTA createSanta(int num, int y, int x) {
	SANTA now;
	now.y = y;
	now.x = x;
	now.score = 0;
	now.isStun = 0;
	now.isDied = false;

	return now;
}

void in() {
	cin >> N >> M >> P >> C >> D;
	cin >> rudolph.y >> rudolph.x;
	mmap[rudolph.y][rudolph.x] = -1;

	maskedAllSanta = 0;
	checkedDeadSanta = 0;
	for (int i = 0; i < P; ++i) {
		int num, y, x;
		cin >> num >> y >> x;

		santas[num] = createSanta(num, y, x);
		mmap[y][x] = num;

		maskedAllSanta = maskedAllSanta | (1 << num);
		santaSeq[i] = num;
	}
}

int main() {
	//freopen("input.txt", "r", stdin);
	in();
	sol();
	return 0;
}