#include <string>
#include <queue>
#include <unordered_map>

using namespace std;

// Example 1:

// Input: s = "leetcode"
// Output: 0
// Example 2:

// Input: s = "loveleetcode"
// Output: 2
// Example 3:

// Input: s = "aabb"
// Output: -1

namespace leetcode_739
{
    class Solution
    {
    public:
        int firstUniqChar(string s)
        {
            // 26 char, save memory
            unordered_map<u_int8_t, uint> cache;

            for(const auto &c : s){
                cache[c]++;
            }

            for(int i = 0; i < s.size(); ++i){
                if(cache[s[i]] == 1){
                    return i;
                }
            }
            return -1
        }
    };
}