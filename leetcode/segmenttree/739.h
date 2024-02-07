#include <vector>
#include <iostream>
#include <format>
#include <memory>
#include <unordered_map>

using namespace std;

// max query

// look up from nodeIndex to node,

// each node need to record the node index in the input vector

namespace leetcode_739
{
    struct NodeMaxProp
    {
        int value;
        int index;
    };

    // max segment tree
    //

    class Node
    {
    public:
        Node(vector<int> &temperatures,
             unordered_map<int, int> &indexLookup,
             unordered_map<int, Node *> &nodeLookup,
             int l, int r, int current, int parent)
            : _parent{parent}
        {
            if (l == r)
            {
                //_max = tuple<int, int>{temperatures[l], l};
                _max = NodeMaxProp{temperatures[l], l};
                indexLookup[l] = current;
            }
            else
            {
                // create two nodes
                auto mid = (l + r) / 2;
                _left = make_unique<Node>(temperatures, indexLookup, nodeLookup, l, mid, 2 * current + 1, current);
                _right = make_unique<Node>(temperatures, indexLookup, nodeLookup, mid + 1, r, 2 * current + 2, current);
                
                nodeLookup[2 * current + 1] = _left.get();
                nodeLookup[2 * current + 2] = _right.get();

                // structural binding
                const auto &[lvalue, lindex] = _left->max();
                const auto &[rvalue, rindex] = _right->max();
                if (rvalue > lvalue)
                {
                    _max = NodeMaxProp{rvalue, rindex};
                }
                else
                {
                    _max = NodeMaxProp{lvalue, lindex};
                }
            }
        }

        inline const NodeMaxProp &max() const
        {
            return _max;
        }
        inline auto parent() const {
            return _parent;
        }

        int findNextLargerNodeIndex(const unordered_map<int, Node *>& nodeLookup)
        {
            auto p = _parent;
            while (p != -1)
            {
                auto pNode = nodeLookup.at(p);
                const auto [value, index] = pNode->max();
                if (_max.value == value){
                    p = pNode->parent();
                } else {
                    return index - _max.index;
                }
            }
        }

    private:
        int _parent;
        bool _isLeaf;
        // [value, index in input]
        // tuple<int, int> _max;
        NodeMaxProp _max;
        std::unique_ptr<Node> _left;
        std::unique_ptr<Node> _right;
    };

    // temperatures = [73,74,75,71,69,72,76,73]
    class Solution
    {
    public:
        vector<int> dailyTemperatures(vector<int> &&temperatures)
        {
            unordered_map<int, Node *> nodeLookup;
            unordered_map<int, int> indexLookup;
            auto root = make_unique<Node>(temperatures, indexLookup, nodeLookup, 0, temperatures.size() - 1, 0, -1);
            nodeLookup[0] = root.get();

            for(const auto& [k, v] : nodeLookup){
               cout << format("{}:{}\n", k, v->max().value);
            }
            for (int i = 0; i < temperatures.size(); ++i)
            {
                auto treeId = indexLookup[i];
                auto node = nodeLookup[treeId];

                cout << node->findNextLargerNodeIndex(nodeLookup) << endl;
            }

            return {};
        }
    };

}