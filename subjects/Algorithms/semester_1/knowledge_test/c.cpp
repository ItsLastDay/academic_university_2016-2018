#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;
#define DBG(x) cout << #x << " = " << x << "\n";
/*
Compile:
    -Wall -Wextra -pedantic -std=c++11 -O2 -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wcast-qual -Wcast-align -fwhole-program -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -lmcheck -fsanitize=address 

    -std=c++11 -O2
 
Check for memory usage:
    valgrind --tool=massif
*/

int main()
{
    cin.sync_with_stdio(false);
    int n;

    cin >> n;
    for(int i = 2; i * static_cast<long long>(i) <= n; i++) {
        while(n % i == 0) {
            cout << i << " ";
            n /= i;
        }
    }
    if(n > 1)
        cout << n;
    return 0;
}
