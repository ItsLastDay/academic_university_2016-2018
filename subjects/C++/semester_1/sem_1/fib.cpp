#include <fstream>

int fib(unsigned int n)
{
    // 1 1 2 3 5 8 13 ...
    if(n <= 2)
        return 1;
    int second_to_last_fib = 1, last_fib = 1;

    for (unsigned int i = 3; i <= n; i++) 
    {
        int tmp = second_to_last_fib + last_fib;
        second_to_last_fib = last_fib;
        last_fib = tmp;
    }

    return last_fib;
}

int main()
{
    using namespace std;

    ifstream inp("input.txt");
    ofstream out("output.txt");

    int n;
    inp >> n;
    out << fib(n) << endl;

    return 0;
}
