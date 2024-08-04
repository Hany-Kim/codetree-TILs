#include <iostream>
#include <vector>
#include <queue>
using namespace std;

#define N_MAX 25
#define PII pair<int, int>

int map[N_MAX][N_MAX];
int wall[2][N_MAX][N_MAX];
int n, m, k;
int dy[6] = { 0,0,0,-1,0,1 };
int dx[6] = { 0,0,-1,0,1,0 };
int ddy[4] = { -1, 0, 1, 0 };
int ddx[4] = { 0, -1, 0, 1 };
int windMap[N_MAX][N_MAX];
struct AIR {
	PII pos;
	int dir;
};
vector<AIR> acList;
vector<PII> officeList;

bool checkWall(int sy, int sx, int num) {
	if (acList[num].dir == 2) {
		if (wall[1][acList[num].pos.first][acList[num].pos.second]) return true;
	}
	else if (acList[num].dir == 3) {
		if (wall[0][acList[num].pos.first][acList[num].pos.second]) return true;
	}
	else if (acList[num].dir == 4) {
		if (wall[1][sy][sx]) return true;
	}
	else if (acList[num].dir == 5) {
		if (wall[0][sy][sx]) return true;
	}
	return false;
}

int getNd(int d, int i) {
	int nd = d + i;
	if (nd < 2) nd = 5;
	if (nd > 5) nd = 2;
	
	return nd;
}

bool canGoStep1(pair<PII, int> now, int num, int i, int ny, int nx) {
	if (acList[num].dir == 2) {
		if (i == 1) {
			if (wall[0][now.first.first][now.first.second]) return true; // 위가 막힘
		}
		if (i == -1) {
			if (wall[0][ny][nx]) return true; // 아래가 막힘
		}
	}
	else if (acList[num].dir == 4) {
		if (i == -1) {
			if (wall[0][now.first.first][now.first.second]) return true; // 위가 막힘
		}
		if (i == 1) {
			if (wall[0][ny][nx]) return true; // 아래가 막힘
		}
	}
	else if (acList[num].dir == 3) {
		if (i == -1) {
			if (wall[1][now.first.first][now.first.second]) return true; // 왼쪽이 막힘
		}
		if (i == 1) {
			if (wall[1][ny][nx]) return true; // 오른쪽이 막힘
		}
	}
	else if (acList[num].dir == 5) {
		if (i == 1) {
			if (wall[1][now.first.first][now.first.second]) return true; // 왼쪽이 막힘
		}
		if (i == -1) {
			if (wall[1][ny][nx]) return true; // 오른쪽이 막힘
		}
	}
	return false;
}

bool canGoStep2(pair<PII, int> now, int num, int i, int ny, int nx) {
	if (acList[num].dir == 2) {
		if (wall[1][ny - dy[acList[num].dir]][nx - dx[acList[num].dir]]) return true;
	}
	else if (acList[num].dir == 4) {
		if (wall[1][ny][nx]) return true;
	}
	else if (acList[num].dir == 3) {
		if (wall[0][ny - dy[acList[num].dir]][nx - dx[acList[num].dir]]) return true;
	}
	else if (acList[num].dir == 5) {
		if (wall[0][ny][nx]) return true;
	}
	return false;
}

void spreadWind(int num) {
	queue<pair<PII, int>> q; // 위치, 온도
	int used[N_MAX][N_MAX] = { 0, };
	int tmp[N_MAX][N_MAX] = { 0, };
	
	int sy = acList[num].pos.first + dy[acList[num].dir];
	int sx = acList[num].pos.second + dx[acList[num].dir];

	// 에어컨 문 앞에 바로 벽 있는지
	if (checkWall(sy, sx, num)) return;

	used[acList[num].pos.first][acList[num].pos.second] = 1;
	used[sy][sx] = 1;
	tmp[sy][sx] = 5;
	q.push(make_pair(make_pair(sy, sx), tmp[sy][sx]));

	while (!q.empty()) {
		pair<PII, int> now = q.front();
		q.pop();

		for (int i = -1; i < 2; ++i) {
			int nd = getNd(acList[num].dir, i);
			int ny = now.first.first;
			int nx = now.first.second;

			if (i != 0) {
				ny = ny + dy[nd];
				nx = nx + dx[nd];
			}
			if (canGoStep1(now, num, i, ny, nx)) continue;

			ny = ny + dy[acList[num].dir];
			nx = nx + dx[acList[num].dir];
			if (canGoStep2(now, num, i, ny, nx)) continue;

			if (ny<1 || ny>n || nx<1 || nx>n) continue;
			if (used[ny][nx] == 1)continue;
			if (tmp[ny][nx] > 0) continue;

			tmp[ny][nx] = now.second - 1;
			used[ny][nx] = 1;
			q.push(make_pair(make_pair(ny, nx), tmp[ny][nx]));
		}
	}

	for (int y = 1; y <= n; ++y) {
		for (int x = 1; x <= n; ++x) {
			windMap[y][x] += tmp[y][x];
		}
	}
}

void turnOnAc() {
	for (int i = 0; i < acList.size(); ++i) {
		spreadWind(i);
	}
}

void mixedAir() {
	int tmp[N_MAX][N_MAX] = { 0, };

	for (int y = 1; y <= n; ++y) {
		for (int x = 1; x <= n; ++x) {
			for (int i = 0; i < 4; ++i) {
				int ny = y + ddy[i];
				int nx = x + ddx[i];

				if (ny<1 || ny>n || nx<1 || nx>n) continue;

				// 상좌하우
				if (i == 0) {
					if (wall[0][y][x]) continue;
				}
				else if (i == 1) {
					if (wall[1][y][x]) continue;
				}
				else if (i == 2) {
					if (wall[0][ny][nx]) continue;
				}
				else if (i == 3) {
					if (wall[1][ny][nx]) continue;
				}

				if (windMap[y][x] <= windMap[ny][nx]) continue;

				int flow = (windMap[y][x] - windMap[ny][nx]) >> 2;
				tmp[y][x] -= flow;
				tmp[ny][nx] += flow;
			}
		}
	}

	for (int y = 1; y <= n; ++y) {
		for (int x = 1; x <= n; ++x) {
			windMap[y][x] += tmp[y][x];
		}
	}
}

void decreaseOutside() {
	for (int y = 1; y <= n; ++y) {
		windMap[y][1] -= 1;
		if (windMap[y][1] < 0) windMap[y][1] = 0;
		windMap[y][n] -= 1;
		if (windMap[y][n] < 0) windMap[y][n] = 0;
	}
	for (int x = 2; x < n; ++x) {
		windMap[1][x] -= 1;
		if (windMap[1][x] < 0) windMap[1][x] = 0;
		windMap[n][x] -= 1;
		if (windMap[n][x] < 0) windMap[n][x] = 0;
	}
}

bool checkOffice() {
	for (int i = 0; i < officeList.size(); ++i) {
		if (windMap[officeList[i].first][officeList[i].second] < k) {
			return false;
		}
	}
	return true;
}

void sol() {
	int curTime = 0;
	while (1) {
		turnOnAc();
		mixedAir();
		decreaseOutside();
		curTime++;
		if (curTime >= 100) {
			curTime = -1;
			break;
		}
		if (checkOffice()) break;
	}
	cout << curTime;
}

void input() {
	cin >> n >> m >> k;
	for (int y = 1; y <= n; ++y) {
		for (int x = 1; x <= n; ++x) {
			cin >> map[y][x];
			if (map[y][x] >= 2) {
				acList.push_back({ make_pair(y,x), map[y][x] });
			}
			else if (map[y][x] == 1) {
				officeList.push_back(make_pair(y, x));
			}
		}
	}
	for (int i = 0; i < m; ++i) {
		int r = 0, c = 0, d = 0;
		cin >> r >> c >> d;
		wall[d][r][c] = 1;
	}
}

int main() {
	//freopen("input.txt", "r", stdin);
	input();
	sol();
	return 0;
}