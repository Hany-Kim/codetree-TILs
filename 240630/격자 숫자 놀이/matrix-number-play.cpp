#include <iostream>
#include <queue>
#include <cstring>
#include <algorithm>
using namespace std;

#define NUM_MAX (100 + 1)
#define PII pair<int,int>

int r, c, k;
int map[NUM_MAX][NUM_MAX];
int tmap[NUM_MAX][NUM_MAX];
struct NODE {
	int first, second;
};
int rCnt, cCnt;
int gameTurn;


void input() {
	cin >> r >> c >> k;

	for (int y = 1; y <= 3; ++y) {
		for (int x = 1; x <= 3; ++x) {
			cin >> map[y][x];
		}
	}

	rCnt = 3;
	cCnt = 3;
}

bool operator<(NODE left, NODE right) {
	if (left.second > right.second) return true;
	if (left.second < right.second) return false;

	if (left.first > right.first) return true;
	if (left.first < right.first) return false;

	return false;
}

void sol() {
	while (map[r][c] != k) {
		gameTurn++;
		if (gameTurn > 100) break;
		memset(tmap, 0, sizeof(tmap));
		
		if (rCnt >= cCnt) {
			int ncCnt = cCnt;
			for (int y = 1; y <= rCnt; ++y) {
				priority_queue<int,vector<int>, greater<int>> pq;
				int cntArr[NUM_MAX] = { 0, };

				for (int x = 1; x <= cCnt; ++x) {
					if (map[y][x] == 0) continue;
					pq.push(map[y][x]);
					cntArr[map[y][x]] += 1;
				}
				priority_queue<NODE> ppq;
				while (!pq.empty()) {
					int now = pq.top();
					pq.pop();
					if (cntArr[now] == -1) continue;
					//PII newPair = make_pair(now, cntArr[now]);
					NODE newPair = { now, cntArr[now] };
					cntArr[now] = -1;
					ppq.push(newPair);
				}

				int turn = -1;
				while (!ppq.empty()) {
					turn++;
					//PII now = ppq.top();
					NODE now = ppq.top();
					ppq.pop();

					if ((turn * 2) + 1 > 100) break;
					tmap[y][(turn * 2) + 1] = now.first;
					if ((turn * 2) + 2 > 100) break;
					tmap[y][(turn * 2) + 2] = now.second;
				}

				ncCnt = max(ncCnt, (turn * 2) + 2);
				ncCnt = min(100, ncCnt);
			}
			
			cCnt = ncCnt;
		}
		else if (cCnt > rCnt) {
			int nrCnt = rCnt;
			for (int x = 1; x <= cCnt; ++x) {
				priority_queue<int, vector<int>, greater<int>> pq;
				int cntArr[NUM_MAX] = { 0, };

				for (int y = 1; y <= cCnt; ++y) {
					if (map[y][x] == 0) continue;
					pq.push(map[y][x]);
					cntArr[map[y][x]] += 1;
				}
				priority_queue<NODE> ppq;
				while (!pq.empty()) {
					int now = pq.top();
					pq.pop();
					if (cntArr[now] == -1) continue;
					//PII newPair = make_pair(now, cntArr[now]);
					NODE newPair = { now, cntArr[now] };
					cntArr[now] = -1;
					ppq.push(newPair);
				}

				int turn = -1;
				while (!ppq.empty()) {
					turn++;
					NODE now = ppq.top();
					ppq.pop();

					if ((turn * 2) + 1 > 100) break;
					tmap[(turn * 2) + 1][x] = now.first;
					if ((turn * 2) + 2 > 100) break;
					tmap[(turn * 2) + 2][x] = now.second;
				}

				nrCnt = max(nrCnt, (turn * 2) + 2);
				nrCnt = min(100, nrCnt); // 100보다 커지지 않게
			}

			rCnt = nrCnt;
		}

		memcpy(map, tmap, sizeof(tmap));

	}
	if (gameTurn > 100) cout << -1;
	else cout << gameTurn;
}

int main() {
	//freopen("input.txt", "r", stdin);
	input();
	sol();
	return 0;
}