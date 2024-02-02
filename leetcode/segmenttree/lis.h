#include <vector>
#include <iostream>
#include <format>
#include <numeric>
#include <algorithm>

using namespace std;

// largest increasing subarray, all positive is pre-requisite

// build on top of max st with query and a lot of update

namespace st
{
    class LIS
    {
    public:
        struct Prop
        {
            int v;
            int index;
        };

        LIS(const vector<int> &input) : _input(input.size()), _st(2 * input.size())
        {
            build(input);
        }

        int MaxLIS()
        {
            auto n = _input.size();
            for (const auto& [v, updatePosition] : _input)
            {
                const auto startRootNodeIndex = 0;
                const auto lis = maxRangeQuery(startRootNodeIndex, 0, n - 1, 0, updatePosition) + 1;
                updateST(0, 0, n - 1,updatePosition,lis);
            }
            return _st[0];
        }

    protected:
        void build(const vector<int> &input)
        {
            // 1. filling the prop vector
            for (size_t i = 0; i < input.size(); ++i)
            {
                _input[i].index = i;
                _input[i].v = input[i];
            }

            // 2. sort the input by increasing order with original index
            // not allow same value
            sort(begin(_input), end(_input), [](Prop v1, Prop v2)
                 { return v1.v < v2.v; });

            for (const auto &[v, index] : _input)
            {
                cout << format("{}:{}\n", v, index);
            }
        }

        // max: range query + recursive
        // nodePos in st
        // low-high: divide and conquer full range, search range
        // start-end: query range
        int maxRangeQuery(int nodePos, int ss, int se, int qs, int qe)
        {
            // query is larger than this node covered
            if (qs <= ss && qe >= se)
                return _st[nodePos];

            // query range outside
            if (qs > se || qe < ss)
                return 0;

            // divide search range
            int sm = (ss + se) / 2;

            return max({maxRangeQuery(2 * nodePos + 1, se, sm, qs, qe),
                        maxRangeQuery(2 * nodePos + 2, se, sm, qs, qe)});
        }

        // update operation + recursive
        // dfs, top down
        void updateST(int nodePosition, int ss, int se, int indexToUpdate, int lisValue)
        {
            // trim the branch
            if (indexToUpdate < ss || indexToUpdate > se)
                return;
            // find the leaf node
            if (ss == se)
            {
                _st[nodePosition] = lisValue;
                return;
            }

            int sm = (ss + se) / 2;
            updateST(2 * nodePosition + 1, ss, sm, indexToUpdate, lisValue);
            updateST(2 * nodePosition + 2, sm + 1, se, indexToUpdate, lisValue);
            // max st
            // root is 0-based
            _st[nodePosition] = max(_st[2 * nodePosition + 1], _st[2 * nodePosition + 2]);
        }

        // // This likes updateQuery

        // // nodePos: in st
        // // search range: [ss, st]
        // // index to update the tree(in the original array)
        // // lis: lis up to this index
        // // dfs
        // void updateTree(int nodePos, int ss, int se,
        //                 int indexToUpdate, int lis)
        // {
        //     // out of bound, do nothing
        //     if (indexToUpdate < ss || indexToUpdate > se)
        //         return;
        //     // find the node to update the lis so far
        //     if (ss == se)
        //     {
        //         _st[nodePos] = lis;
        //         return;
        //     }
        //     // divide and conquer
        //     int sm = (ss + se) / 2;
        //     updateTree(2 * nodePos + 1, ss, sm, indexToUpdate, lis);
        //     updateTree(2 * nodePos + 2, sm + 1, se, indexToUpdate, lis);

        //     // max lis for this sub range
        //     _st[nodePos] = max(_st[2 * nodePos + 1], _st[2 * nodePos + 2]);
        // }

    private:
        vector<Prop> _input;
        vector<int> _st;
    };
}