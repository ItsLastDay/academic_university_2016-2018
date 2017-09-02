#include <iostream>

#include "lazy_string.h"


int main()
{
    using namespace std;
    lazy::lazy_string s = "abacaba";
    lazy::lazy_string t = "bracadabra";
    lazy::lazy_string g = "brac";
    std::cout << s << std::endl;
    std::cout << find(s, "aba") << std::endl;
    std::cout << find(s, "aba", 1) << std::endl;
    std::cout << find(s, "aba", 5) << std::endl;

    std::cout << (s < t) << " true" << std::endl;
    std::cout << (t < s) << " false" << std::endl;
    std::cout << (s < g) << " true" << std::endl;
    std::cout << (g < s) << " false" << std::endl;
    std::cout << (t < g) << " false" << std::endl;
    std::cout << (g < t) << " true" << std::endl;
    std::cout << (s < s) << " false" << std::endl;

    s += t;
    cout << s << endl;
    cout << s[5] << endl;
    s[5] = '6';
    cout << s << endl;
    return 0;
}



