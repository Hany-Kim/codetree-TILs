#include <iostream>
#include <cstring>
using namespace std;

#define N_MAX 10
#define M_MAX 10
#define PII pair<int,int>

int n, m;
char map[N_MAX][M_MAX];
PII blue, red, goal;
int dy[4] = { -1,1, 0,0 };
int dx[4] = { 0,0, -1,1 };
int path[10];
int ans = 10;

bool moveCandy(char nmap[N_MAX][M_MAX], PII* candy, int dir) {
	bool isEscape = false;
	while (1) {
		int ny = candy->first + dy[dir];
		int nx = candy->second + dx[dir];

		if (ny < 0 || ny >= n || nx < 0 || nx >= m) break;
		if (nmap[ny][nx] == '#') break;
		if (nmap[ny][nx] == 'R' || nmap[ny][nx] == 'B') break;
		if (nmap[ny][nx] == 'O') {
			isEscape = true;
		}
		else {
			nmap[ny][nx] = nmap[candy->first][candy->second];
		}
		nmap[candy->first][candy->second] = '.';
		candy->first = ny;
		candy->second = nx;
	}
	return isEscape;
}

bool moveCandies(char nmap[N_MAX][M_MAX], int dir, PII* cBlue, PII* cRed) {
	bool isBlue = false;
	bool isRed = false;
	bool isExit = false;

	switch (dir)
	{
	case 0:
		if (cBlue->first < cRed->first && cBlue->second == cRed->second) {
			isBlue = moveCandy(nmap, cBlue, dir);
			isRed = moveCandy(nmap, cRed, dir);
		}
		else {
			isRed = moveCandy(nmap, cRed, dir);
			isBlue = moveCandy(nmap, cBlue, dir);
		}
		break;
	case 1:
		if (cBlue->first < cRed->first && cBlue->second == cRed->second) {
			isRed = moveCandy(nmap, cRed, dir);
			isBlue = moveCandy(nmap, cBlue, dir);
		}
		else {
			isBlue = moveCandy(nmap, cBlue, dir);
			isRed = moveCandy(nmap, cRed, dir);
		}
		break;
	case 2:
		if (cBlue->first == cRed->first && cBlue->second < cRed->second) {
			isBlue = moveCandy(nmap, cBlue, dir);
			isRed = moveCandy(nmap, cRed, dir);
		}
		else {
			isRed = moveCandy(nmap, cRed, dir);
			isBlue = moveCandy(nmap, cBlue, dir);
		}
		break;
	case 3:
		if (cBlue->first == cRed->first && cBlue->second < cRed->second) {
			isRed = moveCandy(nmap, cRed, dir);
			isBlue = moveCandy(nmap, cBlue, dir);
		}
		else {
			isBlue = moveCandy(nmap, cBlue, dir);
			isRed = moveCandy(nmap, cRed, dir);
		}
		break;
	}

	if (!isBlue && isRed) isExit = true;
	return isExit;
}

void dfs(int lv) {
	if (lv >= ans) {
		PII cBlue = blue;
		PII cRed = red;
		char nmap[N_MAX][M_MAX];
		memcpy(nmap, map, sizeof(map));

		for (int i = 0; i < lv; ++i) {
			bool isExit = moveCandies(nmap, path[i], &cBlue, &cRed);

			if (cBlue.first != goal.first || cBlue.second != goal.second) {
				if (cRed.first == goal.first && cRed.second == goal.second && isExit) {
					ans = min(ans, i + 1);
					break;
				}
			}
		}

		return;

	}
	for (int i = 0; i < 4; ++i) {
		path[lv] = i;
		dfs(lv + 1);
		path[lv] = 0;
	}
}

void sol() {
	dfs(0);
	if (ans == 10) cout << -1;
	else cout << ans;
}

void in() {
	cin >> n >> m;

	for (int y = 0; y < n; ++y) {
		for (int x = 0; x < m; ++x) {
			cin >> map[y][x];
			if (map[y][x] == 'B') blue = make_pair(y, x);
			if (map[y][x] == 'R') red = make_pair(y, x);
			if (map[y][x] == 'O') goal = make_pair(y, x);
		}
	}
}

int main() {
	//freopen("input.txt", "r", stdin);
	in();
	sol();
	return 0;
}