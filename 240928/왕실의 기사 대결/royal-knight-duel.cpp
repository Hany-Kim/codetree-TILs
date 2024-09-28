#include <iostream>
#include <cstring>
using namespace std;

#define L_MAX (4 + 1)
#define KNIGHT_MAX (100 + 1)
#define QUERY_MAX (100 + 1)

struct Knight {
	int r, c, h, w;
	int hp;
	bool isLive;
};
struct CMD {
	int i, d;
};
int L, N, Q;
int map[L_MAX][L_MAX]; // 0:빈칸 | 1:함정 | 2:벽
int knightMap[L_MAX][L_MAX];
int tmp[L_MAX][L_MAX];
Knight knights[KNIGHT_MAX];
int pushed[KNIGHT_MAX];
int damageSum[KNIGHT_MAX];
CMD cmd[QUERY_MAX];
int dy[4] = { -1,0,1,0 };
int dx[4] = { 0,1,0,-1 };

void deleteKnightFromMap(int num) {
	Knight* now = &knights[num];

	for (int y = now->r; y < (now->r + now->h); ++y) {
		for (int x = now->c; x < (now->c + now->w); ++x) {
			tmp[y][x] = 0;
		}
	}
}

void createKnightToMap(int num) {
	Knight* now = &knights[num];

	for (int y = now->r; y < (now->r + now->h); ++y) {
		for (int x = now->c; x < (now->c + now->w); ++x) {
			tmp[y][x] = num;
		}
	}
}

void moveKnight(int num, int dir) {
	Knight* now = &knights[num];
	// 다른 기사 있는지 확인
	for (int y = now->r; y < (now->r + now->h); ++y) {
		for (int x = now->c; x < (now->c + now->w); ++x) {
			int ny = y + dy[dir];
			int nx = x + dx[dir];

			if (ny<1 || ny>L || nx<1 || nx>L) continue;
			if (tmp[ny][nx] == tmp[y][x]) continue;

			if (knightMap[ny][nx] != 0) { // 다른 기사가 있다면
				moveKnight(tmp[ny][nx], dir);
			}
		}
	}

	// 밀 수 있는지 확인
	bool canPush = true;
	for (int y = now->r; y < (now->r + now->h); ++y) {
		for (int x = now->c; x < (now->c + now->w); ++x) {
			int ny = y + dy[dir];
			int nx = x + dx[dir];

			if (ny<1 || ny>L || nx<1 || nx>L) {
				canPush = false;
				break;
			}
			if (map[ny][nx] == 2) {
				canPush = false;
				break;
			}
			if (tmp[ny][nx] != 0 && tmp[ny][nx] != tmp[y][x]) {
				canPush = false;
				break;
			}

		}
		if (!canPush) break;
	}

	if (canPush) {
		deleteKnightFromMap(num);
		now->r = now->r + dy[dir];
		now->c = now->c + dx[dir];
		createKnightToMap(num);
		pushed[num] = 1;
	}
}

void disapearKnight(int num) {
	Knight* now = &knights[num];
	for (int y = now->r; y < (now->r + now->h); ++y) {
		for (int x = now->c; x < (now->c + now->w); ++x) {
			knightMap[y][x] = 0;
		}
	}
}

void minusHP(int num) {
	Knight* now = &knights[num];
	int cnt = 0;
	for (int y = now->r; y < (now->r + now->h); ++y) {
		for (int x = now->c; x < (now->c + now->w); ++x) {
			if (map[y][x] == 1) cnt += 1;
		}
	}
	now->hp -= cnt;
	damageSum[num] += cnt;

	if (now->hp <= 0) {
		now->isLive = false;
		disapearKnight(num);
	}
}

void calcDamage(int num) {
	for (int i = 1; i <= N; ++i) {
		if (!knights[i].isLive) continue;
		if (pushed[i] == 0) continue;
		if (i == num) continue;

		minusHP(i);
	}
}

void getAns() {
	int ans = 0;
	for (int i = 1; i <= N; ++i) {
		if (!knights[i].isLive) continue;

		ans += damageSum[i];
	}
	cout << ans;
}

void sol() {
	for (int query = 0; query < Q; ++query) {
		if (!knights[cmd[query].i].isLive) continue; // 이미 사라진 기사면 X
		memcpy(tmp, knightMap, sizeof(tmp));
		moveKnight(cmd[query].i, cmd[query].d);
		memcpy(knightMap, tmp, sizeof(tmp));
		calcDamage(cmd[query].i);
		memset(pushed, 0, sizeof(pushed));
	}

	getAns();
}

void in() {
	cin >> L >> N >> Q;
	for (int y = 1; y <= L; ++y) {
		for (int x = 1; x <= L; ++x) {
			cin >> map[y][x];
		}
	}
	for (int i = 1; i <= N; ++i) {
		Knight* now = &knights[i];
		int r, c, h, w, k;
		cin >> r >> c >> h >> w >> k;
		now->r = r;
		now->c = c;
		now->h = h;
		now->w = w;
		now->hp = k;
		now->isLive = true;

		for (int y = r; y < (r + h); ++y) {
			for (int x = c; x < (c + w); ++x) {
				knightMap[y][x] = i;
			}
		}
	}
	for (int j = 0; j < Q; ++j) {
		CMD* now = &cmd[j];
		int i, d;
		cin >> i >> d;
		now->i = i;
		now->d = d;
	}
	memset(pushed, 0, sizeof(pushed));
	memset(damageSum, 0, sizeof(damageSum));
}

int main() {
	//freopen("input.txt", "r", stdin);
	in();
	sol();
	return 0;
}