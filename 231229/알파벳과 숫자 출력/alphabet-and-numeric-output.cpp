#include <iostream>
using namespace std;

int n;
char c = 'A';
char num = '1';

int main() {
    cin >> n;
    for (int y = 1; y < (n + 1); ++y) {
        for (int x = 0; x < (n + 1 - y); ++x) {
            cout << c << ' ';
            c++;
            if (c > 'Z') c = 'A';
        }
        for (int x = (n + 1 - y); x < (n + 1); ++x) {
            cout << num << ' ';
            num++;
            if (num > '9') num = '1';
        }
        cout << '\n';
    }
    return 0;
}