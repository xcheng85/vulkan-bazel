#pragma once

#include <thread>
#include <condition_variable>
#include <exception>
#include <mutex>
#include <memory>
#include <format>
#include <queue>
#include <mutex>
#include <iostream>
#include <vector>
#include <chrono>

// pub sub between multiple threads

// ADT container: queue/stream --> thread-safe

// producer: thread to call push

// multiple consumers: to call read

// forward delcaration of template class
// friend template class

namespace cpp_low_latency
{
    namespace mt
    {
        using namespace std;
        using namespace std::chrono;

        template <class T>
        class publisher;

        template <class T>
        class consumer;

        template <typename T>
        class stream
        {
        public:
            stream() = default;
            stream(const stream &) = delete;
            stream &operator=(const stream &) = delete;
            stream(const stream &&) = delete;
            stream &operator=(const stream &&) = delete;

            friend class publisher<T>;
            friend class consumer<T>;

        private:
            mutex _mut;
            condition_variable _signal;
            queue<T> _queue;
        };
        //
        template <typename T>
        class publisher
        {
        public:
            publisher() = delete;
            explicit publisher(int id, stream<T> &stream) : _publisherId{id}, _stream{stream}
            {
            }
            void operator()()
            {
                auto randSec = rand() % 10;
                this_thread::sleep_for(seconds(randSec));
                xadd();
            }
            // mimic redis xadd command
            void xadd()
            {
                lock_guard lk{_stream._mut};
                _stream._queue.push(_publisherId);
                cout << format("publisher: {}\n", _publisherId);
                _stream._signal.notify_all();
            }

        private:
            int _publisherId;
            stream<T> &_stream;
        };

        template <typename T>
        class consumerGroup
        {
        };

        template <typename T>
        class consumer
        {
        public:
            explicit consumer(int id, stream<T> &stream) : _consumerId{id}, _stream{stream}
            {
            }
            void operator()()
            {
                while(true) {
                    xreadGroup();
                    this_thread::sleep_for(seconds(3));
                }
            }
            void xreadGroup()
            {
                unique_lock ul{_stream._mut};
                // block call
                _stream._signal.wait(ul, [this]
                                     { return !_stream._queue.empty(); });
                auto payload = _stream._queue.front();
                _stream._queue.pop();
                ul.unlock();
                cout << format("consumer: {}, xreadGroup: {}\n", _consumerId, payload);
            }

        private:
            int _consumerId;
            stream<T> &_stream;
        };

        void TestStreamPubSubSimpleCase()
        {
            // 1 pub + 1 sub thread
            stream<int> st;
            std::jthread pub{publisher<int>{123, st}};
            std::jthread sub{consumer<int>(456, st)};
            pub.join();
            sub.join();
        }

        void TestStreamPubSub()
        {
            srand(time(0));

            stream<int> st;
            constexpr int numPubs = 10;
            constexpr int numSubs = 5;

            vector<jthread> pubThreads(numPubs);
            vector<jthread> subThreads(numSubs);

            for (int i = 0; i < numPubs; i++)
            {
                pubThreads[i] = jthread(publisher<int>{i, st});
            }

            for (int i = 0; i < numSubs; i++)
            {
                subThreads[i] = jthread(consumer<int>(i, st));
            }

            for (int i = 0; i < numPubs; i++)
            {
                pubThreads[i].join();
            }
            for (int i = 0; i < numSubs; i++)
            {
                subThreads[i].join();
            }
        }
    }
}