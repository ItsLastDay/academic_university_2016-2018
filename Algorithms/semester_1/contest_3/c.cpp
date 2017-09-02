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

#define prev sdfsdfsd

map<string, int> dst;
map<string, string> prev;

const int dx[8] = {-2, -2, -1, -1, 1, 1, 2, 2};
const int dy[8] = {1, -1, 2, -2, 2, -2, 1, -1};

int main()
{   
    cin.sync_with_stdio(false);
    string s1, s2;
    cin >> s1 >> s2;

    dst[s1] = 0;
    queue<string> q;
    q.push(s1);
    while(not q.empty())
    {
        string s = q.front();
        q.pop();
        for (int i = 0; i < 8; i++)
        {
            int nx = s[0] - 'a' + dx[i];
            int ny = s[1] - '1' + dy[i];
            if (nx < 0 or nx >= 8 or ny < 0 or ny >= 8)
                continue;
            string nxt = "";
            nxt += nx + 'a';
            nxt += ny + '1';
            if (dst.count(nxt))
               continue;
            dst[nxt] = dst[s] + 1;
            prev[nxt] = s; 
            q.push(nxt);
        }
    }

    vector<string> answer;
    while (s2 != s1)
    {
        answer.push_back(s2);
        s2 = prev[s2];
    }

    answer.push_back(s1);
    reverse(answer.begin(), answer.end());
    for (const auto &s: answer)
        cout << s << "\n";
    return 0;
}
