#include <iostream>
#include <memory>
#include <chrono>
#include <complex>
#include <exception>
#include <algorithm>
#include <future>
#include <iostream>
#include <mutex>
#include <spdlog/spdlog.h>
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

using namespace std::chrono_literals;
using namespace std::complex_literals;

// functor for thread based approach
struct CriticalSection
{
    std::mutex mut;
};

class RenderPass
{
public:
    explicit RenderPass(std::string name, size_t numPasses = 1, bool debug = false)
        : _name(name), _numPasses{numPasses}, _debug{debug}
    {
        spdlog::info("ctor1!");
    }

    RenderPass(const RenderPass &src) : RenderPass(src._name, src._numPasses, src._debug)
    {
        spdlog::info("ctor2!");
    }

    // move constructor
    // no memory allocation, no except risk
    RenderPass(RenderPass &&src) noexcept
        : _name(src._name), _numPasses{src._numPasses}, _debug{src._debug}
    {
        spdlog::info("move ctor!");
    }

    // constructor delegation
    RenderPass(int numPasses = 1)
        : RenderPass("", numPasses, true)
    {
    }

    void run()
    {
        spdlog::info("run: {}", callId);
        // thread local storage
        callId++;
        this->_numPasses++;
    }

    // mimic function which throws exception
    void throwSomeExeption()
    {
        throw std::runtime_error{"throwSomeExeption"};
    }

    // wrapper to notify the caller main thread
    // c++11
    void throwSomeExeptionWrapper(std::exception_ptr &err)
    {
        try
        {
           throwSomeExeption();
        }
        catch(const std::exception& e)
        {
            err = std::current_exception();
        }
    }

    void operator()(CriticalSection &c)
    {
        // std::lock_guard<std::mutex> lockGuard(c.mut);
        //  same as previous, c++17
        std::scoped_lock lock(c.mut);
        std::this_thread::sleep_for(200ms);
        spdlog::info("Render Pass: {} done!", _name);
    }

    // assignment operator
    RenderPass &operator=(const RenderPass &s) &
    {
        spdlog::info("copy ctor is called!");
        if (this == &s)
        {
            return *this;
        }
        _numPasses = s._numPasses;
        _debug = s._debug;
        return *this;
    }

    // move assignment operator
    // no memory allocation, no exception
    RenderPass &operator=(RenderPass &&s) noexcept
    {
        spdlog::info("move ctor is called!");
        _numPasses = s._numPasses;
        _debug = s._debug;
        return *this;
    }
    // non const version
    // & only allow lvalue
    auto &operator[](size_t i) &
    {
        assert(i >= 0 && i < 2);
        if (i == 0)
        {
            return _numPasses;
        }
        else
        {
            // return 1; // auto& must be a lvalue
            return _numPasses;
        }
    }

    // works for both const and non const
    // & only allow lvalue, because it is a reference
    const auto &operator[](size_t i) const &
    {
        assert(i >= 0 && i < 2);
        if (i == 0)
        {
            return _numPasses;
        }
        else
        {
            // return 1; // auto& must be a lvalue
            return _numPasses;
        }
    }
    // for rvalue, non-reference
    const auto operator[](size_t i) &&
    {
        assert(i >= 0 && i < 2);
        if (i == 0)
        {
            return _numPasses;
        }
        else
        {
            // return 1; // auto& must be a lvalue
            return _numPasses;
        }
    }

    friend RenderPass operator+(RenderPass const &p1, RenderPass const &p2)
    {
        spdlog::info("operator+ is called!");
        return RenderPass(p1._name, p1._numPasses + p2._numPasses, p1._debug);
    }
    friend auto &NumRenderPass(RenderPass &p);
    friend const auto &NumRenderPass(const RenderPass &p);

    // thread local storage
    static thread_local size_t callId;

private:
    std::string _name;
    size_t _numPasses{0};
    bool _debug{true};
};

// best member accessor pattern
inline auto &NumRenderPass(RenderPass &p)
{
    return p._numPasses;
};

inline const auto &NumRenderPass(const RenderPass &p)
{
    return p._numPasses;
};

// initialize static class member
thread_local size_t RenderPass::callId = 0;

template <typename F, typename... Args>
inline auto Async(F &&f, Args &&...args)
{
    return std::async(std::launch::async, std::forward<F>(f), std::forward<Args>(args)...);
}

auto f()
{
    std::this_thread::sleep_for(1s);
    return 10;
}

// mutli-threading buffer

int main()
{
    // replace the default logger
    // auto new_logger = spdlog::basic_logger_mt("new_default_logger", "logs/new-default-log.txt", true);
    // spdlog::set_default_logger(new_logger);
    auto console = spdlog::stdout_color_mt("console");
    auto err_logger = spdlog::stderr_color_mt("stderr");
    spdlog::set_default_logger(console);

    constexpr auto d1{250ms};
    spdlog::info(d1.count());

    std::complex<double> c = 1.0 + 1i;
    spdlog::info(c.imag());

    auto future = Async(f);
    spdlog::info(future.get());

    spdlog::info(c.real());

    // synchronization using lock
    CriticalSection c1;
    // functor copy into internal storage for the thread
    std::thread t1(RenderPass("rp1", 1, true), std::ref(c1));
    std::thread t2(RenderPass("rp2", 2, false), std::ref(c1));
    std::thread t3(RenderPass("rp3", 3, true), std::ref(c1));

    // avoid functor copy
    RenderPass rp4{"rp4", 1, true};
    std::thread t4(std::ref(rp4), std::ref(c1));

    // thread + member function
    std::thread t5(&RenderPass::run, &rp4);
    std::thread t6(&RenderPass::run, &rp4);

    // retrieve result from thread
    // create new instance and ref it in the thread
    RenderPass rp5{"rp5", 2, true};
    std::jthread t7(&RenderPass::run, &rp5);

    // exception hander
    std::exception_ptr error;
    std::jthread t8(&RenderPass::throwSomeExeptionWrapper, &rp5, std::ref(error));

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
    t7.join();
    t8.join();

    // retrieving result, reference is the key
    spdlog::info("NumRenderPass: {}", NumRenderPass(rp5));

    if(error){
        spdlog::info("captured exception in thread");
        std::rethrow_exception(error);
    }
    // reader-writer lock: majority read and few writes
}