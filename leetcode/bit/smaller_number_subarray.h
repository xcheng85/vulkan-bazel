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

// sort the input array with index

// sort the query array with index

namespace bit
{
    using namespace std;

    // operator+ and operator=
    // operator<< for logging
    template <typename T = int>
        requires requires(T x) { x+x; x++; ++x; x=x; cout << x; }
    class smallerElementsRange
    {
    public:
        smallerElementsRange(const vector<T> &input) : _bit(input.size() + 1), _inputWithIndex(input.size())
        {
            for (int i = 0; i < input.size(); ++i)
            {
                _inputWithIndex[i].first = input.at(i);
                _inputWithIndex[i].second = i;
            }
            // use pairs operator overload <
            sort(begin(_inputWithIndex), end(_inputWithIndex));
        }

        vector<int> rangeQuerySmallerElementBatch(const vector<tuple<int, int, T>> &queries)
        {
            vector<int> queryResults(queries.size());
            vector<tuple<int, int, T, int>> queryWithIndex(queries.size());
            for (size_t i = 0; i < queries.size(); ++i)
            {
                queryWithIndex[i] = tuple<int, int, T, int>{
                    get<0>(queries.at(i)),
                    get<1>(queries.at(i)),
                    get<2>(queries.at(i)),
                    i};
            }

            // 1. sorted so you don't need to rebuild the bit for each query
            // also needs to carry the original index
            sort(begin(queryWithIndex), end(queryWithIndex), [](auto v1, auto v2)
                 { return get<2>(v1) < get<2>(v2); });

            // double pointer
            // query from smaller to larger
            int currArrayIndex = 0; // pause and container by different sorted queries (ascending order)
            for (int qi = 0; qi < queryWithIndex.size(); ++qi)
            {
                const auto qs = get<0>(queryWithIndex.at(qi));
                const auto qe = get<1>(queryWithIndex.at(qi));
                const auto comparedValue = get<2>(queryWithIndex.at(qi));
                // for store result
                const auto qiOriginalIndex = get<3>(queryWithIndex.at(qi));
                // since the array is sorted, the update stops when the array value larger than comparedValue
                while(currArrayIndex < _inputWithIndex.size() && _inputWithIndex[currArrayIndex].first <= comparedValue) {
                    // fetch the original array index
                    const auto arrayOriginalIndex = _inputWithIndex[currArrayIndex].second;
                    updateNode(arrayOriginalIndex, 1);
                    currArrayIndex++;
                }
                // this query is done
                queryResults[qiOriginalIndex] = smallerElementSum(qe) - smallerElementSum(qs-1); 
            }
            return queryResults;
        }

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
        int smallerElementSum(int imax)
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
        // carry index in original array
        vector<pair<T, int>> _inputWithIndex;
        vector<T> _bit;
    };
}