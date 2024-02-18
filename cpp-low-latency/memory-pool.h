#pragma once

#include <vector>

namespace cpp_low_latency
{
    using namespace std;
    template <typename T>
    class MemoryPool
    {
    public:
        MemoryPool() = delete;
        MemoryPool(size_t numChunks) : _data(numChunks, Chunk{})
        {
        }
        MemoryPool(const MemoryPool &) = delete;
        MemoryPool(MemoryPool &&) = delete;
        MemoryPool &operator=(const MemoryPool &) = delete;
        MemoryPool &operator=(MemoryPool &&) = delete;

        // variadic ctor params
        template <typename... Args>
        T *acquire(Args... args) noexcept
        {
            auto allocatedMemoryChunk = &_data[_nextFreeChunkIndex];
            assert(allocatedMemoryChunk->isFree);
            T *memoryPtr = &(allocatedMemoryChunk->data);
            // placement new operator
            memoryPtr = new (memoryPtr) T(args...);
            allocatedMemoryChunk->isFree = false;
            updateNextFreeChunkIndex();
            return memoryPtr;
        }

        void release(const T *obj) noexcept
        {
            // calculate the index from the vector
            // rely on the pointer arithmatic
            // T is the first element of Chunk.
            const auto offset = reinterpret_cast<const Chunk*>(obj) - &_data[0];
            _data[offset].isFree = true; // other obj can place new op on it
        }

    private:
        void updateNextFreeChunkIndex()
        {
            const auto current = _nextFreeChunkIndex;
            while (!_data[_nextFreeChunkIndex].isFree)
            {
                _nextFreeChunkIndex++;

                if (_nextFreeChunkIndex == _data.size())
                {
                    _nextFreeChunkIndex = 0;
                }

                if (current == _nextFreeChunkIndex)
                {
                    throw std::bad_alloc("memory pool is full");
                }
            }
        }

        struct Chunk
        {
            T data;
            bool isFree{true};
        };
        vector<Chunk> _data;
        size_t _nextFreeChunkIndex;
    };
}