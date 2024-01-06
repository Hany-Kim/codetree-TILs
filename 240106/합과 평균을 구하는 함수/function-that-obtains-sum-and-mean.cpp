#include <iostream>
#include <math.h>
using namespace std;

float n1, n2, n3;
int sum;
int avg;

int main() {
    cin >> n1 >> n2 >> n3;

    sum = floor(n1 + 0.5) + floor(n2 + 0.5) + floor(n3 + 0.5);
    avg = floor((sum / 3) + 0.5);

    cout << sum << '\n' << avg;

    return 0;
}