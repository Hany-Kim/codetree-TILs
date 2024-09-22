#include <iostream>
#include <queue>
#include <algorithm>
#include <cstring>
using namespace std;

#define MAP_MAX (71 + 1 + 2)
#define K_MAX 1001
#define PII pair<int, int>

struct NODE {
	PII state;
	PII pos;
};
int R, C, K;
NODE yoJung[K_MAX];
int dy[4] = { -1, 0, 1, 0 };
int dx[4] = { 0, 1, 0, -1 };
int map[MAP_MAX][MAP_MAX];
int ans;

bool canMoveSouth(PII pos) {
	if (pos.first + 1 >= (R + 2)) return false;
	if (map[pos.first + 2][pos.second] != 0
		|| map[pos.first + 1][pos.second - 1] != 0
		|| map[pos.first + 1][pos.second + 1] != 0) return false;
	return true;
}

bool canMoveWest(PII pos) {
	if (pos.second - 1 <= 1) return false;
	if (map[pos.first][pos.second - 2] != 0
		|| map[pos.first - 1][pos.second - 1] != 0
		|| map[pos.first + 1][pos.second - 1] != 0) return false;
	else {
		pos.second -= 1;
		return canMoveSouth(pos);
	}
}

bool canMoveEast(PII pos) {
	if (pos.second + 1 >= C) return false;
	if (map[pos.first][pos.second + 2] != 0
		|| map[pos.first - 1][pos.second + 1] != 0
		|| map[pos.first + 1][pos.second + 1] != 0) return false;
	else {
		pos.second += 1;
		return canMoveSouth(pos);
	}
}

void putDownToEnd(PII* pos, int* dir) {
	while (1) {
		if (canMoveSouth(*pos)) {
			pos->first += 1;
		}
		else if (canMoveWest(*pos)) {
			pos->first += 1;
			pos->second -= 1;

			*dir -= 1;
			if (*dir < 0) *dir = 3;
		}
		else if (canMoveEast(*pos)) {
			pos->first += 1;
			pos->second += 1;

			*dir += 1;
			if (*dir > 3) *dir = 0;
		}
		else break;
	}

	map[pos->first][pos->second] = 1;
	if(*dir == 2) map[pos->first + 1][pos->second] = 2;
	else map[pos->first + 1][pos->second] = 1;

	if (*dir == 1) map[pos->first][pos->second + 1] = 2;
	else map[pos->first][pos->second + 1] = 1;

	if (*dir == 0) map[pos->first - 1][pos->second] = 2;
	else map[pos->first - 1][pos->second] = 1;

	if(*dir == 3) map[pos->first][pos->second - 1] = 2;
	else map[pos->first][pos->second - 1] = 1;

}

int getOutToGolem(PII pos) {
	queue<PII> q;
	int used[MAP_MAX][MAP_MAX] = { 0, };

	q.push(pos);
	used[pos.first][pos.second] = 1;
	int maxRow = max(0, pos.first - 2);

	while (!q.empty()) {
		PII now = q.front();
		q.pop();

		for (int i = 0; i < 4; ++i) {
			if (used[now.first][now.second] == 2 && map[now.first][now.second] != 2) continue;

			int ny = now.first + dy[i];
			int nx = now.second + dx[i];

			if (ny < 2 || ny >= (R + 1 + 2) || nx < 1 || nx >= (C + 1)) continue;
			if (map[ny][nx] == 0) continue;
			if (used[ny][nx] >= 1) continue;

			used[ny][nx] = used[now.first][now.second] + 1;
			maxRow = max(ny - 2, maxRow);

			q.push(make_pair(ny, nx));
		}
	}

	return maxRow;
}

void sol() {
	for (int num = 1; num <= K; ++num) {
		PII now = yoJung[num].state;
		PII pos; // 낙하 시작 위치
		pos.first = 1;
		pos.second = now.first;

		putDownToEnd(&pos, &now.second); // 골렘 낙하

		int score = getOutToGolem(pos);
		if(pos.first - 1 >= 3) ans += score;
		else {
			memset(map, 0, sizeof(map));
		}
		
		int de = 1;
	}
	cout << ans;
}

void input() {
	cin >> R >> C >> K;

	for (int i = 1; i <= K; ++i) {
		int c, d;
		cin >> c >> d;
		yoJung[i].state = make_pair(c, d);
	}
}

int main() {
	//freopen("input.txt", "r", stdin);

	input();
	sol();

	return 0;
}