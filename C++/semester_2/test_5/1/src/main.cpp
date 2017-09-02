#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "signal.hpp"
#include <memory>
#include <string>
#include <functional>


namespace {
	template<class T>
	int get42(T const &) {
		return 42;
	}

	template<class T>
	int get_value(T const &, int value) {
		return value;
	}
}

TEST_CASE("simple tests")
{
	SECTION("sum") {
		size_t sum = 0;
		auto callback = [&sum](size_t x) {
			sum += x;
		};

		utils::signal<void(size_t)> signal;
		signal(6);
		signal.connect(callback);
		signal(10);
		CHECK(sum == 10);
		utils::signal<void(size_t)>::connection connection = signal.connect(callback);
		signal(3);
		CHECK(sum == 16);
		connection.disconnect();
		signal(4);
		CHECK(sum == 20);
	}

	SECTION("different callbacks") {
		struct tmp_struct
		{
			int y = 12;
		};
		utils::signal<int(tmp_struct const &)> signal;
		signal.connect(get42<tmp_struct>);
		signal.connect(std::bind(get_value<tmp_struct>, std::placeholders::_1, 12));
		signal.connect([](tmp_struct const &x) {
			return x.y;
		});
		signal(tmp_struct());
	}

	SECTION("move signal and connection") {
		utils::signal<int(int)> signal;
		std::shared_ptr<int> shared_int(std::make_shared<int>(456));
		signal.connect([ancor = shared_int](int x) {return x; });
		{
			utils::signal<int(int)> signal2;
			signal2 = utils::signal<int(int)>(std::move(signal));
			CHECK_FALSE(shared_int.unique()); // ancor + shared_int

		} // ancor removed here
		CHECK(shared_int.unique());
	}
}

TEST_CASE("disconnect after signal removed")
{
	std::unique_ptr<utils::signal<int(size_t)>> signal(
		new utils::signal<int(size_t)>());
	auto connection = signal->connect([](size_t) { return 1; });
	signal.reset();
	connection.disconnect();
}

TEST_CASE("multiple disconnect")
{
	utils::signal<size_t(size_t)> signal;
	auto connection = signal.connect([](size_t) {
		FAIL("Shouldn't be called");
		return 1;
	});
	connection.disconnect();
	connection.disconnect();
	connection.disconnect();
	signal(12);
}


TEST_CASE("test signal with variadic template")
{
	utils::signal<void(size_t, int, std::string *, char&)> signal;
	std::string test("test");
	signal.connect([&test](size_t x, int y, std::string *z, char &c) {
		CHECK(z == &test);
		CHECK(x == 12);
		CHECK(y == 32);
		c = '!';
	});
	char c;
	signal(12, 32, &test, c);
	CHECK(c == '!');
}


#ifdef TASK2

TEST_CASE("test unique_connection")
{
	size_t callbacks_called = 0;
	auto callback = [&callbacks_called](size_t) {
		callbacks_called += 1;
	};

	utils::signal<void(size_t)> signal;
	typedef typename utils::signal<void(size_t)>::unique_connection unique_connection_t;
	{
		unique_connection_t unique_connection(signal.connect(callback));
		signal(0);
		CHECK(callbacks_called == 1);
	}
	signal(0);
	CHECK(callbacks_called == 1);
	{
		unique_connection_t unique_connection(signal.connect(callback));

		unique_connection_t(std::move(unique_connection)); // should be disconnected here

		signal(0);
		CHECK(callbacks_called == 1);

		unique_connection = signal.connect(callback);

		signal(0);
		CHECK(callbacks_called == 2);
	}
	signal(0);
	CHECK(callbacks_called == 2);

}

#endif // TASK2
#ifdef TASK3

TEST_CASE("reqursive signal call and remove a callback from another callback")
{
	utils::signal<size_t(size_t)> signal;
	size_t count_called1 = 0;
	auto connection = signal.connect([&count_called1](size_t) {
		return count_called1 += 1;
	});
	signal.connect([&signal, &connection](size_t x) {
		static size_t called_req = 0;
		CHECK(x == 12 + called_req);
		if (called_req < 4) {
			called_req += 1;
			if (called_req == 3) {
				connection.disconnect();
			}
			signal(12 + called_req);
		}
		return 1;
	});
	size_t count_called2 = 0;
	signal.connect([&count_called2](size_t) {
		return count_called2 += 1;
	});
	signal(12);
	CHECK(count_called1 <= 4); // 1st callback can be called up to 4 times. All values from 
							   // 0 to 4 are valid. Depends on callbacks call order. Can't be equal 5

	CHECK(count_called2 == 5);
}

TEST_CASE("add a callback from another callback")
{
	utils::signal<size_t(size_t)> signal;
	size_t count_called;
	std::function<size_t(size_t)> count_called_callback;
	std::vector<utils::signal<size_t(size_t)>::connection> connections;
	count_called_callback = [&](size_t x) {
		for (size_t i = 0; i < x; ++i) {
			connections.emplace_back(signal.connect(count_called_callback));
		}
		return count_called += 1;
	};
	connections.emplace_back(signal.connect(count_called_callback));
	count_called = 0;
	signal(0); // 1 called, 0 added
	CHECK(count_called == 1);
	count_called = 0;
	signal(3); // 1 called, 3 added
	CHECK(count_called == 1);
	count_called = 0;
	signal(1); // 4 called, 4 added
	CHECK(count_called == 4);
	count_called = 0;
	signal(10); // 8 called, 80 added
	CHECK(count_called == 8);
	count_called = 0;
	signal(0); // 88 called, 0 added
	CHECK(count_called == 88);
	std::for_each(connections.begin(), connections.end(),
		[](utils::signal<size_t(size_t)>::connection &conn) {
		conn.disconnect();
	});
	count_called = 0;
	signal(0); // 0 called, 0 added
	CHECK(count_called == 0);
}

TEST_CASE("a callback disconecting itself")
{
	utils::signal<std::string(std::string const &)> signal;
	std::unique_ptr<typename utils::signal<std::string(std::string const &)>::connection>
		connection_ptr;
	std::shared_ptr<int> shared_int(std::make_shared<int>(456));
	connection_ptr.reset(new utils::signal<std::string(std::string const &)>::connection(
		signal.connect([&connection_ptr, ancor = shared_int, &shared_int](std::string const &str) {
		connection_ptr->disconnect();
		CHECK_FALSE(shared_int.unique()); // ancor shouldn't be deleted here
		return str;
	})));
	signal("ok GO");
	CHECK(shared_int.unique()); // ancor should be deleted here
}

#endif // TASK3
