#include <iostream>
using namespace std;

int n, m;

int main() {
    // 여기에 코드를 작성해주세요.
    cin >> n >> m;

    for(int y = 1; y <= n; ++y){
        for(int x = 0; x < m; ++x){
            cout << (y + (x * n)) << ' ';
        }
        cout << '\n';
    }
    return 0;
}