#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;

#define N_MAX 15 + 1
#define FOR(i,s,e) for(int i=(s); i<(e); ++i)
#define PII pair<int,int>

int N;
int schedular[N_MAX];
vector<PII> outPjt;
int used[N_MAX];
int maxSum;

void input() {
	outPjt.push_back(make_pair(0,0));
	cin >> N;
	FOR(y, 1, N + 1) {
		int t, p;
		cin >> t >> p;
		outPjt.push_back(make_pair(t, p));
	}
}

void dfs(int now) {
	if (schedular[now] == 0 && used[now] == 0) {
		used[now] = 1;
		FOR(i, 0, outPjt[now].first) {
			schedular[now + i] = now;
		}
	}
	if (now == N) {
		int pastDay = 0;
		int sum = 0;
		FOR(i, 1, N + 1) {
			if (pastDay == schedular[i]) continue;
			if (schedular[i] != 0) {
				pastDay = i;
				sum += outPjt[pastDay].second;
			}
		}
		maxSum = max(maxSum, sum);

		if (schedular[now] == now) {
			FOR(j, 0, outPjt[now].first) {
				schedular[now + j] = 0;
			}
		}
		return;
	}

	FOR(i, now + 1, outPjt.size()) {
		dfs(i);
		if (schedular[now] == now) {
			FOR(j, 0, outPjt[now].first) {
				schedular[now + j] = 0;
			}
		}
	}
}

void sol() {
	FOR(i, 1, outPjt.size()) {
		memset(used, 0, sizeof(used));
		dfs(i);
		used[i] = 0;
	}

	cout << maxSum;
}

int main() {
	//freopen("input.txt", "r", stdin);
	
	input();
	sol();

	return 0;
}