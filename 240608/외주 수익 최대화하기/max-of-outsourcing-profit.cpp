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
vector<PII> outPjt;
int used[N_MAX];
int maxSum;
int path[N_MAX];

void input() {
	outPjt.push_back(make_pair(0, 0));
	cin >> N;
	FOR(y, 1, N + 1) {
		int t, p;
		cin >> t >> p;
		outPjt.push_back(make_pair(t, p));
	}
}

int getSchedular(int len) {
	int schedular[N_MAX] = { 0, };

	int pastDay = 0;
	FOR(i, 0, len) {
		int nowDay = path[i];
		if (nowDay > pastDay) {
			if (schedular[nowDay] == 0) {
				if ((N + 1) < outPjt[nowDay].first + nowDay) break;
				FOR(j, nowDay, outPjt[nowDay].first + nowDay) {
					schedular[j] = nowDay;
				}
				pastDay = nowDay;
			}
		}
		else break;
	}

	int sum = 0;
	int day = 0;
	FOR(i, 1, N + 1) {
		if (day == schedular[i]) continue;
		if (schedular[i] != 0) {
			day = schedular[i];
			sum += outPjt[schedular[i]].second;
		}
	}

	return sum;
}

void dfs(int lv, int now) {
	if (lv >= N) {
		int sum = getSchedular(lv);
		maxSum = max(maxSum, sum);
		/*FOR(i, 0, lv) {
			cout << path[i] << ' ';
		}
		cout << '\n';*/
		return;
	}
	FOR(to, 1, outPjt.size()) {
		if (used[to] == 1) continue;
		used[to] = 1;
		path[lv] = to;
		dfs(lv + 1, to);
		path[lv] = 0;
		used[to] = 0;
	}
}

void sol() {
	FOR(i, 1, outPjt.size()) {
		dfs(0, i);
	}

	cout << maxSum;
}

int main() {
	//freopen("input.txt", "r", stdin);

	input();
	sol();

	return 0;
}