#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
using namespace std;

#define R_MAX 20 + 1
#define C_MAX 20 + 1
#define FOR(i,s,e) for(int i=s; i<e; ++i)

int R, C, K;
int map[R_MAX][C_MAX];
struct NODE {
	int y;
	int x;
	int d;
};
NODE heater[410];
NODE target[410];
int dy[5] = { 0,0,0,-1,1 }; // 동 서 북 남
int dx[5] = { 0,1,-1,0,0 };
int W;
int wall[R_MAX * 2][C_MAX * 2];
int temper[R_MAX][C_MAX];
int h_cnt;
int t_cnt;
int chocolate;

void add_heaterMap_to_temper(int heater_map[][C_MAX]) {
	FOR(y, 1, R + 1) {
		FOR(x, 1, C + 1) {
			temper[y][x] += heater_map[y][x];
		}
	}
}

void blow_from_heater(int num) {
	/*
	온풍기에서 바람이 분다.
	*/
	queue<pair<int, int>> q;
	int heater_map[R_MAX][C_MAX] = { 0, };
	NODE* now_heater = &heater[num];

	int sy = now_heater->y + dy[now_heater->d];
	int sx = now_heater->x + dx[now_heater->d];

	heater_map[sy][sx] = 5;
	q.push(make_pair(sy,sx));

	if (now_heater->d == 1 || now_heater->d == 2) {
		while (!q.empty()) {
			pair<int,int> now = q.front();
			q.pop();

			FOR(i, 1, 5) {
				if (now_heater->d == 1 && i == 2) continue;
				else if (now_heater->d == 2 && i == 1) continue;

				if (wall[(now.first * 2) - 1 + dy[i]][(now.second * 2) - 1 + dx[i]] == 1) continue; // 벽에 막혔다면
				int ny = now.first + dy[i];
				int nx = now.second + dx[i];
				if (i != now_heater->d) {
					if (wall[(ny * 2) - 1 + dy[now_heater->d]][(nx * 2) - 1 + dx[now_heater->d]] == 1) continue; // 벽에 막혔다면
					ny = ny + dy[now_heater->d];
					nx = nx + dx[now_heater->d];
				}

				if (ny < 1 || ny > R || nx < 1 || nx > C) continue;
				if (heater_map[ny][nx] != 0) continue;

				if ((heater_map[now.first][now.second] - 1) < 1) continue;
				heater_map[ny][nx] = heater_map[now.first][now.second] - 1;
				q.push(make_pair(ny, nx));
			}
		}
	}
	else if (now_heater->d == 3 || now_heater->d == 4) {
		while (!q.empty()) {
			pair<int, int> now = q.front();
			q.pop();

			FOR(i, 1, 5) {
				if (now_heater->d == 3 && i == 4) continue;
				else if (now_heater->d == 4 && i == 3) continue;

				if (wall[(now.first * 2) - 1 + dy[i]][(now.second * 2) - 1 + dx[i]] == 1) continue;
				int ny = now.first + dy[i];
				int nx = now.second + dx[i];
				if (i != now_heater->d) {
					if (wall[(ny * 2) - 1 + dy[now_heater->d]][(nx * 2) - 1 + dx[now_heater->d]] == 1) continue;
					ny = ny + dy[now_heater->d];
					nx = nx + dx[now_heater->d];
				}

				if (ny < 1 || ny > R || nx < 1 || nx > C) continue;
				if (heater_map[ny][nx] != 0) continue;

				if ((heater_map[now.first][now.second] - 1) < 1) continue;
				heater_map[ny][nx] = heater_map[now.first][now.second] - 1;
				q.push(make_pair(ny, nx));
			}
		}
	}

	add_heaterMap_to_temper(heater_map);
	int de = 1;
}

void control_temper() {
	/*
	온도 조절
	*/
	int spread_temper[R_MAX][C_MAX] = { 0, };
	int flow_out_temper[R_MAX][C_MAX] = { 0, };

	FOR(y, 1, R + 1) {
		FOR(x, 1, C + 1) {
			spread_temper[y][x] = 0;
			flow_out_temper[y][x] = 0;
		}
	}

	FOR(y, 1, (R + 1)) {
		FOR(x, 1, (C + 1)) {
			if (temper[y][x] != 0) {
				FOR(i, 1, 5) {
					if (wall[(2 * y) - 1 + dy[i]][(2 * x) - 1 + dx[i]] == 1) continue;
					int ny = y + dy[i];
					int nx = x + dx[i];

					if (ny<1 || ny > R || nx < 1 || nx > C) continue;

					if (temper[y][x] > temper[ny][nx]) {
						int L = (temper[y][x] - temper[ny][nx]) >> 2; // (두칸의 온도차 / 4)

						spread_temper[ny][nx] += L;
						flow_out_temper[y][x] -= L;
					}
				}
				flow_out_temper[y][x] += temper[y][x];
			}
		}
	}

	FOR(y, 1, (R + 1)) {
		FOR(x, 1, (C + 1)) {
			temper[y][x] = flow_out_temper[y][x] + spread_temper[y][x];
		}
	}
}

void decrease_outside_temper() {
	/*
	가장 바깥쪽 칸의 온도 1씩 감소
	*/
	FOR(x, 1, C) {
		if (temper[1][x] < 1) continue;
		temper[1][x] -= 1;
	}
	FOR(x, 2, (C + 1)) {
		if (temper[R][x] < 1) continue;
		temper[R][x] -= 1;
	}
	FOR(y, 2, (R + 1)) {
		if (temper[y][1] < 1) continue;
		temper[y][1] -= 1;
	}
	FOR(y, 1, R) {
		if (temper[y][C] < 1) continue;
		temper[y][C] -= 1;
	}
}

void eat_chocolate() {
	/* 초콜릿 먹기 */
	++chocolate;
}

bool isReachTargetTemper() {
	/* 목표 온도에 도달 했는지 */
	FOR(i, 0, t_cnt) {
		if (temper[target[i].y][target[i].x] < K) {
			return false;
		}
	}
	return true;
}

void sol() {
	while (isReachTargetTemper() == false) {
		FOR(i, 0, h_cnt) {
			blow_from_heater(i);
		}
		control_temper();
		decrease_outside_temper();
		eat_chocolate();
		if (chocolate > 100) {
			chocolate = 101;
			break;
		}
	}
	cout << chocolate;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	//cin >> R >> C >> K;
	cin >> R >> W >> K;
	C = R;
	FOR(y, 1, (R + 1)) {
		FOR(x, 1, (C + 1)) {
			cin >> map[y][x];
			if (map[y][x] == 1) {
			// if (map[y][x] == 5) {
				map[y][x] = 5;
				target[t_cnt].y = y;
				target[t_cnt++].x = x;
			}
			else if (map[y][x] != 0) {
				if (map[y][x] == 2) map[y][x] = 2;
				if (map[y][x] == 3) map[y][x] = 3;
				if (map[y][x] == 4) map[y][x] = 1;
				if (map[y][x] == 5) map[y][x] = 4;
				heater[h_cnt].y = y;
				heater[h_cnt].x = x;
				heater[h_cnt++].d = map[y][x];
			}
		}
	}
	//cin >> W;
	FOR(i, 0, W) {
		int r = 0, c = 0, d = 0;
		cin >> r >> c >> d;
		int y = (r * 2) - 1;
		int x = (c * 2) - 1;
		if (d == 0) {
			wall[y - 1][x] = 1;
		}
		else if (d == 1) {
			wall[y][x + 1] = 1;
		}
	}

	sol();

	return 0;
}