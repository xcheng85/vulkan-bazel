#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <unordered_set>
#include <iostream>
#include <format>

// minimum spanning tree
// a weighted, connected, undirected graph

// return all the edges
// return the minimum weighted sum

// two algorithms
// 1. Kruskal (Greedy Algorithm)
// sorted the weight increasing order and pick the edge does not have cycle

// end condition: (V-1) edges in the spanning tree or non-exist -1

namespace leetcode_1135
{
    using namespace std;

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
    // Input: n = 3, connections = [[1,2,5],[1,3,6],[2,3,1]]
        int minimumCost(int N, vector<vector<int>> &&connections)
        {
            // 1. sorting using less lamda
            // 2. functor
            // 3. operator < 

            // auto works
            sort(connections.begin(), connections.end(), [](auto e1, auto e2){
                return e1[2] < e2[2];
            });
            
            for(const auto& e: connections){
                std::cout << std::format("{}--{}:{}", e[0], e[1], e[2]) << std::endl;
            }

            // step2: use DSU for detect cycle
            // iterate sorted edges one by one
            auto dsu = make_unique<DSU>(N + 1);
            int totalWeight = 0;
            int numEdges = 0;
            for(const auto& e: connections){
                const auto v0 = e[0];
                const auto v1 = e[1];
                const auto w = e[2];
                const auto setId0 = dsu->getSetId(v0);
                const auto setId1 = dsu->getSetId(v1);
                if(setId0 == setId1){
                    continue;
                }
                dsu->unionSets(setId0, setId1);
                totalWeight += w;
                ++numEdges;
                if(numEdges == N-1){
                    return totalWeight;
                }
            }
            return -1;
        }   
    };
}