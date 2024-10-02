#include <iostream>
#include <algorithm>
using namespace std;

#define N_MAX (15 + 1)

struct TASK {
	int day, reward;
};
int N;
int date[N_MAX];
TASK arr[N_MAX];
int maxReward;

void dfs(int to, int rew) {
	maxReward = max(rew, maxReward);

	for (int i = to + 1; i <= N; ++i) {
		if (date[i] == 1) continue;
		TASK now = arr[i];

		for (int j = 0; j < now.day; ++j) {
			if (i + j > N) return;
			date[i + j] = 1;
		}

		dfs(i, rew + now.reward);
		
		for (int j = 0; j < now.day; ++j) {
			date[i + j] = 0;
		}
	}
}

void sol() {
	dfs(0, 0);
	cout << maxReward;
}

void in() {
	cin >> N;
	for (int i = 1; i <= N; ++i) {
		cin >> arr[i].day >> arr[i].reward;
	}
}

int main() {
	//freopen("input.txt", "r", stdin);
	in();
	sol();
	return 0;
}