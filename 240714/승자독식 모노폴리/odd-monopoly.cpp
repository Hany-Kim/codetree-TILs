#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
using namespace std;

#define PII pair<int,int>
#define N_MAX 21
#define M_MAX 401

int n, m, k;
PII map[N_MAX][N_MAX]; // pNum, k턴
PII nmap[N_MAX][N_MAX];

struct NODE {
	PII pos;
	int d;
	int dir[4][4];
	bool isCatch;
};
NODE playerList[M_MAX];
int ans;
int dy[5] = { 0, -1, 1, 0, 0 };
int dx[5] = { 0, 0, 0, -1, 1 };

void input() {
	cin >> n >> m >> k;
	for (int y = 0; y < n; ++y) {
		for (int x = 0; x < n; ++x) {
			int num = 0;
			cin >> num;
			if (num != 0) {
				map[y][x] = make_pair(num, k);
				playerList[num].pos = make_pair(y, x);
			}
			else map[y][x] = make_pair(num, 0);
		}
	}
	for (int i = 1; i <= m; ++i) {
		int num = 0;
		cin >> num;
		playerList[i].d = num;
	}
	for (int i = 1; i <= m; ++i) {
		for (int y = 0; y < 4; ++y) {
			for (int x = 0; x < 4; ++x) {
				int num = 0;
				cin >> num;
				playerList[i].dir[y][x] = num;
			}
		}
	}
}

bool isPlayerCatch() {
	for (int i = 1; i <= m; ++i) {
		if (i == 1 && playerList[i].isCatch == true)return false;
		else if (i > 1 && playerList[i].isCatch == false) return false;
	}
	return true;
}

pair<bool, PII> checkNoVisit(int num) {
	pair<bool, PII> ret; // 이동 가능, 다음 좌표
	NODE* now = &playerList[num];
	for (int i = 0; i < 4; ++i) {
		int ny = now->pos.first + dy[now->dir[now->d - 1][i]];
		int nx = now->pos.second + dx[now->dir[now->d - 1][i]];

		if (ny < 0 || ny >= n || nx < 0 || nx >= n) continue;
		if (map[ny][nx].second > 0) continue;

		ret = make_pair(true, make_pair(ny, nx));
		now->d = now->dir[now->d - 1][i];
		return ret;
	}
	ret = make_pair(false, make_pair(now->pos.first, now->pos.second));
	return ret;
}

pair<bool, PII> checkPastVisit(int num) {
	pair<bool, PII> ret; // 이동 가능, 다음 좌표
	NODE* now = &playerList[num];
	for (int i = 0; i < 4; ++i) {
		int ny = now->pos.first + dy[now->dir[now->d - 1][i]];
		int nx = now->pos.second + dx[now->dir[now->d - 1][i]];

		if (ny < 0 || ny >= n || nx < 0 || nx >= n) continue;

		if (map[ny][nx].second > 0 && map[ny][nx].first == num) {
			ret = make_pair(true, make_pair(ny, nx));
			now->d = now->dir[now->d - 1][i];
			return ret;
		}
	}
	ret = make_pair(false, make_pair(now->pos.first, now->pos.second));
	return ret;
}

void movePlayer(int num) {
	NODE* now = &playerList[num];

	pair<bool, PII> ret = checkNoVisit(num);
	if (ret.first) {
		// 독점계약 없는 칸이면
		if (nmap[ret.second.first][ret.second.second].first == 0) {
			nmap[ret.second.first][ret.second.second] = make_pair(num, k);
			now->pos = ret.second; // 이동
		}
		else { // 이미 작은 번호 있어서 먹힘
			now->isCatch = true;
		}
	}
	else {
		pair<bool, PII> pret = checkPastVisit(num);
		nmap[pret.second.first][pret.second.second] = make_pair(num, k);
		now->pos = pret.second;
	}
}

void decrease() {
	for (int y = 0; y < n; ++y) {
		for (int x = 0; x < n; ++x) {
			if (nmap[y][x].first == 0 && map[y][x].second > 0) {
				int bal = map[y][x].second - 1;
				if(bal != 0) nmap[y][x] = make_pair(map[y][x].first, bal);
				else nmap[y][x] = make_pair(0, 0);
			}
		}
	}
}

void init() {
	for (int y = 0; y < n; ++y) {
		for (int x = 0; x < n; ++x) {
			nmap[y][x] = make_pair(0, 0);
		}
	}
}

void allMovePlayer() {
	for (int i = 1; i <= m; ++i) {
		if (playerList[i].isCatch) continue;
		movePlayer(i);
	}
	decrease();
	memcpy(map, nmap, sizeof(map));
	init();
}

void sol() {
	ans = -1;
	for (int turn = 0; turn < 1000; ++turn) {
		if (isPlayerCatch()) {
			ans = turn;
			break;
		}
		allMovePlayer();
	}
	cout << ans;
	int de = 1;
}

int main() {
	// 여기에 코드를 작성해주세요.
	//freopen("input.txt", "r", stdin);
	input();
	sol();
	return 0;
}