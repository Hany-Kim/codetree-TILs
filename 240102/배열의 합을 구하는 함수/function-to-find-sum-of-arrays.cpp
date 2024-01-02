#include <iostream>
using namespace std;

int n;
int arr[10][10];
int dpx[11][11];
int dpy[11][11];

int main() {
    cin >> n;

    for (int y = 0; y < n; ++y) {
        for (int x = 0; x < n; ++x) {
            cin >> arr[y][x];
        }
    }

    dpx[0][0] = arr[0][0];
    dpx[1][0] = arr[1][0];
    dpx[2][0] = arr[2][0];

    for (int y = 0; y < n; ++y) {
        for (int x = 0; x < n; ++x) {
            dpx[y][x + 1] = dpx[y][x] + arr[y][x + 1];
        }
    }

    dpy[0][0] = arr[0][0];
    dpy[0][1] = arr[0][1];
    dpy[0][2] = arr[0][2];

    for (int x = 0; x < n; ++x) {
        for (int y = 0; y < n; ++y) {
            dpy[y + 1][x] = dpy[y][x] + arr[y + 1][x];
        }
    }

    int num = 0;
    for (int y = 0; y < n; ++y) {
        num += dpx[y][n];
    }

    for (int y = 0; y < (n + 1); ++y) {
        if (y == n) {
            for (int x = 0; x < (n + 1); ++x) {
                if (x == n) cout << num << ' ';
                else cout << dpy[y][x] << ' ';
            }
        }
        else {
            for (int x = 0; x < (n + 1); ++x) {
                if (x == n) cout << dpx[y][x] << ' ';
                else cout << arr[y][x] << ' ';
            }
        }

        cout << '\n';
    }

    return 0;
}