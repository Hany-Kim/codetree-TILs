#include <iostream>
#include <queue>
using namespace std;

#define PII pair<int, int>
#define N_MAX 21

int map[N_MAX][N_MAX];
int N, M;
int getScoreMap[N_MAX][N_MAX];
int dy[4] = { -1, 0, 1, 0 };
int dx[4] = { 0, -1, 0, 1 };
vector<int> rDice, cDice;
int rPos = 1, cPos = 1;
int dDice = 3;
PII posDice;
int ans;

void bfs(int sy, int sx) {
	queue<PII> q;
	int used[N_MAX][N_MAX] = { 0, };
	q.push(make_pair(sy, sx));
	used[sy][sx] = 1;
	int cnt = 1;
	int std = map[sy][sx];

	while (!q.empty()) {
		PII now = q.front();
		q.pop();

		for (int i = 0; i < 4; ++i) {
			int ny = now.first + dy[i];
			int nx = now.second + dx[i];

			if (ny<1 || ny>N || nx<1 || nx>N) continue;
			if (used[ny][nx] == 1) continue;
			if (getScoreMap[ny][nx] > 0) continue;
			if (map[ny][nx] != std) continue;
			
			used[ny][nx] = 1;
			++cnt;
			q.push(make_pair(ny, nx));
		}
	}

	int score = map[sy][sx] * cnt;
	for (int y = 1; y <= N; ++y) {
		for (int x = 1; x <= N; ++x) {
			if (used[y][x] != 1) continue;
			getScoreMap[y][x] = score;
		}
	}
}

void setScoreMap() {
	for (int y = 1; y <= N; ++y) {
		for (int x = 1; x <= N; ++x) {
			if (getScoreMap[y][x] > 0) continue;
			bfs(y, x);
		}
	}
}

void reverseDir() {
	if (dDice == 0) dDice = 2;
	else if (dDice == 1) dDice = 3;
	else if (dDice == 2) dDice = 0;
	else if (dDice == 3) dDice = 1;
}

void moveDice() {
	int ny = posDice.first + dy[dDice];
	int nx = posDice.second + dx[dDice];

	if (ny<1 || ny>N || nx<1 || nx>N) {
		reverseDir();
		ny = posDice.first + dy[dDice];
		nx = posDice.second + dx[dDice];
	}

	posDice.first = ny;
	posDice.second = nx;
}

void updateDice() {
	if (dDice == 0) {
		cPos -= 1;
		if (cPos < 0) cPos = 3;
		cDice[cPos] = rDice[rPos]; // 아랫면
	}
	else if (dDice == 1) {
		rPos -= 1;
		if (rPos < 0) rPos = 3;
		rDice[rPos] = cDice[cPos];
	}
	else if (dDice == 2) {
		cPos += 1;
		if (cPos >= 4) cPos = 0;
		rDice[rPos] = cDice[cPos];
	}
	else if (dDice == 3) {
		rPos += 1;
		if (rPos >= 4) rPos = 0;
		cDice[cPos] = rDice[rPos]; // 아랫면
	}
	cDice[(cPos + 2) % 4] = rDice[(rPos + 2) % 4]; // 윗면
}

void updateDiceDir() {
	if (rDice[rPos] > map[posDice.first][posDice.second]) {
		dDice -= 1;
		if (dDice < 0) dDice = 3;
	}
	else if (rDice[rPos] < map[posDice.first][posDice.second]) {
		dDice += 1;
		if (dDice >= 4) dDice = 0;
	}
}

void sol() {
	posDice = make_pair(1, 1);
	ans = 0;
	setScoreMap();
	for (int i = 0; i < M; ++i) {
		moveDice();
		updateDice();
		ans += (getScoreMap[posDice.first][posDice.second]);
		updateDiceDir();
	}
	cout << ans;
}

void input() {
	cin >> N >> M;
	for (int y = 1; y <= N; ++y) {
		for (int x = 1; x <= N; ++x) {
			cin >> map[y][x];
		}
	}
	rDice.push_back(4);
	rDice.push_back(6);
	rDice.push_back(3);
	rDice.push_back(1);

	cDice.push_back(5);
	cDice.push_back(6);
	cDice.push_back(2);
	cDice.push_back(1);
}

int main() {
	//freopen("input.txt", "r", stdin);
	input();
	sol();
	return 0;
}