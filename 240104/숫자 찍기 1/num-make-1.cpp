#include <iostream>
#include <cstring>
using namespace std;

int num, mtype;
int map[110][110];

void func1() {
    int s = 1;
    for (int y = 1; y <= num; ++y) {
        if (y % 2 == 1) {
            for (int x = 1; x <= y; ++x) {
                map[y][x] = s;
                ++s;
            }
        }
        else if (y % 2 == 0) {
            for (int x = y; x >= 1; --x) {
                map[y][x] = s;
                ++s;
            }
        }
    }

    for (int y = 1; y <= num; ++y) {
        for (int x = 1; x <= num; ++x) {
            if (map[y][x] == -1) cout << "  ";
            else cout << map[y][x] << ' ';
        }
        cout << '\n';
    }
}
void func2() {
    int s = 0;
    for (int y = 1; y <= num; ++y) {
        for (int x = y; x <= ((num * 2) - y); ++x) {
            map[y][x] = s;
        }
        ++s;
    }

    for (int y = 1; y <= num; ++y) {
        for (int x = 1; x <= ((num * 2) - y); ++x) {
            if (map[y][x] == -1) cout << "  ";
            else cout << map[y][x] << ' ';
        }
        cout << '\n';
    }
}
void func3() {
    int s = 1;
    for (int x = 1; x <= ((num / 2) + 1); ++x) { // 1 ~ 3
        for (int y = x; y <= (num - x + 1); ++y) {
            map[y][x] = s;
        }
        ++s;
    }

    for (int y = 1; y <= num; ++y) {
        for (int x = 1; x <= ((num / 2) + 1); ++x) {
            if (map[y][x] == -1) cout << "  ";
            else cout << map[y][x] << ' ';
        }
        cout << '\n';
    }
}

int main() {
    // 여기에 코드를 작성해주세요.
    cin >> num >> mtype;
    memset(map, -1, sizeof(map));

    if (mtype == 1) func1();
    else if (mtype == 2) func2();
    else if (mtype == 3) func3();

    return 0;
}