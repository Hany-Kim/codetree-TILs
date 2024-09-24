#include <iostream>
using namespace std;

#define N_MAX 1000000

int n;
long long arr[N_MAX];
long long teamLeader, teamMember;
long long ans;

void hasTeamLeader() {
	for (int i = 0; i < n; ++i) {
		if (arr[i] > 0) {
			arr[i] -= teamLeader;
			ans += 1;
		}
	}
}

void hasTeamMember() {
	for (int i = 0; i < n; ++i) {
		if (arr[i] > 0) {
			ans += (arr[i] / teamMember);
			if (arr[i] % teamMember > 0) ans += 1;
		}
	}
}

void in() {
	cin >> n;
	for (int i = 0; i < n; ++i) {
		cin >> arr[i];
	}
	cin >> teamLeader >> teamMember;
}
void sol() {
	hasTeamLeader();
	hasTeamMember();

	cout << ans;
}

int main() {
	//freopen("input.txt", "r", stdin);

	in();
	sol();

	return 0;
}