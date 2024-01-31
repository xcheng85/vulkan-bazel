#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <unordered_set>
#include <iostream>
#include <format>
#include <queue>
#include <list>
#include <utility> // for pair
#include <tuple>   // for tuple
#include <numeric>

// Graph Properties
// 1. Is it weighted
// 2. Is it have negative weight

// Greedy vs DP

// dijkstra: greedy

// memory footprint

// 1. build adj list
// 2. array for current shortest path for each node, weight + stops (extra restriction)

// 1. starting from src
// 2. every step pick the neighbor with minimum dist
// 3. update cache
// 4. return the result

// Dp-based solution
// Bottom up
// Bellman–Ford Algorithm (DP)

// Dp[nodeIndex, k] = min(Dp[neight, k-1] + weight)

// Dp[nodeIndex, 0] --> min(Dp[neightbor, 0] + weight, Dp[neightbor][1])

// unitialized: int_max,

namespace leetcode_787
{
    using namespace std;

    class Solution
    {
    public:
        // directional
        // flights: [[0,1,100],[1,2,100],[2,0,100],[1,3,600],[2,3,200]]
        int findCheapestPrice(int n, vector<vector<int>> &&flights, int src, int dst, int k)
        {
            // 1. build the adj list
            // list does not have random access ability
            // <nodeIndex, weight>
            vector<vector<tuple<int, int>>> adj(n);
            for (const auto &flight : flights)
            {
                const auto src = flight[0];
                const auto dst = flight[1];
                const auto weight = flight[2];

                // no duplicate edge as prerequisite
                adj[src].emplace_back(tuple{dst, weight});
            }

            // dp bottom up
            vector<int> currentMin(n, -1);
            queue<int> curr;
            queue<int> next;
            queue<int> empty;
            curr.emplace(src);
            currentMin[src] = 0;
            for (int i = 0; i < k + 1; i++)
            {
                while (curr.size() > 0)
                {
                    auto nodeIndex = curr.front();
                    curr.pop();

                    for (const auto &neighbor : adj[nodeIndex])
                    {
                        const auto neightborNodeIndex = std::get<0>(neighbor);
                        const auto weight = std::get<1>(neighbor);
                        next.emplace(neightborNodeIndex);
                        currentMin[neightborNodeIndex] = currentMin[neightborNodeIndex] == -1 ? currentMin[nodeIndex] + weight : std::min(currentMin[neightborNodeIndex], currentMin[nodeIndex] + weight);
                    }
                }
                curr.swap(next);
                // swap clear idiom
                next.swap(empty);
            }
            return currentMin[dst];
        }
    };
}