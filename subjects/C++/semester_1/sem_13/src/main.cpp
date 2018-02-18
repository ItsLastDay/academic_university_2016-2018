#include "linq.h"

#include <iostream>
#include <string>
#include <vector>

using namespace linq;

using std::vector;
using std::string;

using std::cout;
using std::endl;

string to_string(int val)
{
    return std::to_string(val);
}


int main()
{
    vector<int> vec = {1,2,3,4,5,6};
    vector<string> vec_string = {"babc", "asdsad", "werewr"};

    auto res = from(vec)
        .where([](int i){return i % 2 == 0;})
        .select<int>([](int i) {return i * i;})
        .toVector();

    for (auto const& u: res)
    {
        cout << u << " ";
    }
    cout << endl;

    auto res_string = from(vec_string)
        .where([](string i) {return i[0] == 'a';})
        .count();

    cout << res_string << endl;


    vec = {1,2,3,4,5,6,7,8};
    enumerable<string> rr = from(vec)
        .select<string>(to_string)
        .select<string>([](string x) { return x + x;})
        .print(cout);


    cout << "END" << endl;
    return 0;
}
