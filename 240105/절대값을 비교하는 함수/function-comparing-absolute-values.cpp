#include <iostream>
#include <math.h>
using namespace std;

int n;

int main() {
    cin >> n;
    for (int i = 0; i < n; ++i) {
        int n1 = 0, n2 = 0;
        cin >> n1 >> n2;

        if (abs(n1) >= abs(n2)) cout << abs(n1) << '\n';
        else if(abs(n1) < abs(n2)) cout <<  abs(n2) << '\n';
    }
    return 0;
}