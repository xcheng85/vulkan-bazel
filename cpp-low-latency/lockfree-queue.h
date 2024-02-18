#pragma once

#include <vector>
#include <atomic>
#include <exception>
#include <stdexcept>

namespace cpp_low_latency
{
    using namespace std;
    template <typename T>
    class LockFreeQueue
    {
    public:
        LockFreeQueue() = delete;
        LockFreeQueue(size_t maxQueueSize) : _data(maxQueueSize)
        {
        }
        LockFreeQueue(const LockFreeQueue &) = delete;
        LockFreeQueue(LockFreeQueue &&) = delete;
        LockFreeQueue &operator=(const LockFreeQueue &) = delete;
        LockFreeQueue &operator=(LockFreeQueue &&) = delete;

        const T *nextRead() const
        {
            // readoffset should never catch up with write offset
            return _nextReadOffset == _nextWriteOffset ? nullptr : &_data[_nextReadOffset];
        }

        T *nextWrite()
        {
            return &_data[_nextWriteOffset];
        }

        void updateNextWriteOffset()
        {
            _nextWriteOffset = (_nextWriteOffset + 1) % _data.size();
            _numUnreads++;
        }

        void updateNextReadOffset()
        {
            _nextReadOffset = (_nextReadOffset + 1) % _data.size();
            if (_numUnreads == 0)
            {
                throw std::runtime_error("Nothing to be read");
            }
            _numUnreads--;
        }

        auto unreadSize() const {
            return _numUnreads;
        }

    private:
        vector<T> _data;
        atomic<size_t> _nextWriteOffset{0};
        atomic<size_t> _nextReadOffset{0};
        atomic<size_t> _numUnreads{0};
    };
}