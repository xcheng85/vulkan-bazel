#pragma once

#include <thread>
#include <condition_variable>
#include <exception>
#include <mutex>
#include <memory>
#include <format>
#include <queue>


// pub sub between multiple threads

// ADT container: queue/stream --> thread-safe

// producer: thread to call push

// multiple consumers: to call read

namespace cpp_low_latency
{
    namespace mt
    {
        using namespace std;

        template<typename T>
        class stream {

            private:
                queue<T> _queue;
        };
    }
}