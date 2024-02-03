#include <vector>
#include <iostream>
#include <format>
#include <memory>

using namespace std;

// array-based

// build

// update a leaf node
// update parent all the way to the root

// if you use node-based approach, you need to have parent pointers

namespace st
{
    class SegmentTreeNode
    {
    public:
        SegmentTreeNode(const vector<int> &input, int l, int r, int nodeIndex, optional<int> parentNodeIndex)
            : _input(input), _l(l), _r(r), _nodeIndex{nodeIndex}, _parent{parentNodeIndex}
        {
            cout << format("ctor: [{} - {}]", l, r) << endl;
            if (l == r)
            {
                // due to const
                _sum = input.at(l);
            }
            else
            {
                // l + r could overflow
                // auto mid = (l + r) / 2;
                auto mid = l + (r - l) / 2;
                SegmentTreeNode left(std::forward<const vector<int> &&>(input), l, mid, 2 * _nodeIndex + 1, nodeIndex);
                auto leftSum = left._sum;
                SegmentTreeNode right(std::forward<const vector<int> &&>(input), mid + 1, r, 2 * _nodeIndex + 2, nodeIndex);
                auto rightSum = right._sum;
                _sum = leftSum + rightSum;
            }
            cout << format("sum: [{} - {}]: {}", l, r, _sum) << endl;
        }

        inline auto left() const &
        {
            return _l;
        }
        inline auto right() const &
        {
            return _r;
        }
        inline auto sum() const &
        {
            return _sum;
        }
        inline auto parent() const &
        {
            // set value for | undefined
            return _parent.value_or(-1);
        }

    private:
        const vector<int> &_input;
        std::optional<int> _parent;
        int _nodeIndex;
        int _l;
        int _r;
        int _sum{0};
    };

    class SegmentTree
    {
    public:
        SegmentTree(const vector<int> &input) : _root(make_unique<SegmentTreeNode>(input, 0, input.size() - 1, 0, nullopt))
        {
        }
        inline const auto root() const
        {
            return _root.get();
        }

    private:
        unique_ptr<SegmentTreeNode> _root;
    };

    inline ostream &operator<<(ostream &os, const SegmentTree &st)
    {
        cout << format("[{} - {}]: {}, parent: {}", st.root()->left(), st.root()->right(), st.root()->sum(), st.root()->parent());
        return os;
    }

    inline ostream &operator<<(ostream &os, const SegmentTree *st)
    {
        cout << format("[{} - {}]: {}, parent: {}", st->root()->left(), st->root()->right(),
         st->root()->sum(), st->root()->parent());
        return os;
    }

}