#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
using namespace std;

#define N_MAX (100 + 1)
#define M_MAX (100 + 1)
#define K_MAX (100 * 100 + 1)

int n, m, k; // n x m 격자, 곰팡이 수 k
int map[N_MAX][M_MAX];
int tmap[N_MAX][M_MAX];
struct INFO {
	int x, y, s, d, b;
	bool isCatch;
};
INFO gompangi[K_MAX];
int dy[5] = { 0, -1, 1, 0, 0 };
int dx[5] = { 0, 0, 0, 1, -1 };
int gameTurn;
int score;

void input() {
	cin >> n >> m >> k;
	for (int i = 1; i <= k; ++i) {
		cin >> gompangi[i].y >> gompangi[i].x >> gompangi[i].s >> gompangi[i].d >> gompangi[i].b;
		map[gompangi[i].y][gompangi[i].x] = i;
	}
}

void catchGompangi(int y, int x) {
	int g_num = map[y][x];
	map[y][x] = 0;
	gompangi[g_num].isCatch = true; // 잡혔다.
	score += gompangi[g_num].b;
}

void findGompangi(int x) {
	for (int y = 1; y <= n; ++y) {
		if (map[y][x] != 0) {
			// 곰팡이 만났다면
			catchGompangi(y, x);
			return; // 채취했으면 다음 열
		}
	}
}

void dieGompangi(int feed, int posik, int y, int x) {
	tmap[y][x] = posik;
	gompangi[feed].isCatch = true; // 잡혔다.
}

void moveGompangi() {
	memset(tmap, 0, sizeof(tmap));
	for (int i = 1; i <= k; ++i) {
		if (gompangi[i].isCatch) continue; // 잡힌 곰팡이는 건너뜀
		INFO* now = &gompangi[i];

		int ny = now->y;
		int nx = now->x;
		for (int moveCnt = 1; moveCnt <= now->s; ++moveCnt) {
			ny = ny + dy[now->d];
			nx = nx + dx[now->d];

			if (ny<1 || nx<1 || ny>n || nx>m) {
				// 벽 만나면 뒤돌기
				if (now->d == 1) now->d = 2;
				else if (now->d == 2) now->d = 1;
				else if (now->d == 3) now->d = 4;
				else if (now->d == 4) now->d = 3;

				ny = ny + (dy[now->d] * 2);
				nx = nx + (dx[now->d] * 2);
			}
		}
		
		if (tmap[ny][nx] != 0) {
			// 다른 곰팡이가 있음
			if (gompangi[map[now->y][now->x]].b > gompangi[tmap[ny][nx]].b) {
				dieGompangi(tmap[ny][nx], map[now->y][now->x], ny, nx);
			}
			if (gompangi[map[now->y][now->x]].b < gompangi[tmap[ny][nx]].b) {
				dieGompangi(map[now->y][now->x], tmap[ny][nx], ny, nx);
			}
		}
		else {
			tmap[ny][nx] = i;
		}

		now->y = ny;
		now->x = nx;
	}
	memcpy(map, tmap, sizeof(map));
}

void sol() {
	for (int x = 1; x <= m; ++x) {
		findGompangi(x);
		moveGompangi();
		gameTurn++;
	}
	cout << score;
}

int main() {
	//freopen("input.txt", "r", stdin);
	input();
	sol();
	return 0;
}