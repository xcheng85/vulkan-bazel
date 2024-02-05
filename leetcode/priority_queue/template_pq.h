#include <string>
#include <queue>
#include <unordered_map>

using namespace std;

namespace pq_template
{
    class Solution
    {
    public:
        // three requirements .empty, .top, .pop
        template <typename T> requires requires(T x){x.empty(); x.top(); x.pop();}
        void printPriorityQueue(T &q) 
        {
            while (q.empty() == false)
            {
                cout << q.top() << " ";
                q.pop();
            }
            cout << endl;
        }
    };
}