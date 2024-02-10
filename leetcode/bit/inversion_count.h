#pragma once
#include <assert.h>
#include <vector>
#include <queue>
#include <memory>
#include <format>
#include <stack>
#include <iostream>
#include <optional>
#include <utility>
#include <tuple>
#include <unordered_map>
#include <algorithm>

// counter the larger at the right side

// means reverse iteration

// prerequisite:
// 1. all number are non-negative, >= 0. Bit array index as value, value 1 means occured
// 2. all the numbers are integer

// How to model BIT ?

// 1. use the prefix sum properties of bit to count how many occurance from beginning up to now.

// max_element algorithm on stl container return iterator

namespace bit
{
    using namespace std;

    // operator+ and operator=
    // operator<< for logging
    template <typename T = int>
        requires requires(T x) { x+x; x++; ++x; x=x; cout << x; }
    class inversiveCount
    {
    public:
        inversiveCount(const vector<T> &input) : _input(input)
        {
            // max_element return the iterator
            auto maxValue = *max_element(input.cbegin(), input.cend());
            _bit.resize(maxValue + 1); // value as index
        }
        // functor
        int operator()()
        {
            int result = 0;
            auto n = _input.size();
            for (int i = n - 1; i >= 0; --i)
            {
                // update the counter by the value _input[n-1]
                auto indexInBit = _input[i];
                result += inversionCountSum(indexInBit - 1); // input larger than 0
                updateNode(indexInBit, 1);
            }
            return result;
        }

    protected:
        // propagate downwards, index go larger
        void updateNode(int bitIndex, T delta)
        {
            while (bitIndex < _bit.size())
            {
                _bit[bitIndex] += delta;
                const auto lastBit = (bitIndex & (-bitIndex));
                bitIndex += lastBit;
            }
        }

        // propagate to parents, index go lower
        int inversionCountSum(int bitIndex)
        {
            int sum = 0;
            while (bitIndex > 0)
            {
                sum += _bit[bitIndex];
                const auto lastBit = (bitIndex & (-bitIndex));
                bitIndex -= lastBit;
            }
            return sum;
        }

    private:
        const vector<T> _input;
        // store the value represent count that less than the index
        vector<T> _bit;
    };
}