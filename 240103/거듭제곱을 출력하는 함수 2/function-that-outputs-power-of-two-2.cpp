#include <iostream>
#include <math.h>
using namespace std;

int a, b;

int main() {
    // 여기에 코드를 작성해주세요.
    cin >> a >> b;

    int n1 = pow(a,b);
    int n2 = pow(b,a);

    cout << max(n1,n2) - min(n1,n2);

    return 0;
}