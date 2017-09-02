// Smoke tests
#include "deque.h"
#include <cassert>
#include <iostream>

#define __TASK1__
#define __TASK2__
#define __TASK3__

using containers::deque;

        using std::cout;
        using std::endl;
void testTask1()
{
#ifdef __TASK1__
	deque< deque<int> > int_deque_deque;
	assert(int_deque_deque.size() == 0);
	int_deque_deque.push_back(deque<int>());
	deque< deque<int> >::reference front_deque = int_deque_deque.front();
	front_deque.push_front(1);
	front_deque.push_front(2);

	assert(front_deque.back() == 1);
	const deque< deque<int> >::value_type front_copy(int_deque_deque[0]);
	front_deque.pop_back();
	assert(front_copy.size() == 2);
	assert(front_copy.front() == 2);
        front_deque[0] = 1;
	front_deque = int_deque_deque.back();
	assert(front_deque.size() == deque< deque<int> >::value_type::size_type(1));

        deque<int> dq;
        for (int i = 0; i < 100; i++)
        {
            dq.push_back(i);
        }
        for (int i = 0; i < 100; i++)
        {
            assert(dq[i] == i);
        }
        assert(dq.size() == 100);
        for (int i = 0; i < 50; i++)
        {
            dq.pop_front();
        }
        assert(dq.size() == 50);
        for (int i = 50; i < 100; i++)
        {
            assert(dq[i - 50] == i);
        }
        for (int i = 50; i < 100; i++)
        {
            dq.pop_front();
        }
        assert(dq.size() == 0);

        for (int i = 0; i < 100; i++)
        {
            dq.push_back(0);
            dq.pop_front();
        }
        assert(dq.size() == 0);

        for (int i = 0; i < 1000; i++)
        {
            dq.push_front(i);
        }
        assert(dq[0] == 999);

        deque<int> d;
        d.push_back(1);
        d.push_front(2);
        d.push_back(1);
        assert(d.back() == 1);
        cout << "TEST 1 END" << endl;
#endif // __TASK1__
}

void testTask2()
{
#ifdef __TASK2__
	deque<double> double_deque;
	double_deque.push_back(0.2);
	assert(double_deque.capacity() >= 1);
	double_deque.reserve(123);
	assert(double_deque.capacity() >= 123);
	double_deque.resize(2, 7.0);
	assert(double_deque.back() == 7.0);
	assert(double_deque.front() == 0.2);
	double_deque.resize(0);
	assert(double_deque.size() == 0);
        cout << "TEST 2 END" << endl;
#endif // __TASK2__
}

void testTask3()
{
#ifdef __TASK3__
	deque<int> deque1;
	deque1.push_back(2);
	deque1.push_back(5);
	deque1.push_back(1);
	deque<int> deque2(deque1);
	assert(deque1 == deque2);
	deque2.pop_back();
	assert(deque1 != deque2);
	assert(deque1 >= deque2);
	assert(deque1 > deque2);
	deque2.pop_front();
	assert(deque1 <= deque2);
	assert(deque1 < deque2);

        deque<int> dd1;
        dd1.push_back(0);
        dd1.push_back(1);
        dd1.push_back(2);
        assert(!(dd1 < dd1));

        deque<int> dd2;
        dd2.push_back(0);
        dd2.push_back(1);
        dd2.push_back(3);

        deque<int> dd3;
        dd3.push_back(0);
        dd3.push_back(1);
        dd3.push_back(3);
        dd3.push_back(0);

        deque<int> dd4;
        dd4.push_back(1);
        dd4.push_back(1);

        assert(dd1 < dd2);
        assert(dd1 <= dd2);
        assert(dd2 > dd1);
        assert(dd1 < dd3);
        assert(dd3 > dd1);
        assert(dd1 < dd4);
        assert(dd4 > dd1);
        assert(dd2 >= dd1);
        assert(dd1 <= dd3);
        assert(dd3 >= dd1);
        assert(dd1 <= dd4);
        assert(dd4 >= dd1);
        assert(dd2 != dd1);
        assert(dd1 != dd3);
        assert(dd3 != dd1);
        assert(dd1 != dd4);
        assert(dd4 != dd1);


        assert(dd2 < dd3);
        assert(dd2 != dd3);
        assert(dd2 <= dd3);
        assert(dd2 < dd4);
        assert(dd2 != dd4);
        assert(dd2 <= dd4);

        assert(dd3 < dd4);
        assert(dd3 <= dd4);
        cout << "TEST 3 END" << endl;
#endif // __TASK3__
}

void runSmokeTests()
{
	testTask1();
	testTask2();
	testTask3();
}

int main(int, char*[])
{
	runSmokeTests();
}
