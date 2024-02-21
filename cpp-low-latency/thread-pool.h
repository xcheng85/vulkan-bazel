#pragma once
#include <queue>
#include <thread>

namespace cpp_low_latency
{
    using namespace std;
    class ThreadPool
    {
    public:
        ThreadPool() = delete;
        ThreadPool(size_t size)
        {
            for(int i = 0; i < size; ++i) {
                _pool.emplace(jthread{});
            }
        }
        ThreadPool(const ThreadPool &) = delete;
        ThreadPool(ThreadPool &&) = delete;
        ThreadPool &operator=(const ThreadPool &) = delete;
        ThreadPool &operator=(ThreadPool &&) = delete;
    private:
        queue<jthread> _pool;
    };
}