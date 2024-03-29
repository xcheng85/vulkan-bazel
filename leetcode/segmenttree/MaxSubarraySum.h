#include <vector>
#include <iostream>
#include <format>
#include <numeric>
#include <algorithm>

using namespace std;

// dfs to build and query
//

namespace MaxSubarraySum
{
    struct Prop
    {
        int sum{numeric_limits<int>::min()};
        int maxPrefixsum{numeric_limits<int>::min()};
        int maxSuffixsum{numeric_limits<int>::min()};
        int maxSum{numeric_limits<int>::min()};

        // for struct weird
        friend ostream &
        operator<<(ostream &os, Prop &st)
        {
            os << format("Min {}, Max {}", st.sum, st.maxSum) << endl;
            return os;
        }
    };

    class MaxSubarraySum
    {
    public:
        // n + n - 1
        // 2 * n to cover both odd and even
        MaxSubarraySum(const vector<int> &&input) : _tree(2 * input.size()), _input(std::move(input)),
                                                    _treeIterative(2 * input.size())
        {
            // perfect forwarding
            // build(std::forward<decltype(input)>(input));
            build();
            // buildUtils(0, _input.size() - 1, 1);

            // for (int i = 0; i < _tree.size(); i++)
            // {
            //     // cout << i << ": " <<  _tree[i] << "-" << _treeIterative[i] << "\n" ;
            //     cout << i << ":" << _tree[i] << "\n";
            //     cout << i << ":" << _treeIterative[i] << "\n";
            // }
        }

        // query method
        auto maxSubarraySum(int left, int right)
        {
            //
            auto startNodeIndex = 1;
            return queryUtil(0, _input.size() - 1, left, right, startNodeIndex).maxSum;
        }

        // // allow it to access private member
        // friend ostream &
        // operator<<(ostream &os, const MaxSubarraySum &st);

    protected:
        // for subclass to tweak behavior
        virtual void build()
        {
            const auto n = _input.size();
            // leaf
            // wrong for the odd input !
            if (n % 2 == 0)
            {
                for (int i = 0; i < n; i++)
                {
                    _tree[n + i] = {_input[i], _input[i], _input[i], _input[i]};
                }
            }
            else
            {
                for (int i = 2; i < n; i++)
                {
                    _tree[n + i - 2] = {_input[i], _input[i], _input[i], _input[i]};
                }
                _tree[2 * n - 2] = {_input[0], _input[0], _input[0], _input[0]};
                _tree[2 * n - 1] = {_input[1], _input[1], _input[1], _input[1]};
            }

            // internal nodes
            // this trick could cover both even and odd number of inputs
            for (int i = n - 1; i >= 1; i--)
            {
                _tree[i] = merge(_tree[2 * i], _tree[2 * i + 1]);
            }
        }

        // Builds the Segment tree recursively
        // post-traversal DFS
        void buildUtils(int start, int end, int nodeIndex)
        {
            cout << format("{} - {}: {}\n", start, end, nodeIndex);
            /* Leaf Node */
            if (start == end)
            {
                _tree[nodeIndex].sum = _input[start];
                _tree[nodeIndex].maxSuffixsum = _input[start];
                _tree[nodeIndex].maxPrefixsum = _input[start];
                _tree[nodeIndex].maxSum = _input[start];
                return;
            }

            int mid = (start + end) / 2;
            buildUtils(start, mid, 2 * nodeIndex);
            buildUtils(mid + 1, end, 2 * nodeIndex + 1);

            _tree[nodeIndex] = merge(_tree[2 * nodeIndex], _tree[2 * nodeIndex + 1]);
        }

        // Returns Parent Node after merging its left and right child
        Prop merge(Prop leftChild, Prop rightChild)
        {
            Prop parentNode;
            parentNode.maxPrefixsum = max(leftChild.maxPrefixsum,
                                          leftChild.sum +
                                              rightChild.maxPrefixsum);

            parentNode.maxSuffixsum = max(rightChild.maxSuffixsum,
                                          rightChild.sum +
                                              leftChild.maxSuffixsum);

            parentNode.sum = leftChild.sum + rightChild.sum;

            // max with init list
            parentNode.maxSum = max({leftChild.maxSum,
                                     rightChild.maxSum,
                                     leftChild.maxSuffixsum +
                                         rightChild.maxPrefixsum});

            return parentNode;
        }

        /* A Recursive function to get the desired
           Maximum Sum Sub-Array,
        The following are parameters of the function-
        tree     --> Pointer to segment tree
        nodeIndex --> Index of the segment tree Node
        ss & se  --> Starting and ending indexes of the
                     segment represented by
                         current Node, i.e., tree[index]
        qs & qe  --> Starting and ending indexes of query range */
        Prop queryUtil(int ss, int se, int qs,
                       int qe, int nodeIndex)
        {
            // No overlap
            if (ss > qe || se < qs)
            {
                // returns a Node for out of bounds condition
                Prop nullNode;
                return nullNode;
            }

            // Complete overlap
            if (ss >= qs && se <= qe)
            {
                return _tree[nodeIndex];
            }

            // Partial Overlap Merge results of Left
            // and Right subtrees
            int mid = (ss + se) / 2;
            auto left = queryUtil(ss, mid, qs, qe, 2 * nodeIndex);
            auto right = queryUtil(mid + 1, se, qs, qe, 2 * nodeIndex + 1);

            // merge left and right subtree query results
            auto res = merge(left, right);
            return res;
        }

    private:
        vector<Prop> _tree;
        vector<Prop> _treeIterative;
        const vector<int> &&_input;
    };

    // inline ostream &operator<<(ostream &os, const IterativeMinMax &st)
    // {
    //     for (int i = 1; i < st._tree.size(); ++i)
    //     {
    //         cout << format("{}: min: {}, max: {}", i, st._tree[i].min, st._tree[i].max) << endl;
    //     }
    //     return os;
    // }

    // inline ostream &operator<<(ostream &os, Prop st)
    // {
    //     os << format("Min {}, Max {}", st.sum, st.maxSum) << endl;
    //     return os;
    // }
}