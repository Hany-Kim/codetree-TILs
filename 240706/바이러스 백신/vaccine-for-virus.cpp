#include <iostream>
#include <algorithm>
#include <queue>
#include <climits>
#include <cstring>
using namespace std;

#define PII pair<int,int>
#define N_MAX 50
#define M_MAX 10

int N, M;
int map[N_MAX][N_MAX];
PII hospitalList[M_MAX];
int used[M_MAX];
int hCnt;
PII combi[M_MAX];
int dy[4] = { -1, 1, 0, 0 };
int dx[4] = { 0, 0, -1, 1 };
int ans = INT_MAX;

void input() {
	cin >> N >> M;
	for (int y = 0; y < N; ++y) {
		for (int x = 0; x < N; ++x) {
			cin >> map[y][x];
			if (map[y][x] == 2) {
				hospitalList[hCnt++] = make_pair(y, x);
			}
		}
	}
}

bool allVirusDie(int visit[][N_MAX]) {
	for (int y = 0; y < N; ++y) {
		for (int x = 0; x < N; ++x) {
			if (map[y][x] == 0) {
				if (visit[y][x] == 0) return false;
			}
		}
	}
	return true;
}

int bfs() {
	queue<PII> q;
	int visit[N_MAX][N_MAX] = { 0, };

	for (int i = 0; i < M; ++i) {
		PII now = combi[i];
		q.push(make_pair(now.first, now.second));
		visit[now.first][now.second] = 1;
	}

	int cnt = 0;

	while (!q.empty()) {
		PII now = q.front();
		q.pop();

		for (int i = 0; i < 4; ++i) {
			int ny = now.first + dy[i];
			int nx = now.second + dx[i];

			if (map[ny][nx] == 1) continue;
			if (ny < 0 || ny >= N || nx < 0 || nx >= N) continue;
			if (visit[ny][nx] != 0) continue;

			visit[ny][nx] = visit[now.first][now.second] + 1;
			if(map[ny][nx] == 0) cnt = max(cnt, visit[ny][nx] - 1);
			
			q.push(make_pair(ny, nx));
		}
	}

	if (allVirusDie(visit)) return (cnt);
	else return INT_MAX;
}

void dfs(int lv, int nowH) {
	if (lv == M) {
		/*for (int i = 0; i < lv; ++i) {
			cout << combi[i].first << ", " << combi[i].second << " | ";
		}
		cout << '\n';*/
		ans = min(bfs(), ans);
		return;
	}
	for (int i = nowH; i < hCnt; ++i) {
		//if (used[i] == 1) continue;
		//used[i] = 1;
		combi[lv] = hospitalList[i];
		dfs(lv + 1, i + 1);
		combi[lv] = make_pair(0, 0);
		//used[i] = 0;
	}
}

void sol() {
	dfs(0, 0);

	if (ans == INT_MAX) cout << -1;
	else cout << ans;
}

int main() {
	//freopen("input.txt", "r", stdin);
	input();

	sol();

	return 0;
}