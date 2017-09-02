#include <string>
#include <iostream>

int main()
{
    using namespace std;

    string s;
    cin >> s;

    string rev_s(s.rbegin(), s.rend());

    cout << rev_s << endl;
    return 0;
}
