#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

#define N_MAX (10 + 2)
#define M_MAX (10 + 2)

struct NODE {
	int y, x;
	int lat; //last attack time
	int att; // 공격력
};
int N, M, K;
int map[N_MAX][M_MAX];
int attackedTower[N_MAX][M_MAX];
int lastAttackTime[N_MAX][M_MAX];
NODE attacker;
NODE targeter;
int dy[4] = { 0, 1, 0, -1 };
int dx[4] = { 1, 0, -1, 0 };
int path[100];
bool findPath;
int by[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
int bx[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };

bool attackerTargeterSort(NODE left, NODE right) {
	if (left.att < right.att) return true;
	if (left.att > right.att) return false;

	if (left.lat > right.lat) return true;
	if (left.lat < right.lat) return false;

	if ((left.y + left.x) > (right.y + right.x)) return true;
	if ((left.y + left.x) < (right.y + right.x)) return false;

	if (left.x > right.x) return true;
	if (left.x < right.x) return false;

	return false;
}

void selectAttackerAndTargeter() {
	vector<NODE> v;
	for (int y = 1; y <= N; ++y) {
		for (int x = 1; x <= M; ++x) {
			if (map[y][x] > 0) {
				NODE now;
				now.y = y;
				now.x = x;
				now.lat = lastAttackTime[y][x];
				now.att = map[y][x];
				v.push_back(now);
			}
		}
	}

	sort(v.begin(), v.end(), attackerTargeterSort);

	attacker = v[0];
	targeter = v[v.size() - 1];
}

void plusAttackAbillity() {
	attacker.att += (N + M);
	map[attacker.y][attacker.x] += (N + M);
}

int bfs() {
	queue<NODE> q;
	int visited[N_MAX][M_MAX] = { 0, };

	visited[attacker.y][attacker.x] = 1;
	q.push(attacker);

	while (!q.empty()) {
		NODE now = q.front(); q.pop();

		for (int i = 0; i < 4; ++i) {
			int ny = now.y + dy[i];
			int nx = now.x + dx[i];

			if (ny < 1) ny = N;
			if (ny > N) ny = 1;
			if (nx < 1) nx = M;
			if (nx > M) nx = 1;

			if (visited[ny][nx] != 0) continue;
			if (map[ny][nx] == 0) continue;

			visited[ny][nx] = visited[now.y][now.x] + 1;
			NODE next;
			next.y = ny;
			next.x = nx;
			q.push(next);
		}
	}

	return visited[targeter.y][targeter.x];
}

void dfs(int depth, int lv, int sy, int sx) {
	if (findPath) return;
	if (lv >= depth) {
		if (sy == targeter.y && sx == targeter.x) {
			int ny = attacker.y;
			int nx = attacker.x;
			for (int i = 0; i < (lv - 1); ++i) {
				ny = ny + dy[path[i]];
				nx = nx + dx[path[i]];

				if (ny < 1) ny = N;
				if (ny > N) ny = 1;
				if (nx < 1) nx = M;
				if (nx > M) nx = 1;

				if (map[ny][nx] == 0) continue;

				map[ny][nx] -= (attacker.att / 2);
				attackedTower[ny][nx] = 1;
			}

			attackedTower[attacker.y][attacker.x] = 1;
			attackedTower[targeter.y][targeter.x] = 1;
			map[targeter.y][targeter.x] -= (attacker.att);

			findPath = true;
		}
		return;
	}
	if (findPath) return;
	for (int i = 0; i < 4; ++i) {
		path[lv] = i;

		int ny = sy + dy[i];
		int nx = sx + dx[i];

		if (ny < 1) ny = N;
		if (ny > N) ny = 1;
		if (nx < 1) nx = M;
		if (nx > M) nx = 1;

		if (map[ny][nx] == 0) continue;

		dfs(depth, lv + 1, ny, nx);
		if (findPath) return;
		path[lv] = 0;
	}
	if (findPath) return;
}

void lazerAtt(int dist) {
	memset(path, 0, sizeof(path));
	memset(attackedTower, 0, sizeof(attackedTower));
	findPath = false;
	dfs(dist, 0, attacker.y, attacker.x);
}

void bombAtt() {
	for (int i = 0; i < 8; ++i) {
		int ny = targeter.y + by[i];
		int nx = targeter.x + bx[i];

		if (ny < 1) ny = N;
		if (ny > N) ny = 1;
		if (nx < 1) nx = M;
		if (nx > M) nx = 1;

		if (map[ny][nx] == 0) continue;

		map[ny][nx] -= (attacker.att / 2);
		attackedTower[ny][nx] = 1;
	}

	attackedTower[attacker.y][attacker.x] = 1;
	attackedTower[targeter.y][targeter.x] = 1;
	map[targeter.y][targeter.x] -= (attacker.att);
}

void checkBrokenTower() {
	for (int y = 1; y <= N; ++y) {
		for (int x = 1; x <= M; ++x) {
			if (map[y][x] < 0) map[y][x] = 0;
		}
	}
}

void updateTower() {
	for (int y = 1; y <= N; ++y) {
		for (int x = 1; x <= M; ++x) {
			if (map[y][x] == 0) continue;
			if (attackedTower[y][x] == 1) continue;

			map[y][x] += 1;
		}
	}
}

void getAns() {
	int ans = 0;

	for (int y = 1; y <= N; ++y) {
		for (int x = 1; x <= M; ++x) {
			ans = max(ans, map[y][x]);
		}
	}

	cout << ans;
}

void sol() {
	for (int turn = 1; turn <= K; ++turn) {
		selectAttackerAndTargeter();
		if (attacker.y == targeter.y && attacker.x == targeter.x) {
			plusAttackAbillity();
			break;
		}
		plusAttackAbillity();
		int shortestDist = bfs();
		if (shortestDist == 0) {
			bombAtt();
		}
		else {
			lazerAtt(shortestDist - 1);
		}

		checkBrokenTower();
		updateTower();

		attacker.lat = turn;
		memset(attackedTower, 0, sizeof(attackedTower));
		lastAttackTime[attacker.y][attacker.x] = turn;
	}


	getAns();
}

void in() {
	cin >> N >> M >> K;
	for (int y = 1; y <= N; ++y) {
		for (int x = 1; x <= M; ++x) {
			cin >> map[y][x];
		}
	}
}

int main() {
	//freopen("input.txt", "r", stdin);
	in();
	sol();
	return 0;
}