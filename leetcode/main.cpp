#include <spdlog/spdlog.h>
#include "./dsu/886.h"
#include "./mst/1135.h"
#include "./topologysort/207.h"
#include "./shortestpath/787.h"
#include "./segmenttree/iterative.h"
#include "./segmenttree/MaxSubarraySum.h"
#include "./segmenttree/lis.h"
#include "./segmenttree/numlis.h"

int main()
{
    {
        leetcode_886::Solution sln;
        // uniform init
        spdlog::info(sln.possibleBipartition(4, {{1, 2}, {1, 3}, {2, 4}}));
        // n = 3, dislikes = [[1,2],[1,3],[2,3]]
        spdlog::info(sln.possibleBipartition(3, {{1, 2}, {1, 3}, {2, 3}}));
    }

    {
        leetcode_1135::Solution sln;
        //  n = 3, connections = [[1,2,5],[1,3,6],[2,3,1]]
        spdlog::info(sln.minimumCost(3, {{1, 2, 5}, {1, 3, 6}, {2, 3, 1}}));
        spdlog::info(sln.minimumCost(4, {{1, 2, 3}, {3, 4, 4}}));
    }

    {
        leetcode_207::Solution sln;
        spdlog::info(sln.canFinish(2, {{1, 0}}));
        spdlog::info(sln.canFinish(2, {{1, 0}, {0, 1}}));
    }

    {
        leetcode_787::Solution sln;
        // n = 4, flights = [[0,1,100],[1,2,100],[2,0,100],[1,3,600],[2,3,200]], src = 0, dst = 3, k = 1
        spdlog::info(sln.findCheapestPrice(4, {{0, 1, 100}, {1, 2, 100}, {2, 0, 100}, {1, 3, 600}, {2, 3, 200}},
                                           0, 3, 1));
        spdlog::info(sln.findCheapestPrice(3, {{0, 1, 100}, {1, 2, 100}, {0, 2, 500}},
                                           0, 2, 1));
        spdlog::info(sln.findCheapestPrice(3, {{0, 1, 100}, {1, 2, 100}, {0, 2, 500}},
                                           0, 2, 0));
    }

    {
        st::IterativeMinMax st({2, 6, 7, 5, 18, 86, 54, 2});
        cout << st << endl;
        cout << st.range_query(2, 7) << endl;

        st.update(3, 90);
        cout << st.range_query(2, 6) << endl;
    }

    {
        MaxSubarraySum::MaxSubarraySum st({1, 3, -4, 5, -2});
        cout << st.maxSubarraySum(0, 2) << endl;
    }

    {
        st::LIS lis({5, 1, 3, 9});
        cout << lis.MaxLIS() << endl;
    }

    {
        st::NumLISOccurance lis({1, 3, 5, 4, 7});
        cout << lis.MaxLISOccurance() << endl;
    }
}