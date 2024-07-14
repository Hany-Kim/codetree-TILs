#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <queue>
#include <algorithm>
using namespace std;

#define N_MAX 21
#define PII pair<int,int>

int n, m, c;
int map[N_MAX][N_MAX];
int mcus[N_MAX][N_MAX];
int mgoal[N_MAX][N_MAX];
struct TAXI {
	int y, x;
	int battery;
	int dist;
};
TAXI taxi;
int dy[4] = { -1,0,1,0 };
int dx[4] = { 0,-1,0,1 };
bool endFlag;

void input() {
	cin >> n >> m >> c;
	taxi.battery = c;
	for (int y = 1; y <= n; ++y) {
		for (int x = 1; x <= n; ++x) {
			cin >> map[y][x];
		}
	}
	cin >> taxi.y >> taxi.x;
	taxi.dist = 0;
	for (int i = 1; i <= m; ++i) {
		int ys = 0, xs = 0, ye = 0, xe = 0;
		cin >> ys >> xs >> ye >> xe;
		mcus[ys][xs] = i;
		mgoal[ye][xe] = i * (-1);
	}
}

bool cmp(pair<PII, int> left, pair<PII, int> right) {
	if (left.second < right.second) return true;
	if (left.second > right.second) return false;

	if (left.first.first < right.first.first) return true;
	if (left.first.first > right.first.first) return false;

	if (left.first.second < right.first.second) return true;
	if (left.first.second > right.first.second) return false;

	return false;
}

pair<PII, int> bfs() {
	pair<PII, int> ret;
	queue<pair<PII, int>> q;
	vector<pair<PII, int>> v;
	int used[N_MAX][N_MAX] = { 0, };

	q.push(make_pair(make_pair(taxi.y, taxi.x), 0));
	used[taxi.y][taxi.x] = 1;

	while (!q.empty()) {
		pair<PII, int> now = q.front();
		q.pop();

		for (int i = 0; i < 4; ++i) {
			int ny = now.first.first + dy[i];
			int nx = now.first.second + dx[i];

			if (ny<1 || ny>n || nx<1 || nx>n) continue;
			if (map[ny][nx] == 1) continue;
			if (used[ny][nx] != 0) continue;

			used[ny][nx] = used[now.first.first][now.first.second] + 1;
			q.push(make_pair(make_pair(ny, nx), used[ny][nx]));
			if(mcus[ny][nx] > 0) v.push_back(make_pair(make_pair(ny, nx), used[ny][nx] - 1));
		}
	}

	if (v.size() == 0) {
		endFlag = true;
		return make_pair(make_pair(-1,-1), -1);
	}
	sort(v.begin(), v.end(), cmp);
	ret = v[0];

	return ret;
}

void calcBattery(pair<PII, int> nextPos, bool isCus) {
	taxi.battery -= nextPos.second;
	if(isCus) taxi.dist += nextPos.second;
}

void moveTaxiToCus(pair<PII, int> cus) {
	if (cus.second > taxi.battery) {
		// 이동 불가 게임 종료
		endFlag = true;
		return;
	}
	else {
		calcBattery(cus, false);
		taxi.y = cus.first.first;
		taxi.x = cus.first.second;
	}
}

pair<PII, int> gbfs(int gNum) {
	pair<PII, int> ret;
	queue<pair<PII, int>> q;
	vector<pair<PII, int>> v;
	int used[N_MAX][N_MAX] = { 0, };

	q.push(make_pair(make_pair(taxi.y, taxi.x), 0));
	used[taxi.y][taxi.x] = 1;

	while (!q.empty()) {
		pair<PII, int> now = q.front();
		q.pop();

		for (int i = 0; i < 4; ++i) {
			int ny = now.first.first + dy[i];
			int nx = now.first.second + dx[i];

			if (ny<1 || ny>n || nx<1 || nx>n) continue;
			if (map[ny][nx] == 1) continue;
			if (used[ny][nx] != 0) continue;

			used[ny][nx] = used[now.first.first][now.first.second] + 1;
			q.push(make_pair(make_pair(ny, nx), used[ny][nx]));
			if (mgoal[ny][nx] == (gNum * -1)) v.push_back(make_pair(make_pair(ny, nx), used[ny][nx] - 1));
		}
	}

	if (v.size() == 0) {
		endFlag = true;
		return make_pair(make_pair(-1, -1), -1);
	}
	ret = v[0];

	return ret;
}

void chargeBattery() {
	taxi.battery += (taxi.dist * 2);
	taxi.dist = 0;
}

void moveTaxiToGoal(pair<PII, int> goal) {
	if (goal.second > taxi.battery) {
		// 이동 불가 게임 종료
		endFlag = true;
		return;
	}
	else {
		calcBattery(goal, true);
		taxi.y = goal.first.first;
		taxi.x = goal.first.second;

		chargeBattery();
	}
}

void sol() {
	for(int i=0; i<m; ++i) {
		// 손님 선택
		pair<PII, int> cus;
		if (mcus[taxi.y][taxi.x] > 0) {
			cus = make_pair(make_pair(taxi.y, taxi.x), 0);
		}
		else cus = bfs();
		moveTaxiToCus(cus);
		if (endFlag) { 
			taxi.battery = -1;
			break; 
		}
		pair<PII, int> goal = gbfs(mcus[cus.first.first][cus.first.second]);
		mcus[cus.first.first][cus.first.second] = 0;
		moveTaxiToGoal(goal);
		if (endFlag) {
			taxi.battery = -1;
			break;
		}
		mgoal[goal.first.first][goal.first.second] = 0;
	}
	cout << taxi.battery;
}

int main() {
	//freopen("input.txt", "r", stdin);
	input();
	sol();
	return 0;
}