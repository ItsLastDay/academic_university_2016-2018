#ifdef MSVC
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "au_map.h"
#include <cassert>
#include <string>
#include <unordered_set>

#define TASK0
#define TASK1
#define TASK2
#define TASK3

/*
 * Check all the required type definitions.
 * Check node interface.
 *
 * points: 0
 */
void task0()
{
#ifdef TASK0
	using namespace containers;
    std::string str = "Hello!";
    au_map<std::string, std::string>::key_type kt = str;
	au_map<int, std::string>::value_type vt = str;
	au_map<int, std::string>::reference ref = str;
	au_map<int, std::string>::const_reference cref = str;
    (void)kt;
	(void)vt;
	(void)ref;
	(void)cref;

	au_map<int, std::string>::node node(nullptr, 777, "Hello!");
	const au_map<int, std::string>::node cnode(nullptr, 777, "Hello!");
	const au_map<int, std::string>::key_type &kref = node.get_key();
	assert(kref == 777);
	au_map<int, std::string>::reference vref = node.get_value();
	assert(vref == "Hello!");
	au_map<int, std::string>::const_reference cvref = cnode.get_value();
	assert(cvref == "Hello!");

	au_map<std::string, int>::node node2(nullptr, "World!", 777);
	int a = 5;
	node2.set_value(a);
	node2.get_value()++;
	assert(a == 5);
	assert(node2.get_value() == a + 1);
#endif // TASK0
}

/*
 * task1: constructor(), destructor,
 * insert(key, value), erase(node),
 * find(key)
 *
 * points: 1
 */
void task1()
{
#ifdef TASK1
	using namespace containers;
    au_map<int, std::string> map;
    for (size_t i = 0; i < 100; ++i)
    {
        assert(map.insert(i, std::to_string(i)));
    }
    for (size_t i = 0; i < 100; ++i)
    {
        auto node = map.find(i);
        assert(node);
        assert(node->get_key() == (int)i);
        assert(node->get_value() == std::to_string(i));
    }
    assert(!map.find(101));
    assert(!map.find(102));
    assert(!map.find(103));
    for (size_t i = 0; i < 100; ++i)
    {
        map.erase(map.find(i));
    }

    for (size_t i = 0; i < 100; ++i)
    {
        assert(!map.find(i));
    }
#endif // TASK1
}

template<class NUM>
struct modulo_eq_cmp_t
{
    modulo_eq_cmp_t(const NUM &modulo)
        : modulo_(modulo)
    {}

    bool operator()(const NUM &num1, const NUM &num2)
    {
        return (num1 % modulo_) == (num2 % modulo_);
    }
private:
    NUM modulo_;
};

/*
 * task2: copy constructor,
 * template comparator
 *
 * points: 1
 */
static void task2()
{
#ifdef TASK2
	using namespace containers;
    au_map<int, std::string> map;
    for (size_t i = 0; i < 100; ++i)
    {
        assert(map.insert(i, std::to_string(i)));
    }

    au_map<int, std::string> map2(map);
    for (size_t i = 0; i < 100; ++i)
    {
        assert(map2.find(i));
    }

    au_map<int, std::string, modulo_eq_cmp_t<int>> mapcmp(modulo_eq_cmp_t<int>(100));
    for (size_t i = 0; i < 200; ++i)
    {
        if (i < 100)
            assert(mapcmp.insert(i, std::to_string(i)));
        else
            assert(!mapcmp.insert(i, std::to_string(i)));
    }
    assert(mapcmp.find(101)->get_value() == "1");

    au_map<int, std::string, modulo_eq_cmp_t<int>> mapcmp2(mapcmp);
    for (size_t i = 100; i < 200; ++i)
    {
        assert(mapcmp2.find(i)->get_value() == std::to_string(i - 100));
    }
#endif // TASK2
}

/*
 * task3: begin(), end(),
 * empty(), size(), get_next(),
 * clear()
 *
 * points: 1
 */
static void task3()
{
#ifdef TASK3
    using namespace containers;

    au_map<std::string, std::string> map;

    assert(map.empty());
    assert(map.size() == 0);
    map.clear();

    for (size_t i = 0; i < 100; ++i)
    {
        assert(map.insert(std::to_string(i), std::to_string(i)));
    }
    assert(!map.empty());
    assert(map.size() == 100);

    std::unordered_set<std::string> map_keys;
    for (auto node = map.begin(); node != map.end(); node = node->get_next())
    {
        assert((std::stoi(node->get_key()) >= 0) &&
                (std::stoi(node->get_key()) < 100));
        assert(map_keys.insert(node->get_key()).second);
    }
    assert(map_keys.size() == 100);
    
    map.clear();
    assert(map.empty());
    assert(map.size() == 0);

#endif // TASK3
}

int main()
{
#ifdef MSVC
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	task0();
	task1();
	task2();
	task3();
	return 0;
}