#include <iostream>

int main()
{
    using namespace std;

    double sum = 0.0;

    for (unsigned int i = 0; i < 5; i++) 
    {
        double number;
        cin >> number;
        sum += number;
    }

    sum /= 5;

    cout << sum << endl;
    return 0;
}
