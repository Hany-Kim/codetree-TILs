#include <iostream>
using namespace std;

int n;

int main() {
    // 여기에 코드를 작성해주세요.
    cin >> n;

    char c = 'A';
    char num = '0';

    for(int y = 0; y < (n + 1); ++y){
        for(int x = 0; x < (n - y); ++x){
            cout << c << ' ';
            c++;
            if(c > 'Z') c = 'A';
        }
        for(int x = (n - y); x < n; ++x){
            cout << num << ' ';
            num++;
            if(num > '9') num = '0';
        }
        cout << '\n';
    }

    return 0;
}