#include <vector>
#include <iostream>
#include <functional>

using namespace std;
using namespace std::placeholders;

using event_callback = function<void(const void*)>;

struct event
{
    void subscribe(event_callback callback)
    {
        callbacks.push_back(move(callback));
    }

    void fire(const void* arg)
    {
        for (auto it = callbacks.begin(); it != callbacks.end(); ++it)
        {
            (*it)(arg);
        }
    }
private:

    vector<event_callback> callbacks;
};

void foobar(int a, int b, int c, const void* arg)
{
    cout << "foobar worked: " << a << " " << b << " " << c << " " << arg<< endl;
}

int main()
{
    event ev;
    ev.subscribe(bind(foobar, 1, 2, 3, _1));
    ev.subscribe([](const void* arg) { cout << arg << endl; });

    const char* arg = "hello";
    ev.fire(arg);
    return 0;
}

