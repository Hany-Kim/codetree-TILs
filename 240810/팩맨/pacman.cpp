#include <iostream>
#include <vector>
#include <cstring>
using namespace std;

int m, t;
struct NODE {
	int y, x, d, liveTurn;
};
NODE packman;
int dy[9] = { 0, -1, -1, 0, 1, 1, 1, 0, -1 };
int dx[9] = { 0, 0, -1, -1, -1, 0, 1, 1, 1 };
int map[5][5];
int eggMap[5][5];
int sicheMap[5][5];
vector<NODE> monsterList;
vector<NODE> eggList;
vector<NODE> sicheList;
int path[3];
int selectPath[3];
int maxCnt;

void copy_mon() {
	eggList = monsterList;
	for (int i = 0; i < monsterList.size(); ++i) {
		NODE now = monsterList[i];
		eggMap[now.y][now.x] += 1;
	}
}

void move_mon() {
	int tmp[5][5] = { 0, };
	//memcpy(tmp, map, sizeof(tmp));

	for (int j = 0; j < monsterList.size(); ++j) {
		NODE now = monsterList[j];
		
		int ny = now.y, nx = now.x;
		int nd = 0;
		for (int i = 0; i < 8; ++i) {
			nd = i + now.d;
			nd = (nd / 9) + (nd % 9);
			ny = now.y + dy[nd];
			nx = now.x + dx[nd];

			if (packman.y == ny && packman.x == nx) continue;
			if (sicheMap[ny][nx] >= 1) continue;
			if (ny < 1 || ny>4 || nx < 1 || nx>4) continue;

			break;
		}
		tmp[ny][nx] += 1;
		now.y = ny;
		now.x = nx;
		now.d = nd;
		monsterList[j] = now;
	}

	memcpy(map, tmp, sizeof(map));
}

void dfs(int lv, NODE pm) {
	if (lv == 3) {
		int used[5][5] = { 0, };

		int cnt = 0;
		if (map[packman.y][packman.x] > 0) {
			cnt += map[packman.y][packman.x];
			used[packman.y][packman.x] = 1;
		}

		int ny = packman.y;
		int nx = packman.x;
		for (int i = 0; i < 3; ++i) {
			ny = ny + dy[path[i]];
			nx = nx + dx[path[i]];

			if (map[ny][nx] > 0 && used[ny][nx] == 0) {
				cnt += map[ny][nx];
				used[ny][nx] = 1;
			}
		}

		if (maxCnt < cnt) {
			maxCnt = cnt;
			memcpy(selectPath, path, sizeof(path));
		}
		return;
	}

	for (int i = 1; i < 8; i += 2) {
		int ny = pm.y + dy[i];
		int nx = pm.x + dx[i];

		if (ny < 1 || ny>4 || nx < 1 || nx>4) continue;

		NODE npm;
		npm.y = ny;
		npm.x = nx;
		path[lv] = i;

		dfs(lv + 1, npm);
		
		path[lv] = 0;
	}
}

void createSiche() {
	memset(sicheMap, 0, sizeof(sicheMap));
	vector<NODE> ttmp;
	for (int j = 0; j < sicheList.size(); ++j) {
		NODE now = sicheList[j];
		if (now.liveTurn - 1 > 0) {
			now.liveTurn -= 1;
			ttmp.push_back(now);
			sicheMap[now.y][now.x] += 1;
		}
	}
	sicheList = ttmp;

	int ny = packman.y;
	int nx = packman.x;
	for (int i = 0; i < 3; ++i) {
		ny = ny + dy[selectPath[i]];
		nx = nx + dx[selectPath[i]];

		if (map[ny][nx] > 0) {
			map[ny][nx] = 0;
			for (int j = 0; j < monsterList.size(); ++j) {
				NODE now = monsterList[j];

				if (now.y == ny && now.x == nx) {
					now.liveTurn = 2;
					monsterList[j].liveTurn = 2;
					sicheList.push_back(now);
					sicheMap[now.y][now.x] += 1;
				}
			}
		}
	}
	packman.y = ny;
	packman.x = nx;

	vector<NODE> tmp;
	for (int j = 0; j < monsterList.size(); ++j) {
		NODE now = monsterList[j];

		if (now.liveTurn == 0) {
			tmp.push_back(now);
		}
	}
	monsterList = tmp;

}

void move_packman() {
	maxCnt = -1;

	memset(selectPath, 0, sizeof(selectPath));
	memset(path, 0, sizeof(path));
	dfs(0, packman);
	createSiche();
}

void buhwa_egg() {
	for (int i = 0; i < eggList.size(); ++i) {
		NODE now = eggList[i];

		map[now.y][now.x] += 1;
		monsterList.push_back(now);
	}
	eggList.clear();
	memset(eggMap, 0, sizeof(eggMap));
}

void sol() {
	for (int turn = 1; turn <= t; ++turn) {
		copy_mon();
		move_mon();
		move_packman();
		buhwa_egg();
	}
	cout << monsterList.size();
}

void input() {
	cin >> m >> t;
	cin >> packman.y >> packman.x;
	for (int i = 0; i < m; ++i) {
		NODE mon;
		cin >> mon.y >> mon.x >> mon.d;
		map[mon.y][mon.x] += 1;
		mon.liveTurn = 0;
		monsterList.push_back(mon);
	}
}

int main() {
	//freopen("input.txt", "r", stdin);
	input();
	sol();
	return 0;
}