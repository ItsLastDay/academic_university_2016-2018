#include <iostream>

int main()
{
    using namespace std;

    int num_stair_ways[21];

    int num_steps;
    cin >> num_steps;

    num_stair_ways[0] = 1;
    num_stair_ways[1] = 1;
    for(int i = 2; i <= num_steps; i++)
    {
        num_stair_ways[i] = num_stair_ways[i - 1] + num_stair_ways[i - 2];
    }

    cout << num_stair_ways[num_steps] << endl;
    return 0;
}
