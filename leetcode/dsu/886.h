// 886. Possible Bipartition

// disjoint set

// n = 4, dislikes = [[1,2],[1,3],[2,4]]
// 1 ->  2, 3
// 2 --> 3

// 1 <= n <= 2000

// thought process

// 1. list based representation of graph (non-directional)

// 2. DSU: modeled as struct in go and class in c++

// a. memory footprint: size of nodes
// b. initially all nodes is its own root
// c. behavior: union two nodes and get root representation of this set
// d. index to represent node and value to represent its parent

// 3. start to merge

#pragma once

#include <vector>
#include <memory>
#include <unordered_set>

namespace leetcode_886
{
    using namespace std;
    class AjacentList
    {
    public:
        explicit AjacentList(int nodeCount) : _data(nodeCount)
        {
        }
        // non-directional
        void addEdge(int v0, int v1) {
            _data[v0].insert(v1);
            _data[v1].insert(v0);
        }
        const auto& getAjacentNodes(int v) const{
            return _data.at(v);
        }
    private:
        vector<unordered_set<int>> _data;
    };

    class DSU
    {
    public:
        explicit DSU(int nodeCount)
        {
            _parents.resize(nodeCount);
            for (int i = 0; i < nodeCount; ++i)
            {
                _parents[i] = i;
            }
        }

        auto getSetId(int nodeIndex) const
        {
            auto parentNodeIdx = _parents[nodeIndex];
            if (parentNodeIdx == nodeIndex)
            {
                return nodeIndex;
            }
            return getSetId(parentNodeIdx);
        }

        void unionSets(int nodeIndex1, int nodeIndex2)
        {
            auto setId1 = getSetId(nodeIndex1);
            auto setId2 = getSetId(nodeIndex2);
            if (setId1 == setId2)
            {
                return;
            }
            _parents[setId1] = setId2;
        }

    private:
        vector<int> _parents;
    };

    class Solution
    {
    public:
        bool possibleBipartition(int n, vector<vector<int>> &&dislikes)
        {
            // why adjacent list is needed.
            // a disliks b and c , this means b and c must be in a union
            // b dislike c, logic proken, return false
            // this means I need to know all the dislikes of each member.
            auto adj = make_unique<AjacentList>(n + 1);
            auto dsu = make_unique<DSU>(n + 1);

            // loop vector using for range
            // index of vector is not const operator
            for (const auto &pair : dislikes)
            {
                adj->addEdge(pair[0], pair[1]);
            }

            for(int nodeIdx = 0; nodeIdx < n; ++nodeIdx){
                const auto& dislikedNodes = adj->getAjacentNodes(nodeIdx);
                for(const auto& n : dislikedNodes){
                    // condition1: nodeIdx and n must be in different set
                    if(dsu->getSetId(nodeIdx) == dsu->getSetId(n)){
                        return false;
                    }
                    dsu->unionSets(n, *(dislikedNodes.cbegin()));
                }
            }
            return true;
        }
    };
}