#include <chrono>

#include "thread_pool.hpp"
#include "catch.hpp"

using au::thread_pool;


// тривиальный тест
TEST_CASE("one thread task submitting")
{
	thread_pool tp(1, 2);
	bool runned = false;
	tp.submit([&runned]() { runned = true; }).get();
	CHECK(runned);
}


// тривиальный тест
TEST_CASE("set thread count")
{
	thread_pool tp(1, 2);
	CHECK(tp.threads_count() == 1);
	tp.set_threads_count(10);
	CHECK(tp.threads_count() == 10);
}


// тривиальный тест
TEST_CASE("set max queue size")
{
	thread_pool tp(1, 2);
	CHECK(tp.max_queue_size() == 2);
	tp.set_max_queue_size(10);
	CHECK(tp.max_queue_size() == 10);
}


// нетривиальный тест
TEST_CASE("recursive submit, set_thread_count")
{
	thread_pool tp(1, 1);
	std::function<size_t(size_t)> fib;
	std::mutex thread_count_change;
	fib = [&tp, &fib, &thread_count_change](size_t n) -> size_t {
		if (n <= 1) {
			return 1;
		}
		{
			std::unique_lock<std::mutex> lock(thread_count_change);
			tp.set_threads_count(tp.threads_count() + 2);
		}
		auto res1 = tp.submit(fib, n - 1);
		auto res2 = tp.submit(fib, n - 2);
		return res1.get() + res2.get();
	};
	CHECK(tp.submit(fib, 6).get() == 13);
	CHECK(tp.threads_count() == 25);
}


// нетривиальный тест
TEST_CASE("test blocking queue")
{
	size_t thread_count = 0;
	size_t max_queue_size = 0;

	SECTION("case 1")
	{
		max_queue_size = 1;
		thread_count = 1;
	}

	SECTION("case 2")
	{
		max_queue_size = 13;
		thread_count = 1;
	}

	SECTION("case 3")
	{
		max_queue_size = 1;
		thread_count = 13;
	}

	SECTION("case 4")
	{
		max_queue_size = 132;
		thread_count = 13;
	}

	thread_pool tp(thread_count, max_queue_size);

	std::mutex mutex;
	mutex.lock();
	for (size_t idx = 0; idx < max_queue_size + thread_count; ++idx) {
		tp.submit([&mutex] { std::unique_lock<std::mutex> lock(mutex); });
	}

	std::atomic_bool wait_over(false);
	std::future<void> wait_future;
	std::thread wait_submit([&tp, &wait_over, &wait_future] {
		wait_future = tp.submit([&wait_over] { wait_over = true; });
	});

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	CHECK(wait_over == false);
	mutex.unlock();
	wait_submit.join();
	wait_future.get();
	CHECK(wait_over == true);
}

// Примеры нетривиальных тестов:
// - количество одновременно работающих задач == thread_cout
// - во время set_thread_count возможно использование тредпула (задачи выполняются)
// - во время set_thread_count можно вызвать другой set_thread_count и ничего не ломается
// - сложная конфигурация вызова задач, set_thread_count, set_max_queue работает как ожидается
// - и т.д.

// added test
TEST_CASE("number of concurrent tasks = number of threads = max queue size")
{
    const int NUM_THREADS = 10;
    thread_pool tp(NUM_THREADS, NUM_THREADS);

    auto time_start = std::chrono::high_resolution_clock::now();

    std::vector<std::future<void>> tasks;
    for (int i = 0; i < NUM_THREADS; i++)
    {
        tasks.push_back(tp.submit([] {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }));
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        tasks[i].get();
    }
    
    auto time_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = time_end - time_start;

    // Each thread sleeps for one second. They are supposed to be parallel.
    // Therefore, approximately once second should pass. I give 0.5 seconds
    // (half of the sleeping time) more for unexpected delays. 
    // If some thread waits for another to stop, running time will be >= 2 seconds.
    CHECK(elapsed.count() < 1.5);
}




