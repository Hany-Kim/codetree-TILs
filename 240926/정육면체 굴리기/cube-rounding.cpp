#include <iostream>
#include <vector>
using namespace std;

#define N_MAX 20
#define M_MAX 20
#define K_MAX 1000
#define PII pair<int,int>

int n, m, y, x, k;
int map[N_MAX][M_MAX];
int dirArr[K_MAX];
vector<int> dice;
int dy[5] = { 0,0,0,-1,1 };
int dx[5] = { 0,1,-1,0,0 };
PII dicePos;

void updateDice(int dir) {
    switch (dir)
    {
    case 1:
        dice = { dice[5], dice[3], dice[2], dice[0], dice[4], dice[1] };
        break;
    case 2:
        dice = { dice[3], dice[5], dice[2], dice[1], dice[4], dice[0] };
        break;
    case 3:
        dice = { dice[2], dice[4], dice[1], dice[3], dice[0], dice[5] };
        break;
    case 4:
        dice = { dice[4], dice[2], dice[0], dice[3], dice[1], dice[5] };
        break;
    }
}

bool moveDice(int dir) {
    int ny = dicePos.first + dy[dir];
    int nx = dicePos.second + dx[dir];

    if (ny < 0 || ny >= n || nx < 0 || nx >= m) return false;

    dicePos.first = ny;
    dicePos.second = nx;

    updateDice(dir);

    if (map[ny][nx] == 0) map[ny][nx] = dice[1];
    else if (map[ny][nx] != 0) {
        dice[1] = map[ny][nx];
        map[ny][nx] = 0;
    }

    return true;
}

void sol() {
    dicePos = make_pair(y, x);
    for (int i = 0; i < k; ++i) {
        bool isMove = moveDice(dirArr[i]);
        if (isMove) cout << dice[0] << '\n';
    }
}

void in() {
    cin >> n >> m >> y >> x >> k;
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < m; ++c) {
            cin >> map[r][c];
        }
    }
    for (int i = 0; i < k; ++i) {
        cin >> dirArr[i];
    }
    for (int i = 0; i < 6; ++i) {
        dice.push_back(0);
    }
}

int main() {
    //freopen("input.txt", "r", stdin);
    in();
    sol();
    return 0;
}