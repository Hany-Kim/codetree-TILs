#include <iostream>
#include <math.h>
using namespace std;

float n1, n2, n3;
float sum;
float avg;

int main() {
    cin >> n1 >> n2 >> n3;

    sum = floor(n1 + 0.5) + floor(n2 + 0.5) + floor(n3 + 0.5);
    avg = floor((sum / 3) + 0.5);

    cout << (int)sum << '\n' << (int)avg;

    return 0;
}