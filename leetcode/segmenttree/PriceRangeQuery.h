#include <vector>
#include <iostream>
#include <format>
#include <numeric>
#include <algorithm>

using namespace std;

// build on top of max st
// extra step is sorting the input

namespace st
{
    class PriceRangeQuery
    {
    public:
        struct InputProp
        {
            int price;
            int weight;
        };

        PriceRangeQuery(vector<InputProp> &input) : _sortedInputByPrice(input),
                                                    _st(2 * input.size())
        {
            // step 1: sorting the input
            // increasing less
            sort(begin(_sortedInputByPrice), end(_sortedInputByPrice), [](InputProp a, InputProp b)
                 { return a.price < b.price; });
            // using 0-index
            buildRecusiveUtils(0, _sortedInputByPrice.size() - 1, 0);

            for (size_t i = 0; i < _st.size(); ++i)
            {
                cout << format("st node idx: {}, max: {}\n", i, _st[i]);
            }
        }

        auto runQueries(vector<pair<int, int>> &queries)
        {
            vector<int> result(queries.size());
            for (size_t i = 0; i < queries.size(); i++)
            {
                auto qs = queries[i].first;
                auto qe = queries[i].second;
                // always start from root: 0
                result[i] = maxWeightRangeQueryRecursiveUtils(0, _sortedInputByPrice.size() - 1,
                                                              qs, qe, 0);
                cout << result[i] << "\n";
            }

            return result;
            // return transform(
            //     begin(queries), end(queries),
            //     begin(result), end(result),
            //     [this](InputProp p){
            //     return this.maxWeightRangeQueryRecursiveUtils();
            // });
        }

    protected:
        void buildRecusiveUtils(int ss, int se, int stNodePos)
        {
            if (ss == se)
            {
                _st[stNodePos] = _sortedInputByPrice[ss].weight;
                return;
            }
            if (ss > se)
            {
                return;
            }
            auto sm = (ss + se) / 2;
            buildRecusiveUtils(ss, sm, 2 * stNodePos + 1);
            buildRecusiveUtils(sm + 1, se, 2 * stNodePos + 2);
            _st[stNodePos] = max({_st[2 * stNodePos + 1], _st[2 * stNodePos + 2]});
        }

        // another tricky is the qs and qe is from the price
        //
        int maxWeightRangeQueryRecursiveUtils(int ss, int se, int qs, int qe, int stNodePos)
        {
            auto ssPrice = _sortedInputByPrice[ss].price;
            auto sePrice = _sortedInputByPrice[se].price;

            if (qs > sePrice || qe < ssPrice)
            {
                return numeric_limits<int>::min();
            }

            if (qs <= ssPrice && qe >= sePrice)
            {
                return _st[stNodePos];
            }

            auto sm = (ss + se) / 2;
            auto maxLeft = maxWeightRangeQueryRecursiveUtils(ss, sm, qs, qe, 2 * stNodePos + 1);
            auto maxRight = maxWeightRangeQueryRecursiveUtils(sm + 1, se, qs, qe, 2 * stNodePos + 2);

            return max({maxLeft, maxRight});
        }

    private:
        vector<InputProp> &_sortedInputByPrice;
        // max st storing weight
        vector<int> _st;
    };
}