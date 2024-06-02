#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <queue>
#include <math.h>
using namespace std;

#define N_MAX 201
#define M_MAX 201
#define FOR(i,s,e) for(int i=(s); i<(e); ++i)
#define PII pair<int,int>

int N, M;
int map[N_MAX][M_MAX];
// 도형 기준 위치
int shapesStdPoint[19][2] = {
	{0,0},
	{0,0},
	{1,0},
	{0,0},
	{0,0},
	{0,1},
	{0,0},
	{1,0},
	{0,0},
	{0,0},
	{0,1},
	{1,0},
	{0,0},
	{1,0},
	{0,0},
	{0,0},
	{0,1},
	{0,0},
	{0,1}
};
// 도형 모양
int shapes[19][4][4] = {
	{{1,1,1,1},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0}},
	{{1,0,0,0},
	{1,0,0,0},
	{1,0,0,0},
	{1,0,0,0}},
	{{0,1,1,0},
	{1,1,0,0},
	{0,0,0,0},
	{0,0,0,0}},
	{{1,0,0,0},
	{1,1,0,0},
	{0,1,0,0},
	{0,0,0,0}},
	{{1,1,0,0},
	{0,1,1,0},
	{0,0,0,0},
	{0,0,0,0}},
	{{0,1,0,0},
	{1,1,0,0},
	{1,0,0,0},
	{0,0,0,0}},
	{{1,1,0,0},
	{1,1,0,0},
	{0,0,0,0},
	{0,0,0,0}},
	{{0,1,0,0},
	{1,1,1,0},
	{0,0,0,0},
	{0,0,0,0}},
	{{1,0,0,0},
	{1,1,0,0},
	{1,0,0,0},
	{0,0,0,0}},
	{{1,1,1,0},
	{0,1,0,0},
	{0,0,0,0},
	{0,0,0,0}},
	{{0,1,0,0},
	{1,1,0,0},
	{0,1,0,0},
	{0,0,0,0}},
	{{1,1,1,0},
	{1,0,0,0},
	{0,0,0,0},
	{0,0,0,0}},
	{{1,0,0,0},
	{1,0,0,0},
	{1,1,0,0},
	{0,0,0,0}},
	{{0,0,1,0},
	{1,1,1,0},
	{0,0,0,0},
	{0,0,0,0}},
	{{1,1,0,0},
	{0,1,0,0},
	{0,1,0,0},
	{0,0,0,0}},
	{{1,1,1,0},
	{0,0,1,0},
	{0,0,0,0},
	{0,0,0,0}},
	{{1,1,0,0},
	{1,0,0,0},
	{1,0,0,0},
	{0,0,0,0}},
	{{1,0,0,0},
	{1,1,1,0},
	{0,0,0,0},
	{0,0,0,0}},
	{{0,1,0,0},
	{0,1,0,0},
	{1,1,0,0},
	{0,0,0,0}}
};
int dy[4] = { 0, 1, 0, -1};
int dx[4] = { 1, 0, -1, 0};

void input() {
	cin >> N >> M;
	FOR(y, 0, N) {
		FOR(x, 0, M) {
			cin >> map[y][x];
		}
	}
}

vector<PII> bfs(PII s, int num) {
	vector<PII> ret;

	queue<PII> q;
	int used[4][4] = { 0, };

	ret.push_back(make_pair(0,0));
	q.push(s);
	used[s.first][s.second] = 1;

	while (!q.empty()) {
		PII now = q.front();
		q.pop();

		FOR(i, 0, 4) {
			int ny = now.first + dy[i];
			int nx = now.second + dx[i];

			if (ny < 0 || ny>=4 || nx < 0 || nx>=4) continue; // 모양 맵
			if (used[ny][nx] == 1) continue;
			if (shapes[num][ny][nx] == 0) continue;

			used[ny][nx] = 1;
			q.push(make_pair(ny, nx));
			ret.push_back(make_pair(ny - s.first, nx - s.second));
		}
	}

	return ret;
}

int getBlockSum(int sy, int sx, vector<PII> path) {
	int ret = 0;

	FOR(i, 0, path.size()) {
		int ny = sy + path[i].first;
		int nx = sx + path[i].second;

		// 맵 벗어나면 모양 만들 수 없음
		if (ny<0 || ny>=N || nx<0 || nx>=M) {
			return 0;
		}

		ret += map[ny][nx];
	}

	return ret;
}

void sol() {
	int maxSum = 0;
	// 도형 한개씩 순회
	FOR(shapeNum, 0, 19) {
		PII start = make_pair(shapesStdPoint[shapeNum][0], shapesStdPoint[shapeNum][1]); // 현재 도형의 기준 위치
		vector<PII> shapePath = bfs(start, shapeNum);
		// 맵 순회
		FOR(y, 0, N) {
			FOR(x, 0, M) {
				int curSum = getBlockSum(y, x, shapePath);
				maxSum = max(maxSum, curSum);
			}
		}
	}
	cout << maxSum;
}

int main() {
	//freopen("input.txt", "r", stdin);
	input();
	sol();
	return 0;
}