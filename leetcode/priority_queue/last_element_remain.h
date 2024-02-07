#pragma once

#include <vector>
#include <queue>

// 1. pick the largest two values. max queue
//

namespace last_element_remain
{
    using namespace std;

    class Solution
    {
    public:
        // { 3, 5, 2, 7 }
        int lastElementRemain(vector<int> &&input)
        {
            priority_queue<int> pq;
            for (const auto &i : input)
            {
                pq.emplace(i);
            }
            while(pq.size() >= 2) {
                auto v1 = pq.top();
                pq.pop();
                auto v2 = pq.top();
                pq.pop();  

                if(v1 != v2) {
                    pq.emplace(v1 - v2);
                }      
            }
            return pq.size() == 0 ? -1 : pq.top();
        }
    };
}