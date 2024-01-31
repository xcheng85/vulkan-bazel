#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <unordered_set>
#include <iostream>
#include <format>
#include <queue>

// Example 1:
// Input: numCourses = 2, prerequisites = [[1,0]]
// Output: true
// Explanation: There are a total of 2 courses to take.
// To take course 1 you should have finished course 0. So it is possible.
// Example 2:

// Input: numCourses = 2, prerequisites = [[1,0],[0,1]]
// Output: false
// Explanation: There are a total of 2 courses to take.
// To take course 1 you should have finished course 0, and to take course 0 you should also have finished course 1. So it is impossible.

// dependencies problems

// Kahn's BFS (think about the binary tree BFS)

// memory footprint
// 1. adjacent list
// 2. in-degree: o(n)

// steps
// 1. build adj list
// 2. build in-degree vector
// 3. BFS --> queue: difference with tree is: tree only have 1 root, and graph could have > 0

// 4. every step, neighboring-- and push to queue if zero in-degree.
// 5. good termination: 0 left
// 6. bad termination: more than one left and there is no 0 in-degree node

namespace leetcode_207
{
    using namespace std;

    class Solution
    {
    public:
        bool canFinish(int numCourses, vector<vector<int>> &&prerequisites)
        {
            //  edge is directional
            vector<int> inDegree(numCourses);
            vector<vector<int>> adj(numCourses);

            for (const auto &e : prerequisites)
            {
                const auto src = e[0];
                const auto dst = e[1];

                adj[src].emplace_back(dst);
                inDegree[dst]++;
            }

            queue<int> bfs;
            for(int i = 0; i < numCourses; i++){
                if(inDegree[i] == 0) {
                    bfs.emplace(i);
                }
            }
            int numNodesProcessed = 0;
            while(bfs.size() > 0) {
                const auto& current = bfs.front();
                bfs.pop();
                numNodesProcessed++;
                for(size_t n = 0; n < adj[current].size(); n++){
                    // operator[] return non-const to reference
                    if(--inDegree[n] == 0) {
                        bfs.push(n);
                    }
                }
            }
            return numNodesProcessed == numCourses;
        }
    };

}