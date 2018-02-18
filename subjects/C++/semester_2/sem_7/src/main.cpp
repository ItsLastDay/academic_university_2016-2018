#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <stdexcept>
#include <numeric>
#include <algorithm>

#include "bin_search_tree.h"

typedef std::string KEY;
typedef std::vector<int> VALUE;
typedef bin_search_tree<KEY, VALUE> bst;

void test_empty_tree()
{
    bst tree;
    assert(!tree.contains("123"));
    try
    {
        tree.find("123");
        std::cerr << "bst_key_absent_exception expected" << std::endl;
        assert(false);
    }
    catch(const bst_key_absent_exception &ex) {}
}

template<class CONT>
bool cont_equal(const CONT &cont1, const CONT &cont2)
{
    for(auto it1 = cont1.begin(), it2 = cont2.begin();
            it1 != cont1.end(); ++it1, ++it2)
    {
        if (*it1 != *it2)
            return false;
    }
    return true;
}

void test_single_element_tree()
{
    bst tree;
    VALUE val({ 1, 2, 3 });
    KEY key("123");
    tree.insert(key, val);
    assert(cont_equal(tree.find(key), val));
    assert(tree.contains("123"));
}

void test_tree1()
{
    bst tree;
    tree.insert("123", VALUE({1, 2, 3}));
    tree.insert("1234", VALUE({2, 2, 3}));
    tree.insert("4123", VALUE({3, 2, 3}));
    tree.insert("12123", VALUE({4, 2, 3}));
    tree.insert("13223", VALUE({5, 2, 3}));
    tree.insert("123123", VALUE({6, 2, 3}));
    tree.insert("123232", VALUE({7, 2, 3}));

    assert(tree.contains("123"));
    assert(tree.contains("1234"));
    assert(tree.contains("4123"));
    assert(tree.contains("12123"));
    assert(tree.contains("13223"));
    assert(tree.contains("123123"));
    assert(tree.contains("123232"));

    assert(tree.find("123") == VALUE({1, 2, 3}));
    assert(tree.find("1234") == VALUE({2, 2, 3}));
    assert(tree.find("4123") == VALUE({3, 2, 3}));
    assert(tree.find("12123") == VALUE({4, 2, 3}));
    assert(tree.find("13223") == VALUE({5, 2, 3}));
    assert(tree.find("123123") == VALUE({6, 2, 3}));
    assert(tree.find("123232") == VALUE({7, 2, 3}));

    try
    {
        tree.find("123123123");
        tree.find("1239");
        tree.find("2asd");
        tree.find("1233222");
        tree.find("1");
        tree.find("4");
        tree.find("0");
        tree.find("10");
        tree.find("11");
        std::cerr << "bst_key_absent_exception expected" << std::endl;
        assert(false);
    }
    catch(const bst_key_absent_exception &ex) {}

    std::vector<KEY> keys;
    keys.reserve(5);
    keys.emplace_back("777");
    keys.emplace_back("asdf");
    keys.emplace_back("2259");
    keys.emplace_back("vvbs");
    keys.emplace_back("tgbasasdasdasdasdasdasd");
 
    std::vector<VALUE> values;
    values.reserve(5);
    values.push_back({ 1, 2, 3, 4, 5 });
    values.push_back({ 1, 2, 3, 4, 5 });
    values.push_back({ 1, 2, 3, 4, 5 });
    values.push_back({ 1, 2, 3, 4, 5 });
    values.push_back({ 1, 2, 3, 4, 5 });

    tree.insert(keys.begin(), keys.end(), values.begin());
}

void add_duplicates()
{
    using tree = bin_search_tree<int, int>;

    tree tr;
    try
    {
        std::vector<int> vec = {1,1,1};
        tr.insert(vec.begin(), vec.end(), vec.begin());
    }
    catch(bst_key_exists_exception& exc)
    {
        std::cout << exc.what() << std::endl;
        return;
    }

    assert(false);
}

void check_size()
{
    using tree = bin_search_tree<int, int>;

    tree tr;

    tr.insert(1, 1);
    tr.insert(2, 5);
    tr.insert(-1, 0);

    assert(tr.contains(2));
    assert(tr.find(-1) == 0);
    /*
    std::cout << tr.size() << std::endl;

    try
    {
        std::vector<int> vec = {1,1,1};
        tr.insert(vec.begin(), vec.end(), vec.begin());
    }
    catch(...)
    {
    }

    std::cout << tr.size() << std::endl;
    */
}

void big_test()
{
    using tree = bin_search_tree<int, int>;

    std::vector<int> keys(100, 0);
    std::iota(keys.begin(), keys.end(), 0);
    std::random_shuffle(keys.begin(), keys.end());

    tree tr;
    tr.insert(keys.begin(), keys.end(), keys.begin());

    for (int i = 0; i < 100; i++)
    {
        int elem = keys[i];
        assert(tr.find(elem) == elem);
        assert(tr.contains(elem));
    }
}

template<class FUNC>
void run_test(FUNC func)
{
    try
    {
        func();
    }
    catch(const std::bad_alloc &ex) {}
    catch(...)
    {
        std::cerr << "unexpected exception" << std::endl;
        throw;
    }
}

int main()
{
    srand(time(nullptr));
    run_test(test_empty_tree);
    run_test(test_single_element_tree);
    run_test(test_tree1);
    run_test(big_test);
    run_test(check_size);
    run_test(add_duplicates);
    return 0;
}
