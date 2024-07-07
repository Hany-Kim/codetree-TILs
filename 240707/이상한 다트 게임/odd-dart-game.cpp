#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
using namespace std;

#define PII pair<int,int>
#define N_MAX (50)
#define M_MAX (50)

int n, m, k;
vector<vector<int>> v;

void input() {
	cin >> n >> m >> k;
	v = vector<vector<int>>(n, vector<int>(m, 0));

	for (int y = 0; y < n; ++y) {
		for (int x = 0; x < m; ++x) {
			cin >> v[y][x];
		}
	}
}

void output() {
	for (int y = 0; y < n; ++y) {
		cout << y + 1 << "번째 원 : ";
		for (int x = 0; x < m; ++x) {
			cout << v[y][x] << ' ';
		}
		cout << '\n';
	}
	cout << '\n';
}

int dy[4] = { -1, 0, 1, 0 };
int dx[4] = { 0, -1, 0, 1 };

int bfs(int sy, int sx, int num) {
	queue<PII> q;
	vector<vector<int>> used;
	used = vector<vector<int>>(n, vector<int>(m, 0));

	q.push(make_pair(sy, sx));
	used[sy][sx] = 1;
	int cnt = 1;

	while (!q.empty()) {
		PII now = q.front();
		q.pop();

		for (int i = 0; i < 4; ++i) {
			int ny = now.first + dy[i];
			int nx = now.second + dx[i];

			if (nx >= m) nx = 0;
			else if (nx < 0) nx = (m - 1);

			if (ny < 0 || ny >= n) continue;
			if (used[ny][nx] == 1) continue;
			if (v[ny][nx] == 0) continue;
			if (v[ny][nx] != num) continue;

			used[ny][nx] = 1;
			cnt += 1;
			q.push(make_pair(ny, nx));
		}
	}

	if (cnt > 1) {
		for (int y = 0; y < n; ++y) {
			for (int x = 0; x < m; ++x) {
				if (used[y][x] == 1) v[y][x] = 0;
			}
		}

		return cnt;
	}
	return 0;
}

bool eraseNum() {
	int eraseCnt = 0;
	/*for (int y = 0; y < n; ++y) {
		for (int x = 0; x < m; ++x) {
			if (v[y][x] == 0) continue;
			eraseCnt += bfs(y, x, v[y][x]);
		}
	}*/
	vector<vector<int>> used;
	used = vector<vector<int>>(n, vector<int>(m, 0));

	for (int y = 0; y < n; ++y) {
		for (int x = 0; x < m; ++x) {
			if (v[y][x] == 0) continue;
			for (int i = 0; i < 4; ++i) {
				int ny = y + dy[i];
				int nx = x + dx[i];

				if (nx >= m) nx = 0;
				else if (nx < 0) nx = (m - 1);

				if (ny < 0 || ny >= n) continue;
				if (v[ny][nx] == 0) continue;
				if (v[ny][nx] != v[y][x]) continue;

				eraseCnt += 1;
				used[y][x] = 1;
			}
		}
	}

	if (eraseCnt > 1) {
		for (int y = 0; y < n; ++y) {
			for (int x = 0; x < m; ++x) {
				if (used[y][x] == 1) v[y][x] = 0;
			}
		}
	}

	if (eraseCnt > 1) return true;
	else return false;
}

int getAvg() {
	int sum = 0;
	int cnt = 0;
	for (int y = 0; y < n; ++y) {
		for (int x = 0; x < m; ++x) {
			if (v[y][x] != 0) {
				sum += v[y][x];
				cnt += 1;
			}
		}
	}
	return sum / cnt;
}

void normalization() {
	int avg = getAvg();

	for (int y = 0; y < n; ++y) {
		for (int x = 0; x < m; ++x) {
			if (v[y][x] == 0) continue;
			
			if (v[y][x] > avg) v[y][x] -= 1;
			else if (v[y][x] < avg) v[y][x] += 1;
		}
	}
}

void ans() {
	int sum = 0;
	for (int y = 0; y < n; ++y) {
		for (int x = 0; x < m; ++x) {
			sum += v[y][x];
		}
	}
	cout << sum;
}

void sol() {
	//output();

	for (int rotateNum = 0; rotateNum < k; ++rotateNum) {
		int x = 0, d = 0, scale = 0;
		cin >> x >> d >> scale;

		for (int i = 0; i < n; ++i) {
			if ((i + 1) % x == 0) { // 원이 x의 배수라면
				if (d == 0) { // 시계
					vector<int> tmp;
					tmp.insert(tmp.end(), v[i].end() - (scale * 1), v[i].end());
					v[i].erase(v[i].end() - (scale * 1), v[i].end());

					tmp.insert(tmp.end(), v[i].begin(), v[i].end());
					v[i].clear();
					v[i] = tmp;
				}
				else if (d == 1) { // 반시계
					vector<int> tmp;
					tmp.insert(tmp.end(), v[i].begin(), v[i].begin() + (scale * 1));
					v[i].erase(v[i].begin(), v[i].begin() + (scale * 1));

					v[i].insert(v[i].end(), tmp.begin(), tmp.end());
				}
			}
		}
		//output();
		bool flag = eraseNum();
		//output();
		if(!flag) normalization();
		//output();
	}

	ans();
}

int main() {
	//freopen("input.txt", "r", stdin);
	input();

	sol();

	return 0;
}