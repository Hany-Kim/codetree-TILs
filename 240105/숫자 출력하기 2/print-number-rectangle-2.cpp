#include <iostream>
using namespace std;

int n, m;
int map[100][100];

int main() {
    cin >> n >> m;
    int s = 1;
    for (int y = 0; y < n; ++y) {
        if (y % 2 == 0) {
            for (int x = 0; x < m; ++x) {
                map[y][x] = s;
                ++s;
            }
        }
        else if (y % 2 == 1) {
            for (int x = (m - 1); x >= 0; --x) {
                map[y][x] = s;
                ++s;
            }
        }
    }

    for (int y = 0; y < n; ++y) {
        for (int x = 0; x < m; ++x) {
            cout << map[y][x] << ' ';
        }
        cout << '\n';
    }
    return 0;
}