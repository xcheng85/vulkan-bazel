#include <spdlog/spdlog.h>
#include <queue>
#include "./dsu/886.h"
#include "./mst/1135.h"
#include "./topologysort/207.h"
#include "./shortestpath/787.h"
#include "./segmenttree/iterative.h"
#include "./segmenttree/MaxSubarraySum.h"
#include "./segmenttree/lis.h"
#include "./segmenttree/numlis.h"
#include "./segmenttree/KSubarrayMax.h"
#include "./segmenttree/PriceRangeQuery.h"
#include "./trie/139.h"
#include "./trie/validWords.h"
#include "./priority_queue/template_pq.h"
#include "./priority_queue/min_range_size.h"
#include "./priority_queue/last_element_remain.h"
#include "./bst/sum_k_smallest.h"

int main()
{
    {
        sum_k_smallest::BST_Iterative<int> bst;
        bst.Insert({50, 30, 20, 40, 70, 60, 80});
        cout << bst;

        bst.bfs();
        {
            auto [current, parent] = bst.Search(31);
            if (current.has_value())
            {
                cout << current.value()->value << "\n";
            }
            else
            {
                cerr << "no 31\n";
            }
        }

        {
            auto [current, parent] = bst.Search(30);
            if (current.has_value())
            {
                cout << current.value()->value << "\n";
            }
            else
            {
                cerr << "no 31\n";
            }
        }

        // bst.deleteNode(80);
        bst.deleteNode(70);
        bst.bfs();

        bst.deleteNode(50);
        bst.bfs();
    }
    {
        last_element_remain::Solution sln;
        cout << sln.lastElementRemain({3, 5, 2, 7}) << endl;
    }

    // pq + template + concpet
    {
        priority_queue<int> pq;
        pq.emplace(2);
        pq.emplace(1);
        pq.emplace(3);
        pq_template::Solution sln;
        // auto type deduction
        sln.printPriorityQueue(pq);
    }

    {
        vector<std::pair<int, int>> intervals{{1, 4}, {2, 3}, {3, 6}, {9, 25}, {7, 15}, {4, 4}};
        vector<int> query{7, 50, 2, 3, 4, 9};

        pq_min_range_size::Solution sln;
        auto r = sln.MinRangeSize(intervals, query);
    }

    // // trie
    // {
    //     leetcode_139::Solution sln;
    //     vector i{"leet"s, "code"s};
    //     cout << sln.wordBreak("leetcode", i) << endl;
    // }
    // {
    //     trie_validwords::Solution sln;
    //     vector dict{"go"s, "bat"s, "me"s, "eat"s, "goal"s,
    //                 "boy"s, "run"s};

    //     vector arr{'e', 'o', 'b', 'a', 'm', 'g', 'l'};
    //     sln.printAllValidWords(dict, arr);
    // }

    // {
    //     leetcode_886::Solution sln;
    //     // uniform init
    //     spdlog::info(sln.possibleBipartition(4, {{1, 2}, {1, 3}, {2, 4}}));
    //     // n = 3, dislikes = [[1,2],[1,3],[2,3]]
    //     spdlog::info(sln.possibleBipartition(3, {{1, 2}, {1, 3}, {2, 3}}));
    // }

    // {
    //     leetcode_1135::Solution sln;
    //     //  n = 3, connections = [[1,2,5],[1,3,6],[2,3,1]]
    //     spdlog::info(sln.minimumCost(3, {{1, 2, 5}, {1, 3, 6}, {2, 3, 1}}));
    //     spdlog::info(sln.minimumCost(4, {{1, 2, 3}, {3, 4, 4}}));
    // }

    // {
    //     leetcode_207::Solution sln;
    //     spdlog::info(sln.canFinish(2, {{1, 0}}));
    //     spdlog::info(sln.canFinish(2, {{1, 0}, {0, 1}}));
    // }

    // {
    //     leetcode_787::Solution sln;
    //     // n = 4, flights = [[0,1,100],[1,2,100],[2,0,100],[1,3,600],[2,3,200]], src = 0, dst = 3, k = 1
    //     spdlog::info(sln.findCheapestPrice(4, {{0, 1, 100}, {1, 2, 100}, {2, 0, 100}, {1, 3, 600}, {2, 3, 200}},
    //                                        0, 3, 1));
    //     spdlog::info(sln.findCheapestPrice(3, {{0, 1, 100}, {1, 2, 100}, {0, 2, 500}},
    //                                        0, 2, 1));
    //     spdlog::info(sln.findCheapestPrice(3, {{0, 1, 100}, {1, 2, 100}, {0, 2, 500}},
    //                                        0, 2, 0));
    // }

    // {
    //     st::IterativeMinMax st({2, 6, 7, 5, 18, 86, 54, 2});
    //     cout << st << endl;
    //     cout << st.range_query(2, 7) << endl;

    //     st.update(3, 90);
    //     cout << st.range_query(2, 6) << endl;
    // }

    // {
    //     MaxSubarraySum::MaxSubarraySum st({1, 3, -4, 5, -2});
    //     cout << st.maxSubarraySum(0, 2) << endl;
    // }

    // {
    //     st::LIS lis({5, 1, 3, 9});
    //     cout << lis.MaxLIS() << endl;
    // }

    // {
    //     st::NumLISOccurance lis({1, 3, 5, 4, 7});
    //     cout << lis.MaxLISOccurance() << endl;
    // }

    // {
    //     st::KSubarrayMax lis({18, 5, 10, 7, 9, 4, 15,
    //                           12, 90, 13},
    //                          4);
    //     lis.run();
    // }

    // {
    //     vector<st::PriceRangeQuery::InputProp> input({{24, 6}, {30, 8}, {21, 7}});
    //     st::PriceRangeQuery st(input);

    //     vector<std::pair<int, int>> q(
    //         {{10, 24}, {20, 30}});
    //     st.runQueries(q);
    // }
}