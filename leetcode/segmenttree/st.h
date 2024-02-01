#include <vector>
#include <iostream>
#include <format>

using namespace std;

// array-based

namespace st
{
    class SegmentTree
    {
    public:
        SegmentTree(const vector<int> &&input, int l, int r) : _input(input), _l(l), _r(r)
        {
            cout <<  format("ctor: [{} - {}]", l, r) << endl;
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
                SegmentTree left(std::forward<const vector<int> &&>(input), l, mid);
                auto leftSum = left._sum;
                SegmentTree right(std::forward<const vector<int> &&>(input), mid + 1, r);
                auto rightSum = right._sum;
                _sum = leftSum + rightSum;
            }
            cout <<  format("sum: [{} - {}]: {}", l, r, _sum) << endl;
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

    private:
        const vector<int> &_input;
        int _l;
        int _r;
        int _sum{0};
    };

    inline ostream &operator<<(ostream &os, const SegmentTree &st)
    {
        cout << format("[{} - {}]: {}", st.left(), st.right(), st.sum());
        return os;
    }
}