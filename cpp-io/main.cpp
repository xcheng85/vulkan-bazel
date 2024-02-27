#include <iostream>
#include <asio.hpp>
#include <functional> // bind
#include <format>

using namespace asio;
using namespace std;

#include <iostream>
#include <asio.hpp>

// The following is the same as c++ thread pattern

// completion token with function
void print(const asio::error_code & /*e*/)
{
    cout << "Hello, world From CB!\n";
}

// functor
class RepeatedTimerFunctor
{
public:
    RepeatedTimerFunctor() = default;
    void operator()(steady_timer &t, int &count)
    {
        if (count < 5)
        {
            std::cout << count << std::endl;
            ++count;
            t.expires_at(t.expiry() + asio::chrono::seconds(1));
            t.async_wait(std::bind(RepeatedTimerFunctor(), ref(t), ref(count)));
        }
    }
};

class RepeatedTimer
{
public:
    RepeatedTimer(io_context &ctx)
        : _timer{ctx, asio::chrono::seconds(1)},
          _timer2{ctx, asio::chrono::seconds(1)},
          _strand(asio::make_strand(ctx))
    {
        // lock-free sync through bind two completion handler to one strand instance
        // bind_executor decorator pattern.
        _timer.async_wait(asio::bind_executor(_strand, bind(&RepeatedTimer::timerCompletionHander, this)));
        _timer2.async_wait(asio::bind_executor(_strand, bind(&RepeatedTimer::timerCompletionHander2, this)));
    }
    virtual ~RepeatedTimer()
    {
        cout << format("dtor: {}\n", _count);
    }

protected:
    void timerCompletionHander()
    {
        if (_count < 10)
        {
            cout << format("timer1: {}\n", _count);
            ++_count;
            _timer.expires_at(_timer.expiry() + asio::chrono::seconds(1));
            _timer.async_wait(bind(&RepeatedTimer::timerCompletionHander, this));
        }
    }

    void timerCompletionHander2()
    {
        if (_count < 10)
        {
            cout << format("timer2: {}\n", _count);
            ++_count;
            _timer.expires_at(_timer.expiry() + asio::chrono::seconds(1));
            _timer.async_wait(bind(&RepeatedTimer::timerCompletionHander2, this));
        }
    }
private:
    steady_timer _timer;
    steady_timer _timer2;
    strand<asio::io_context::executor_type> _strand;
    int _count{0};
};

// class member function

int main()
{
    io_context io;
    steady_timer t(io, asio::chrono::seconds(5));
    // sync wait
    // t.wait();
    // async wait
    // "work"
    t.async_wait(&print);
    std::cout << "Hello, world!" << std::endl;

    // async_wait + lamda
    t.async_wait([&](const asio::error_code &)
                 { cout << "Hello, world From Lamda!\n"; });

    // functor + pipelined async_await + reference
    int count = 0;
    t.async_wait(bind(RepeatedTimerFunctor(), ref(t), ref(count)));

    // class member function
    RepeatedTimer p(io);

    // launch a separator thread
    asio::thread thread(
        [&io]()
        {
            try
            {
                io.run();
            }
            catch (std::exception &e)
            {
                std::cerr << "Exception in thread: " << e.what() << "\n";
                std::exit(1);
            }
        });
    // run the io's event processing loop
    // single thread, node.js style
    io.run();
    thread.join();
    return 0;
}