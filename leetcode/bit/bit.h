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

// tree
// not a binary tree
// root is dummy node
// num child at each node depends on bit manipulation (minus rightmost bit)

// behavior
// 1. build tree
// 2. update node
// 3. prefix sum
// 4. range query upon 3

// tricky
// index 1 based
// y 2’s complement

// memory footprint:
// sizeof input + 1 dummy
namespace bit
{
    using namespace std;

    // operator+ and operator=
    // operator<< for logging
    template <typename T = int>
        requires requires(T x) { x+x; x++; ++x; x=x; cout << x; }
    class bit
    {
    public:
        bit(const vector<T> &input) : _bit(input.size() + 1)
        {
            build(input);
        }

        // equal update in segment tree
        void updateNode(int index, T v)
        {
            index++; // 1-index
            while (index < _bit.size())
            {
                _bit[index] += v;
                // pre-requisite: 2's complement signed integer knowledge
                int rightMostSetBit = (index & (-index));
                // to the child, if withinbound
                index += rightMostSetBit;
            }
        }

        // comparsion vs segment tree
        // [qs, qe]
        int rangeQuery(int qs, int qe)
        {
            return prefixSum(qe) - prefixSum(qs - 1);
        }

    protected:
        void build(const vector<T> &input)
        {
            for (size_t i = 0; i < input.size(); ++i)
            {
                updateNode(i, input.at(i));
            }
        }

        // index from original input
        // 1-offset
        int prefixSum(int index)
        {
            index++;
            int sum = 0;
            while (index > 0)
            {
                sum += _bit[index];
                int rightMostSetBit = (index & (-index));
                // parent
                index -= rightMostSetBit;
            }
            return sum;
        }

    private:
        vector<T> _bit;
    };
}