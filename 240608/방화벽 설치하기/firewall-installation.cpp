#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <queue>
using namespace std;

#define N_MAX (8)
#define M_MAX (8)
#define FOR(i,s,e) for(int i=(s); i<(e); ++i)
#define PII pair<int,int>

int n, m;
int map[N_MAX][M_MAX];
int tmap[N_MAX][M_MAX];
int used[N_MAX][M_MAX];

PII fireList[10];
int fireCnt;
PII path[3];

int dy[4] = { -1, 1, 0, 0 };
int dx[4] = { 0, 0, -1, 1 };
int maxSum;

void input() {
	cin >> n >> m;
	FOR(y, 0, n) {
		FOR(x, 0, m) {
			cin >> map[y][x];
			if (map[y][x] == 2) {
				fireList[fireCnt++] = make_pair(y, x);
			}
		}
	}
}

void bfs(int num) {
	queue<PII> q;
	int visit[N_MAX][M_MAX] = { 0, };

	PII nowFire = fireList[num];

	q.push(nowFire);
	visit[nowFire.first][nowFire.second] = 1;

	while (!q.empty()) {
		PII now = q.front();
		q.pop();

		FOR(i, 0, 4) {
			int ny = now.first + dy[i];
			int nx = now.second + dx[i];

			if (ny < 0 || ny >= n || nx < 0 || nx >= m) continue;
			if (visit[ny][nx] == 1) continue; // 방문했으면
			if (tmap[ny][nx] == 1) continue; // 방화벽이면
			
			tmap[ny][nx] = 2;
			visit[ny][nx] = 1;
			q.push(make_pair(ny, nx));
		}
	}
}

/*
void dfs(int lv, PII now) {
	if (lv >= 3) {
		memcpy(tmap, map, sizeof(map));
		FOR(i, 0, 3) {
			PII now = path[i];

			tmap[now.first][now.second] = 1;
		}

		FOR(i, 0, fireCnt) {
			bfs(i);
		}

		int sum = 0;
		FOR(y, 0, n) {
			FOR(x, 0, m) {
				if (tmap[y][x] == 0) 
					++sum;
			}
		}
		maxSum = max(maxSum, sum);

		return;
	}

	FOR(y, now.first, n) {
		FOR(x, 0, m) {
			if (map[y][x] != 0) continue;
			if (path[lv - 1].first >= y && path[lv - 1].second > x) continue;
			if (used[y][x] == 1) continue;
			used[y][x] = 1;
			path[lv] = make_pair(y, x);
			dfs(lv + 1, make_pair(y, x));
			path[lv] = make_pair(0, 0);
			used[y][x] = 0;
		}
	}
}
*/

void sol() {
	/*FOR(y, 0, n) {
		FOR(x, 0, m) {
			if (map[y][x] == 0) {
				memset(used, 0, sizeof(used));
				used[y][x] = 1;
				PII now = make_pair(y, x);
				path[0] = now;
				dfs(1, now);
				path[0] = make_pair(0, 0);
				used[y][x] = 0;
			}
		}
	}*/

	FOR(y1, 0, n) {
		FOR(x1, 0, m) {
			if (map[y1][x1] != 0) continue;
			path[0] = make_pair(y1, x1);
			map[y1][x1] = 1;

			FOR(y2, y1, n) {
				FOR(x2, 0, m) {
					if (map[y2][x2] != 0) continue;
					path[1] = make_pair(y2, x2);
					map[y2][x2] = 1;

					FOR(y3, y2, n) {
						FOR(x3, 0, m) {
							if (map[y3][x3] != 0) continue;
							path[2] = make_pair(y3, x3);
							map[y3][x3] = 1;

							memcpy(tmap, map, sizeof(map));

							FOR(i, 0, fireCnt) {
								bfs(i);
							}

							int sum = 0;
							FOR(y, 0, n) {
								FOR(x, 0, m) {
									if (tmap[y][x] == 0)
										++sum;
								}
							}
							maxSum = max(maxSum, sum);

							map[y3][x3] = 0;
						}
					}
					map[y2][x2] = 0;
				}
			}
			map[y1][x1] = 0;
		}
	}

	cout << maxSum;
}

int main() {
	//freopen("input.txt", "r", stdin);
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);
	input();
	sol();
	return 0;
}