#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <math.h>
using namespace std;

#define FOR(i,s,e) for(int i=(s); i<(e); i++)
#define PII pair<int, int>
#define K_MAX (100)

int k;
struct OCTA {
	int dir;
	int ptr;
	string local;
};
OCTA chair[5];
PII cmd[K_MAX];
int used[5];

void input() {
	FOR(i, 1, 5) {
		cin >> chair[i].local;
		chair[i].ptr = 0;
	}
	cin >> k;
	FOR(i, 0, k) {
		cin >> cmd[i].first >> cmd[i].second;
	}
}

int getLeftChair(int ptr) {
	FOR(i, 0, 2) {
		ptr--;
		if (ptr < 0) ptr = 7;
	}
	return ptr;
}
int getRightChair(int ptr) {
	FOR(i, 0, 2) {
		ptr++;
		if (ptr > 7) ptr = 0;
	}
	return ptr;
}

void checkRotateChair(int num) {
	if (num < 5) {
		FOR(i, (num + 1), 5) {
			int cur = getLeftChair(chair[i].ptr);
			int left = getRightChair(chair[i-1].ptr);

			if (chair[i].local[cur] != chair[i-1].local[left]) {
				used[i] = 1;

				if (chair[i - 1].dir == 1) chair[i].dir = -1;
				else if (chair[i - 1].dir == -1) chair[i].dir = 1;
			}
			else break;
		}
	}
	if (num > 0) {
		for (int i = (num - 1); i > 0; i--) {
			int cur = getRightChair(chair[i].ptr);
			int right = getLeftChair(chair[i+1].ptr);

			if (chair[i].local[cur] != chair[i + 1].local[right]) {
				used[i] = 1;

				if (chair[i + 1].dir == 1) chair[i].dir = -1;
				else if (chair[i + 1].dir == -1) chair[i].dir = 1;
			}
			else break;
		}
	}
}

void rotateChair() {
	FOR(i, 1, 5) {
		if (used[i] == 0) continue;
		
		if (chair[i].dir == 1) {
			chair[i].ptr--;
			if (chair[i].ptr < 0) chair[i].ptr = 7;
		}
		else if (chair[i].dir == -1) {
			chair[i].ptr++;
			if (chair[i].ptr > 7) chair[i].ptr = 0;
		}
	}
}

void sol() {
	FOR(i, 0, k) {
		int num = cmd[i].first; // 의자 번호
		int rotate = cmd[i].second; // 의자 회전 방향

		chair[num].dir = rotate;

		memset(used, 0, sizeof(used));
		used[num] = 1;

		checkRotateChair(num);

		rotateChair();
	}

	int sum = 0;
	FOR(i, 1, 5) {
		if (chair[i].local[chair[i].ptr] == '1') {
			sum += (pow(2, (i - 1)));
		}
	}
	cout << sum;
}

int main() {
	//freopen("input.txt", "r", stdin);
	input();
	sol();
	return 0;
}