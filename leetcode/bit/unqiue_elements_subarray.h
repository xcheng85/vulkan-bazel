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
#include <unordered_map>

// tricky part:

// state to propagate:

// the index of first occurance of each unqiue element

// build and query concurrently.

// cannot build first and then query due to the overwrite of element index position

// need a cache for the index of first occurance. unordered_set

// vector: swap and clean idiom

namespace bit
{
    using namespace std;

    // operator+ and operator=
    // operator<< for logging
    template <typename T = int>
        requires requires(T x) { x+x; x++; ++x; x=x; cout << x; }
    class uniqueElements
    {
    public:
        uniqueElements(const vector<T> &input) : _bit(input.size() + 1), _input(input)
        {
            for(const auto& e : _input){
                cout << e << "\n";
            }
        }

        // comparsion vs segment tree
        // [qs, qe]
        int rangeQueryUniqueElement(int qs, int qe)
        {
            // does not work swap 
            vector<T> tmp(_input.size() + 1);
            _bit.swap(tmp);
            // [value, index]
            unordered_map<int, int> hashmap;

            // rebuild every time.
            for (int i = 0; i <= qe; ++i)
            {
                const auto value = _input.at(i);
                // cout << value << "\n";
                // check cached index
                if (hashmap.find(value) != hashmap.end())
                {
                    // to create the delta. or else when you do the uniqueElementSum, you will have 0 for the dup
                    // clean the old index counter impact on the bit
                    const auto oldIndex = hashmap.at(value);
                    // counter--
                    updateNode(oldIndex, -1);
                }
                hashmap[value] = i;
                // increase counter for the curent node and propogate
                updateNode(i, 1);
            }
            return uniqueElementSum(qe) - uniqueElementSum(qs - 1);
        }

        // vector<int> rangeQueryUniqueElementBatch(const vector<pair> queries){

        // }

    protected:
        // propagate downwards, index go larger
        void updateNode(int index, T delta)
        {
            auto indexOffset = ++index;
            while (indexOffset < _bit.size())
            {
                _bit[indexOffset] += delta;
                const auto lastBit = (indexOffset & (-indexOffset));
                indexOffset += lastBit;
            }
        }

        // propagate to parents, index go lower
        int uniqueElementSum(int imax)
        {
            auto indexOffset = ++imax;
            int sum = 0;
            while (indexOffset > 0)
            {
                sum += _bit[indexOffset];
                const auto lastBit = (indexOffset & (-indexOffset));
                indexOffset -= lastBit;
            }
            return sum;
        }

    private:
        const vector<T> &_input;
        vector<T> _bit;
    };
}