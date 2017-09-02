#include "imstring.h"
#include <string>
#include <cassert>
#include <vector>
#include <cstring>
#include <iterator>
#include <algorithm>
#include <iostream>

template<class STR1, class STR2>
void test_strings_equal(STR1& str1, STR2 &str2)
{
    assert(str1.size() == str2.size());
    assert(std::equal(str1.begin(), str1.end(), str2.begin()));
    assert(std::distance(str1.begin(), str1.end()) ==
            std::distance(str2.begin(), str2.end()));
    // call operator << before we force strings to be not lazy
    std::cout << str1 << std::endl << str2 << std::endl;
    assert(strcmp(str1.c_str(), str2.c_str()) == 0);
}

int main()
{
    std::string str1("foo bar buzz");

    // Test creation from implicit c string
    imstring imstr1 = str1.c_str();
    test_strings_equal(str1, imstr1);
    test_strings_equal(imstr1, str1);

    // Test copying of shared immutable string buffer
    imstring imstr1_cp(imstr1);
    test_strings_equal(str1, imstr1);
    test_strings_equal(imstr1, imstr1_cp);
    std::vector<imstring> blabla;
    for (int i = 0; i < 10000; i++)
    {
        blabla.push_back(imstr1_cp);
    }
    assert(imstr1.begin() == imstr1.begin());
    assert(imstr1.end() == imstr1.end());
    // check laziness
    assert(&(*imstr1.begin()) == &(*imstr1_cp.begin()));

    // Test lazy concatenation
    imstring imstr_concat1 = imstr1 + imstr1_cp;
    imstring imstr_concat2 = imstr_concat1 + imstr_concat1 +
        imstr1 + imstr1_cp + imstr_concat1;
    std::string str_concat1 = str1 + str1;
    std::string str_concat2 = str_concat1 + str_concat1 +
        str1 + str1 + str_concat1;
    test_strings_equal(str_concat2, imstr_concat2);
    test_strings_equal(imstr_concat2, str_concat2);

    return 0;
}
