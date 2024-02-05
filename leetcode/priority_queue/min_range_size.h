#include <string>
#include <queue>
#include <unordered_map>
#include <utility>
#include <algorithm>

using namespace std;

// pre-processing
// sorting

// pq + trick

namespace pq_min_range_size
{
    // Input: Intervals[] = [[1, 4], [2, 3], [3, 6], [9, 25], [7, 15], [4, 4]]
    //            Q[] = [7, 50, 2]
    // Output: [9, -1, 2]
    class Solution
    {
    public:
        vector<int> MinRangeSize(vector<pair<int, int>> &intervals, vector<int> &query)
        {
            // 1. sort the interval (by lowerbound)
            sort(begin(intervals), end(intervals));
            // 2. sort the query (by lowerbound)
            // index in the original input query is needed
            vector<pair<int, int>> queriesWithIndex;
            for (int i = 0; i < query.size(); i++)
            {
                queriesWithIndex.push_back({query[i], i});
            }

            sort(begin(queriesWithIndex), end(queriesWithIndex));

            // 3. iterate on each query and use pq
            // Current position of intervals

            // default operator <, so tricky is -.
            // you can also use std::greater, but also the default allocator

            // pq with [-intervalsize, rightbound]

            // comparison is based on operator<
            priority_queue<vector<int>, vector<vector<int>>, greater<vector<int>>> pq;
            int i = 0;
            vector<int> result(query.size(), -1);
            for (const auto &[value, index] : queriesWithIndex)
            {
                // all the intervals that lowerbound < value will be pushed to the pq
                while (i < intervals.size() && intervals[i].first <= value)
                {
                    auto intervalSize = intervals[i].second - intervals[i].first + 1;
                    pq.emplace(vector<int>{intervalSize, intervals[i].second});
                    i++;
                }
                // remove unmatched intervals
                while (!pq.empty() && value > pq.top()[1])
                {
                    pq.pop();
                }

                // now pq have all the matched interval, the first one is good.
                if (!pq.empty()) {
                    result[index] = pq.top()[0];
                }

            }

            return result;
        }
    };
}