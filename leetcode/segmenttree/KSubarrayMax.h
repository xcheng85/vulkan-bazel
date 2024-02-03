#include <vector>
#include <iostream>
#include <format>
#include <numeric>
#include <algorithm>

using namespace std;

// build on top of max st

namespace st
{
    class KSubarrayMax
    {
    public:
        // only need max value
        struct Prop
        {
            int max;
        };

        KSubarrayMax(const vector<int> &input, int k) : _k{k}, _input(input), _st(2 * input.size())
        {
            build(input);
        }

        void run()
        {
            auto n = _input.size();
            for (int i = 0; i < n; i++)
            {
                // check boundary
                if ((_k - 1 + i) < n)
                {
                    cout << maxRangeQuery(0, 0, n - 1, i, _k - 1 + i) << "\n";
                }
                else
                {
                    break;
                }
            }
        }

    protected:
        void build(const vector<int> &input)
        {
            // starting from root and using 0-index approach
            buildDFS(0, input.size() - 1, 0);

            for (size_t i = 0; i < _st.size(); ++i)
            {
                cout << format("st node idx: {}, max: {}\n", i, _st[i].max);
            }
        }
        // nodePos: current node in st to write
        // ss and se are the searching range
        // The relationship between nodePos and search range
        // dfs
        void buildDFS(int ss, int se, int nodePosToWrite)
        {
            cout << format("buildDFS: {} - {}, {}\n", ss, se, nodePosToWrite);
            if (ss == se)
            {
                // leaf node
                _st[nodePosToWrite] = {_input[ss]};
            }
            else
            {
                auto sm = (ss + se) / 2;
                // 0-based
                buildDFS(ss, sm, 2 * nodePosToWrite + 1);
                buildDFS(sm + 1, se, 2 * nodePosToWrite + 2);
                _st[nodePosToWrite] = {max({_st[2 * nodePosToWrite + 1].max,
                                            _st[2 * nodePosToWrite + 2].max})};
            }
        }

        // max: range query + recursive
        // nodePos in st
        // low-high: divide and conquer full range, search range
        // start-end: query range
        // DFS
        int maxRangeQuery(int nodePos, int ss, int se, int qs, int qe)
        {
            // query is larger than this node covered
            if (qs <= ss && qe >= se)
                return _st[nodePos].max;

            // query range outside
            if (qs > se || qe < ss)
                return numeric_limits<int>::min();

            // divide search range
            int sm = (ss + se) / 2;

            return max({maxRangeQuery(2 * nodePos + 1, ss, sm, qs, qe),
                        maxRangeQuery(2 * nodePos + 2, sm + 1, se, qs, qe)});
        }

    private:
        int _k;
        const vector<int>& _input;
        vector<Prop> _st;
    };
}