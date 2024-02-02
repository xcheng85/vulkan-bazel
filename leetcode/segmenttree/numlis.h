#include <vector>
#include <iostream>
#include <format>
#include <numeric>
#include <algorithm>

using namespace std;

// num largest increasing subarray, all positive is pre-requisite

// build on top of lis

// diff: updateST logic
// query range

namespace st
{
    class NumLISOccurance
    {
    public:
        struct Prop
        {
            int v;
            int index;
        };

        struct NodeProp
        {
            int lis;
            int occurance;
        };

        NumLISOccurance(const vector<int> &input) : _input(input.size()), _st(2 * input.size())
        {
            build(input);
        }

        int MaxLISOccurance()
        {
            auto n = _input.size();
            for (const auto &[v, updatePosition] : _input)
            {
                const auto startRootNodeIndex = 0;
                const auto [lis, occurance] = maxRangeQueryWithOccurance(startRootNodeIndex, 0, n - 1, 0, updatePosition);
                // occurance could be 0
                updateST(0, 0, n - 1, updatePosition, {lis + 1, max({1, occurance})});
            }
            return _st[0].occurance;
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
        NodeProp maxRangeQueryWithOccurance(int nodePos, int ss, int se, int qs, int qe)
        {
            // query is larger than this node covered
            if (qs <= ss && qe >= se)
                return _st[nodePos];

            // query range outside
            // 0 lis + 0 occurance
            if (qs > se || qe < ss)
                return {0, 0};

            // divide search range
            int sm = (ss + se) / 2;

            // logic for merge
            auto [leftLis, leftOccurance] = maxRangeQueryWithOccurance(2 * nodePos + 1, se, sm, qs, qe);
            auto [rightLis, rightOccurance] = maxRangeQueryWithOccurance(2 * nodePos + 2, se, sm, qs, qe);

            // first compare lis value
            if (leftLis > rightLis)
            {
                return {leftLis, leftOccurance};
            }
            else if (rightLis > leftLis)
            {
                return {rightLis, rightOccurance};
            }
            else
            {
                // if equal then combine occurance
                return {leftLis, leftOccurance + rightOccurance};
            }
        }

        // update operation + recursive
        // dfs, top down
        void updateST(int nodePosition, int ss, int se, int indexToUpdate, NodeProp valueToUpdate)
        {
            // trim the branch
            if (indexToUpdate < ss || indexToUpdate > se)
                return;
            // find the leaf node
            if (ss == se)
            {
                // rely on compiler-generated operator=()
                _st[nodePosition] = valueToUpdate;
                return;
            }

            int sm = (ss + se) / 2;
            updateST(2 * nodePosition + 1, ss, sm, indexToUpdate, valueToUpdate);
            updateST(2 * nodePosition + 2, sm + 1, se, indexToUpdate, valueToUpdate);
            // max st
            // root is 0-based
            auto [leftLis, leftOccurance] = _st[2 * nodePosition + 1];
            auto [rightLis, rightOccurance] = _st[2 * nodePosition + 2];

            if (leftLis > rightLis)
            {
                _st[nodePosition] = {leftLis, leftOccurance};
            }
            else if (rightLis > leftLis)
            {
                _st[nodePosition] = {rightLis, rightOccurance};
            }
            else
            {
                // if equal then combine occurance
                _st[nodePosition] = {rightLis, leftOccurance + rightOccurance};
            }
        }

    private:
        vector<Prop> _input;
        vector<NodeProp> _st;
    };
}