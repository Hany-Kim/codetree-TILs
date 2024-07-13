#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <cstring>
using namespace std;

#define K_MAX 10001
#define PII pair<int,int>

int k;
struct CMD {
	int t, y, x;
};
CMD cmdList[K_MAX];
int bt[3][2][2] = {
	{{ 0,0 },
	{ 0,0 }},

	{{ 0,0 },
	{ 0,1 }},

	{{ 0,0 },
	{ 1,0 }}
};
int yMap[10][4];
int ybm[6]; // y 비트 마스킹
int rMap[10][4];
int rbm[6]; // r 비트 마스킹
int score;
int ans;

void input() {
	cin >> k;
	for (int i = 0; i < k; ++i) {
		CMD* now = &cmdList[i];
		cin >> now->t >> now->y >> now->x;
	}
}

vector<pair<PII, PII>> putBlock(CMD cmd) {
	vector<pair<PII, PII>> v;
	int by = cmd.y;
	int bx = cmd.x;

	if (cmd.t != 1) { // type1이 아닐때
		for (int y = 0; y < 2; ++y) {
			int ny = by + bt[cmd.t - 1][y][0];
			int nx = bx + bt[cmd.t - 1][y][1];
			yMap[ny][nx] = 1;
			rMap[nx][3 - ny] = 1;
			v.push_back(make_pair(make_pair(ny, nx), make_pair(nx, 3-ny)));
		}
	}
	else {
		yMap[by][bx] = 1;
		rMap[bx][3 - by] = 1;
		v.push_back(make_pair(make_pair(by, bx), make_pair(bx, 3 - by)));
	}

	return v;
}

void downBlockInYellow(int t, vector<pair<PII, PII>> v) {
	int by = v[0].first.first;
	int bx = v[0].first.second;
	int nowBit = (1 << (3 - bx)); // 0 1 0 0
	// 1 0 1 0 | 0 1 0 0 => 1 1 1 0
	// 1 1 0 0 | 0 1 0 0 => 1 1 0 0

	int bsy = 0;
	int bsx = 0;
	int nowSideBit = 0;
	if (t == 2 && v.size() == 2) {
		bsy = v[1].first.first;
		bsx = v[1].first.second;
		nowSideBit = (1 << (3 - bsx));
	}

	for (int i = 0; i < 6; ++i) {
		if (((ybm[i] | nowBit) == ybm[i]) || (((ybm[i] | nowSideBit) == ybm[i]) && (t == 2))) { // 내려가다 막힘
			if (i == 0) {
				return;
			}
			else {
				if (t == 3) ybm[i - 2] = ybm[i - 2] | nowBit;
				ybm[i - 1] = ybm[i - 1] | nowBit;
				if (t == 2) ybm[i - 1] = ybm[i - 1] | nowSideBit;
				return; // 놓고 나옴
			}
		}
		else if (i == 5) {
			if (t == 3) ybm[i - 1] = ybm[i - 1] | nowBit;
			ybm[i] = ybm[i] | nowBit;
			if (t == 2)ybm[i] = ybm[i] | nowSideBit;
			return;
		}
	}
	return;
}

void downBlockInRed(int t, vector<pair<PII, PII>> v) {
	int by = v[0].second.first;
	int bx = v[0].second.second;
	int nowBit = (1 << (3 - bx)); // 0 1 0 0
	// 1 0 1 0 | 0 1 0 0 => 1 1 1 0
	// 1 1 0 0 | 0 1 0 0 => 1 1 0 0

	int bsy = 0;
	int bsx = 0;
	int nowSideBit = 0;
	if (t == 3 && v.size() == 2) {
		bsy = v[1].second.first;
		bsx = v[1].second.second;
		nowSideBit = (1 << (3 - bsx));
	}

	for (int i = 0; i < 6; ++i) {
		if (((rbm[i] | nowBit) == rbm[i]) || (((rbm[i] | nowSideBit) == rbm[i]) && (t == 3))) { // 내려가다 막힘
			if (i == 0) {
				return;
			}
			else {
				if (t == 2) rbm[i - 2] = rbm[i - 2] | nowBit;
				rbm[i - 1] = rbm[i - 1] | nowBit;
				if (t == 3) rbm[i - 1] = rbm[i - 1] | nowSideBit;
				return; // 놓고 나옴
			}
		}
		else if (i == 5) {
			if (t == 2) rbm[i - 1] = rbm[i - 1] | nowBit;
			rbm[i] = rbm[i] | nowBit;
			if (t == 3) rbm[i] = rbm[i] | nowSideBit;
			return;
		}
	}
	return;
}

void arrangeBlockYellow() {
	int yTmp[6] = { 0, };
	int yIdx = 5;

	memcpy(yTmp, ybm, sizeof(ybm));
	memset(ybm, 0, sizeof(ybm));

	for (int i = 5; i >= 0; i--) {
		if (yTmp[i] != 0) {
			ybm[yIdx--] = yTmp[i];
		}
	}
}

void arrangeBlockRed() {
	int rTmp[6] = { 0, };
	int rIdx = 5;

	memcpy(rTmp, rbm, sizeof(rbm));
	memset(rbm, 0, sizeof(rbm));

	for (int i = 5; i >= 0; i--) {
		if (rTmp[i] != 0) {
			rbm[rIdx--] = rTmp[i];
		}
	}
}

void calcScore() {
	for (int i = 5; i >= 0; i--) {
		if (ybm[i] == 15) {
			score += 1;
			ybm[i] = 0;
		}
		if (rbm[i] == 15) {
			score += 1;
			rbm[i] = 0;
		}
	}

	arrangeBlockYellow();
	arrangeBlockRed();
}

void checkYellow() {
	int cnt = 0;
	for (int i = 0; i < 2; ++i) {
		if (ybm[i] > 0) cnt++;
	}
	for (int i = 5; i > (5 - cnt); i--) {
		ybm[i] = 0;
	}
	arrangeBlockYellow();
}
void checkRed() {
	int cnt = 0;
	for (int i = 0; i < 2; ++i) {
		if (rbm[i] > 0) cnt++;
	}
	for (int i = 5; i > (5 - cnt); i--) {
		rbm[i] = 0;
	}
	arrangeBlockRed();
}

void downBlock(int t, vector<pair<PII, PII>> v) {
	downBlockInYellow(t, v);
	downBlockInRed(t, v);
	
	//점수계산
	calcScore();

	// 연한부분확인
	checkYellow();
	checkRed();
}

void getAns() {
	for (int i = 0; i < 6; ++i) {
		if (ybm[i] > 0) {
			while (ybm[i] > 0) {
				if ((ybm[i] & 1) == 1) ans++;
				ybm[i] = ybm[i] >> 1;
			}
		}
		if (rbm[i] > 0) {
			while (rbm[i] > 0) {
				if ((rbm[i] & 1) == 1) ans++;
				rbm[i] = rbm[i] >> 1;
			}
		}
	}
}

void sol() {
	for (int i = 0; i < k; ++i) {
		CMD now = cmdList[i];
		vector<pair<PII, PII>> v = putBlock(now);
		downBlock(now.t, v);
	}
	getAns();
	cout << score << '\n' << ans;
}

int main() {
	// 여기에 코드를 작성해주세요.
	//freopen("input.txt", "r", stdin);
	input();
	sol();
	return 0;
}