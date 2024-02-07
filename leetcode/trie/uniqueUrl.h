#pragma once

#include <vector>
#include <string>
#include <memory>
#include <optional>
#include <unordered_map>

using namespace std;

// tries + list

// node in tries: pointer

// random delete
// order container: deque, map with key

namespace uniqueUrl
{
    class Trie
    {
    public:
        struct Node
        {
            // key is offset
            unordered_map<int, unique_ptr<Node>> children;
            bool isLeaf;

            Node() : isLeaf{false}
            {
            }
        };

        Trie(vector<string> &wordDict) : _root(make_unique<Node>())
        {
            for (const auto &w : wordDict)
            {
                insertWord(w);
            }
        }

        void insertWord(const string &w)
        {
            auto current = _root.get();
            // iterate string
            for (size_t i = 0; i < w.size(); ++i)
            {
                const auto offset = w[i] - 'a';
                // replacle nullptr check
                // if (!current->children[offset].has_value())
                // operator[] is not const
                // count or containers, const
                if (!current->children.contains(offset))
                {
                    auto n = std::make_unique<Node>();
                    current->children.emplace(offset, std::move(n));
                }
                if (i == w.size() - 1)
                {
                    cout << "isLeaft hit" << endl;
                    current->isLeaf = true;
                }
                current = current->children[offset].get();
            }
        }

    private:
        unique_ptr<Node> _root;
    };

    class Solution
    {
    public:
        bool wordBreak(string s, vector<string> &wordDict)
        {
            // step 1, build the trie out of wordDict
            Trie trie(wordDict);

            // step 2: search the trie using recursive, divide and conq
            return wordBreakUtils(trie, s);
        }

    protected:
        bool wordBreakUtils(const Trie &trie, string s)
        {
            // Base case: key
            if (s.size() == 0)
                return true;

            cout << "wordBreakUtils: " << s << endl;
            for (int i = 1; i <= s.size(); i++)
            {
                if (trie.searchWord(s.substr(0, i)) && wordBreakUtils(trie, s.substr(i, s.size() - i)))
                {
                    cout << "done" << endl;
                    return true;
                }
            }
            return false;
        }
    };

};