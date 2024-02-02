#include <vector>
#include <iostream>
#include <format>
#include <numeric>

using namespace std;

namespace st
{

    // Complexity Analysis:

    // Time Complexity: (N * log N)
    // Auxiliary Space: O(N)
    struct Prop
    {
        int min;
        int max;
    };

    class IterativeMinMax
    {
    public:
        // n + n - 1
        // 2 * n to cover both odd and even
        IterativeMinMax(const vector<int> &&input) : _tree(2 * input.size()), _input(std::move(input))
        {
            // perfect forwarding
            build(std::forward<decltype(input)>(input));
        }

        // update is simpler
        void update(int pos, int value)
        {
            pos += _input.size();

            // update leaf node
            _tree[pos] = {value, value};

            // update internal node
            while (pos > 1)
            {
                // /2
                pos >>= 1;
                _tree[pos].max = max(_tree[2 * pos].max, _tree[2 * pos + 1].max);
                _tree[pos].min = min(_tree[2 * pos].min, _tree[2 * pos + 1].min);
            }
        }

        // query method
        Prop range_query(int left, int right)
        {
            // tricky part
            // four cases
            // 1. even odd: good
            // 2. even even: right adj
            // 3. odd even: both adj
            // 4. odd odd: left adj

            left += _input.size();
            right += _input.size();

            int maxValue = std::numeric_limits<int>::min();
            int minValue = std::numeric_limits<int>::max();

            while (left < right)
            {
                if (left % 2 == 1)
                {
                    maxValue = max(maxValue, _tree[left].max);
                    minValue = min(minValue, _tree[left].min);
                    left++;
                }
                if (right % 2 == 0)
                {
                    maxValue = max(maxValue, _tree[right].max);
                    minValue = min(minValue, _tree[right].min);
                    right--;
                }
                left /= 2;
                right /= 2;
            }
            // last node to compare
            if (left == right)
            {
                maxValue = max(maxValue, _tree[left].max);
                minValue = min(minValue, _tree[left].min);
            }

            return {
                minValue,
                maxValue};
        }

        // allow it to access private member
        friend ostream &
        operator<<(ostream &os, const IterativeMinMax &st);

    protected:
        // for subclass to tweak behavior
        virtual void build(const vector<int> &&input)
        {
            const auto n = input.size();
            // leaf
            for (int i = 0; i < n; i++)
            {
                _tree[n + i] = {input[i], input[i]};
            }

            // internal nodes
            // this trick could cover both even and odd number of inputs
            for (int i = n - 1; i >= 1; i--)
            {
                _tree[i].min = max(_tree[2 * i].min, _tree[2 * i + 1].min);
                _tree[i].max = max(_tree[2 * i].max, _tree[2 * i + 1].max);
            }
        }

    private:
        vector<Prop> _tree;
        const vector<int> &&_input;
    };

    inline ostream &operator<<(ostream &os, const IterativeMinMax &st)
    {
        for (int i = 1; i < st._tree.size(); ++i)
        {
            cout << format("{}: min: {}, max: {}", i, st._tree[i].min, st._tree[i].max) << endl;
        }
        return os;
    }

    inline ostream &operator<<(ostream &os, const Prop &st)
    {
        cout << format("Min {}, Max {}", st.min, st.max) << endl;
        return os;
    }
}