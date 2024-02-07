#pragma once

#include <vector>
#include <string>
#include <memory>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <iostream>

using namespace std;

// twist in the insert

namespace trie_validwords
{
    class Trie
    {
    public:
        constexpr static size_t ALPHABET_SIZE = 26;
        // RAII
        struct Node
        {
            // key is offset
            unordered_map<int, unique_ptr<Node>> children;
            bool isLeaf;

            Node() : isLeaf{false}
            {
            }
        };

        Trie(const vector<string> &wordDict, vector<char> &allowedChars) : _root(make_unique<Node>())
        {
            // vector to unordered_set
            unordered_set<char> cache;
            // adaptor iterator
            // copy(allowedChars.begin(), allowedChars.end(), std::back_inserter(cache));
            //copy(allowedChars.begin(), allowedChars.end(), inserter(cache));
            cache.insert(allowedChars.begin(), allowedChars.end());
            for (const auto &w : wordDict)
            {
                insertWord(w, cache);
            }
        }

        inline const auto& validWords() const{
            return _validWords;
        } 

        void insertWord(const string &w, const unordered_set<char> &allowedChars)
        {
            auto current = _root.get();
            // iterate string
            for (size_t i = 0; i < w.size(); ++i)
            {
                if(allowedChars.find(w[i]) == allowedChars.end()) {
                    return;
                }

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
                current = current->children[offset].get();
            }
            current->isLeaf = true;
            _validWords.emplace_back(w);
        }

    private:
        unique_ptr<Node> _root;
        vector<string> _validWords;
    };

    class Solution
    {
    public:
        void printAllValidWords(const vector<string> &wordDict, vector<char>& allowed)
        {
            // step 1, build the trie out of wordDict
            Trie trie(wordDict, allowed);
            for(const auto w : trie.validWords()){
                std::cout << w << endl;
            }
        }
    };

};