#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <set>
#include <list>

#include <assert.h>
#include <sstream>
#include "serialization.h"

template<class type>
bool eq_container(type const& lhs, type const& rhs)
{
    if (lhs.size() != rhs.size())
        return false;

    for (auto it = lhs.begin(), jt = rhs.begin(); it != lhs.end(); ++it, ++jt)
        if (!(*it == *jt))
            return false;

    return true;
}


class Base
{
    virtual void f();
};

class Derived: public Base
{
    Derived() = default;
    void f();
};

static void test_pod_serialization()
{
    using namespace std;
    using namespace serialization;
    std::stringstream stream;
    std::ostream &os = stream;
    std::istream &is = stream;

    /*
    std::pair<int, int> xyz(1,2);
    serialize(os, xyz);
    deserialize(is, xyz);
    assert(xyz.first == 1 and xyz.second == 2);
    */

    struct mypod_t
    {
        size_t a;
        size_t b;
        unsigned bit:1;
    };
    mypod_t mypod = { 1, 2, 0 };
    serialize(os, mypod);

    vector<Derived> blabla;
//  serialize(os, blabla); 
//  deserialize(is, blabla);

    mypod_t mypod2;
    deserialize(is, mypod2);

    assert((mypod.a == mypod2.a)
            && (mypod.b == mypod2.b)
            && (mypod.bit == mypod2.bit));
}

static void test_pod_vector_serialization()
{
    using namespace std;
    using namespace serialization;
    std::stringstream stream;
    std::ostream &os = stream;
    std::istream &is = stream;

    std::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    serialize(os, v);

    vector<int> v2;
    deserialize(is, v2);

    assert(eq_container(v, v2));
}

static void test_nonpod_vector_serialization()
{
    using namespace std;
    using namespace serialization;
    std::stringstream stream;
    std::ostream &os = stream;
    std::istream &is = stream;

    std::vector<string> v;
    v.push_back("1");
    v.push_back("2");
    v.push_back("3");
    v.push_back("4");
    serialize(os, v);

    vector<string> v2;
    deserialize(is, v2);

    assert(eq_container(v, v2));
}


static void test_nonpod_map_serialization()
{
    using namespace std;
    using namespace serialization;
    std::stringstream stream;
    std::ostream &os = stream;
    std::istream &is = stream;

    {
        std::map<string, int> v;
        v["1"] = 1;
        v["2"] = 2;
        v["3"] = 3;
        v["4"] = 4;
        serialize(os, v);

        map<string, int> v2;
        deserialize(is, v2);

        assert(eq_container(v, v2));
    }
}


struct custom_record
{
    custom_record()
        : number(0)
    {}

    custom_record(std::string const& t, int n)
        : text  (t)
        , number(n)
        , texts (n, t)
    {}

    friend void serialize(std::ostream& s, custom_record const& r)
    {
        serialization::serialize(s, r.text);
        serialization::serialize(s, r.number);
        serialization::serialize(s, r.texts);
    }

    friend void deserialize(std::istream& s, custom_record& r)
    {
        serialization::deserialize(s, r.text);
        serialization::deserialize(s, r.number);
        serialization::deserialize(s, r.texts);
    }

    friend bool operator==(custom_record const& lhs,
            custom_record const& rhs)
    {
        return lhs.text == rhs.text
            && lhs.number == rhs.number
            && eq_container(lhs.texts, rhs.texts);
    }
private:
    std::string text;
    int number;
    std::vector<std::string> texts;
};

static void test_custom_struct_serialization()
{
    using namespace std;
    using namespace serialization;
    std::stringstream stream;
    std::ostream &os = stream;
    std::istream &is = stream;

    list<custom_record> v;
    v.push_back(custom_record("1", 1));
    v.push_back(custom_record("2", 2));
    v.push_back(custom_record("3", 3));
    v.push_back(custom_record("4", 4));
    serialize(os, v);

    list<custom_record> v2;
    deserialize(is, v2);

    assert(eq_container(v, v2));
}

int main()
{
    // Task 1
    test_pod_serialization();
    // Task 2
    test_pod_vector_serialization();
    // Task 3
    test_nonpod_vector_serialization();
    
    // Task 4
    test_nonpod_map_serialization();

    // Task 5
    test_custom_struct_serialization();
    return 0;
}
