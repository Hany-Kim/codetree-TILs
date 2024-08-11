#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

#define N_MAX (99 + 1)

int n, m, h, k;
int map[N_MAX][N_MAX];
int namoo[N_MAX][N_MAX];
struct NODE {
	int y, x, d;
	bool isCatch;
};
vector<NODE> domangja;
int dy[4] = { -1, 0, 1, 0 };
int dx[4] = { 0, -1, 0, 1 };
NODE sulae;
vector<int> sDist;
vector<int> sDir;
vector<int> revSDir;
int score;

void initSulae() {
	sulae.y = (n >> 1) + 1;
	sulae.x = (n >> 1) + 1;
	sulae.d = 0;

	for (int i = 1; i < n; ++i) {
		for (int j = 0; j < 2; ++j) {
			sDist.push_back(i);
		}
		if (i == (n - 1)) {
			sDist.push_back(i);
		}
	}

	int d = 0;
	for (int i = 0; i < sDist.size(); ++i) {
		for (int j = 0; j < sDist[i]; ++j) {
			sDir.push_back(d);
		}
		d -= 1;
		if (d < 0) d = 3;
	}
	for (int i = (sDir.size() - 1); i >= 0; i--) {
		if (sDir[i] == 0) revSDir.push_back(2);
		else if (sDir[i] == 1) revSDir.push_back(3);
		else if (sDir[i] == 2) revSDir.push_back(0);
		else if (sDir[i] == 3) revSDir.push_back(1);
	}
}

int getDistFromSulae(NODE* now) {
	return abs(now->y - sulae.y) + abs(now->x - sulae.x);
}

bool isOutOfMap(int y, int x) {
	if (y<1 || y>n || x<1 || x>n) return true;
	return false;
}

void move_domangja() {
	for (int i = 0; i < domangja.size(); ++i) {
		NODE* now = &domangja[i];
		if (now->isCatch) continue;
		if (getDistFromSulae(now) > 3) continue;
		
		int ny = now->y + dy[now->d];
		int nx = now->x + dx[now->d];
		if (isOutOfMap(ny, nx)) {
			if (now->d == 0) now->d = 2;
			else if (now->d == 1) now->d = 3;
			else if (now->d == 2) now->d = 0;
			else if (now->d == 3) now->d = 1;

			ny = now->y + dy[now->d];
			nx = now->x + dx[now->d];
		}

		if (ny == sulae.y && nx == sulae.x) continue;
		
		map[now->y][now->x] -= 1;
		map[ny][nx] += 1;
		now->y = ny;
		now->x = nx;
	}
}

int move_sulae(int turn) {
	int mod = pow(n, 2) - 1;

	NODE* s = &sulae;
	
	int idx = (turn - 1) % (mod);
	int see = -1;
	int nd = 0;

	if (s->d == 0) {
		nd = sDir[idx];
		if (idx != sDir.size() - 1) { // 마지막 원소가 아닐때
			if (sDir[idx] != sDir[idx + 1]) { // 틀어지는 곳
				see = sDir[idx + 1];
			}
			else see = nd;
		}
		else see = revSDir[0]; // 마지막 원소는 그냥 틀어짐
	}
	else if (s->d == 1) {
		nd = revSDir[idx];
		if (idx != revSDir.size() - 1) { // 마지막 원소가 아닐때
			if (revSDir[idx] != revSDir[idx + 1]) { // 틀어지는 곳
				see = revSDir[idx + 1];
			}
			else see = nd;
		}
		else see = sDir[0]; // 마지막 원소는 그냥 틀어짐
	}

	// 술래 이동
	s->y = s->y + dy[nd];
	s->x = s->x + dx[nd];
	
	if (s->y == 1 && s->x == 1) s->d = 1;
	else if (s->y == ((n >> 1) + 1) && s->x == ((n >> 1) + 1)) s->d = 0;

	return see;
}

void catch_domangja(int turn, int dir) {
	int cnt = 0;
	NODE* s = &sulae;

	int ny = s->y;
	int nx = s->x;

	for (int j = 0; j < 3; ++j) {
		if (j != 0) {
			ny = ny + dy[dir];
			nx = nx + dx[dir];
		}

		if (namoo[ny][nx] != 1 && (map[ny][nx] > 0)) {
			cnt += map[ny][nx];
			map[ny][nx] = 0;

			for (int i = 0; i < domangja.size(); ++i) {
				if (domangja[i].y == ny && domangja[i].x == nx) {
					domangja[i].isCatch = true;
				}
			}
		}
	}

	score += (cnt * turn);
}

void sol() {
	initSulae();
	//int cnt = 0;
	for (int turn = 1; turn <= k; ++turn) {
		move_domangja();
		//map[sulae.y][sulae.x] = cnt++;
		int dir = move_sulae(turn);
		catch_domangja(turn, dir);
	}
	cout << score;
}

void input() {
	cin >> n >> m >> h >> k;
	for (int i = 0; i < m; ++i) {
		NODE t;
		cin >> t.y >> t.x >> t.d;
		if (t.d % 2 == 0) { // 상하
			t.d = 2;
		}
		else if (t.d % 2 == 1) { // 좌우
			t.d = 3;
		}
		t.isCatch = false;
		domangja.push_back(t);
		map[t.y][t.x] += 1;
	}
	for (int i = 0; i < h; ++i) {
		int y = 0, x = 0;
		cin >> y >> x;
		namoo[y][x] = 1;
	}
}

int main() {
	// 여기에 코드를 작성해주세요.
	//freopen("input.txt", "r", stdin);

	input();
	sol();

	return 0;
}