#pragma once

#include <vector>
#include <algorithm>
#include <thread>
#include <numeric>

namespace cpp_low_latency
{
    using namespace std;

    template <typename IteratorType, typename T>
    class AccumulateBlockFunctor
    {
    public:
        void operator()(IteratorType begin, IteratorType end, T &result)
        {
            result = accumulate(begin, end, result);
        }
    };

    // like reducer
    template <typename IteratorType, typename T>
    T AccumulateMT(IteratorType begin, IteratorType end, T initVal)
    {
        auto numThreads = std::thread::hardware_concurrency();
        auto length = distance(begin, end);
        if (!length)
        {
            return initVal;
        }
        auto blockSize = length / numThreads;
        vector<T> results(numThreads);
        // -1: due to main thread
        vector<jthread> threads; //(numThreads - 1);
        IteratorType blockStart{begin};
        for (size_t i = 0; i < (numThreads - 1); ++i)
        {
            auto blockEnd = blockStart;
            std::advance(blockEnd, blockSize);
            threads[i] = std::jthread(
                AccumulateBlockFunctor<IteratorType, T>(),
                blockStart, blockEnd, std::ref(results[i]));
            blockStart = blockEnd;
        }
        // last block is done by main thread
        AccumulateBlockFunctor<IteratorType, T>()(blockStart, end, results[numThreads - 1]);
        return accumulate(results.begin(), results.end(), initVal);
    }
}