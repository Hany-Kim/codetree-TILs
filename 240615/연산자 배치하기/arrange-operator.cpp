#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

#define N_MAX (11)
#define FOR(i,s,e) for(int i=(s); i<(e); i++)
#define MMIN_MAX -1100000000

int n;
int numList[N_MAX];
int calCnt[3];
char kindOfCal[3] = { '+', '-', '*' };
vector<char> calList;
vector<int> used;
char calPath[N_MAX];
int ansMax = MMIN_MAX;
int ansMin = INT_MAX;

void input() {
	cin >> n;
	
	FOR(i, 0, n) {
		cin >> numList[i];
	}
	FOR(i, 0, 3) {
		cin >> calCnt[i];
		FOR(j, 0, calCnt[i]) {
			calList.push_back(kindOfCal[i]);
		}
	}

	used = vector<int>(calList.size(), 0);
}

void dfs(int lv) {
	if (lv >= (n-1)) {
		int sum = numList[0];
		FOR(i, 0, n-1) {
			char cal = calPath[i];
			switch (cal)
			{
			case '+':
				sum += numList[i + 1];
				break;
			case '-':
				sum -= numList[i + 1];
				break;
			case '*':
				sum *= numList[i + 1];
				break;
			}
		}
		ansMax = max(ansMax, sum);
		ansMin = min(ansMin, sum);
		return;
	}
	FOR(i, 0, calList.size()) {
		if (used[i] == 1) continue;
		used[i] = 1;
		calPath[lv] = calList[i];
		dfs(lv + 1);
		calPath[lv] = ' ';
		used[i] = 0;
	}
}

void sol() {
	dfs(0);
	cout << ansMin << ' ' <<  ansMax;
}

int main() {
	//freopen("input.txt", "r", stdin);
	input();
	sol();
	return 0;
}