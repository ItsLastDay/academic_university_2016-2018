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


stack<int> st;
vector<int> edges[100500];
int vis[100500];

void dfs(int v)
{
    if (vis[v] == 1)
    {
        vector<int> ans;
        cout << "YES" << endl;
        while (not st.empty() and st.top() != v)
        {
            ans.push_back(st.top());
            st.pop();
        }
        ans.push_back(v);
        reverse(ans.begin(), ans.end());
        for (const auto &u: ans)
            cout << u << " ";
        exit(0);
    }
    if (vis[v] == 2)
        return;

    vis[v] = 1;
    st.push(v);
    for (const auto &u: edges[v])
    {
        dfs(u);
    }

    st.pop();
    vis[v] = 2;
}

int main()
{   
    cin.sync_with_stdio(false);
    int n, m;
    cin >> n >> m;
    for (int i = 0; i < m; i++)
    {
        int fr, to;
        cin >> fr >> to;
        edges[fr].push_back(to);
    }

    for (int i = 1; i <= n; i++)
        dfs(i);
    cout << "NO" << endl;
    return 0;
}
