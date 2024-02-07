#pragma once
#include <assert.h>
#include <vector>
#include <queue>
#include <memory>
#include <format>
#include <stack>
#include <iostream>
#include <optional>
#include <utility>

namespace sum_k_smallest
{
    using namespace std;
    // default value for template
    template <typename T = int>
    class BST_Iterative
    {
    public:
        struct Node
        {
            T value;
            unique_ptr<Node> left;
            unique_ptr<Node> right;

            ~Node() {
                cout << "dtor\n";
            }
        };
        BST_Iterative()
        {
        }

        void Insert(const initializer_list<T> &values)
        {
            for (const auto &v : values)
            {
                InsertValue(v);
            }
        }

        // iterative search
        pair<optional<Node *>, optional<Node *>> Search(T value)
        {
            Node *curr = _root.get();
            Node *parent = nullptr;
            while (curr)
            {
                auto v = curr->value;

                if (value < v)
                {
                    if (!curr->left)
                    {
                        return {{}, {}};
                    }
                    parent = curr;
                    curr = curr->left.get();
                }

                if (value > v)
                {
                    if (!curr->right)
                    {
                        return {{}, {}};
                    }
                    parent = curr;
                    curr = curr->right.get();
                }

                if (value == v)
                {
                    if (!parent)
                    {
                        return {curr, {}};
                    }
                    return {curr, parent};
                }
            }
        }

        // increasing sorted
        // every traversal is a flow step, each down or up
        // record previous in the flow
        vector<T> InorderTraversal() const
        {
            vector<T> results;
            stack<Node *> s;
            if (_root)
            {
                s.push(_root.get());
            }

            Node *previous = _root.get();
            while (s.size() > 0)
            {
                Node *curr = s.top();
                // up flow
                if (previous == nullptr)
                {
                    s.pop();
                    results.emplace_back(curr->value);
                    if (curr->right)
                    {
                        s.push(curr->right.get());
                        previous = nullptr; // this node is poped
                    }
                }
                else
                {
                    // down flow
                    if (curr->left)
                    {
                        s.push(curr->left.get());
                        previous = curr;
                    }
                    else
                    {
                        // reach the bottom
                        results.emplace_back(curr->value);
                        s.pop();
                        previous = nullptr;
                    }
                }
            }
            // compiler optm
            return results;
        }

        // // 1. leaf node
        // root may change
        void deleteNode(T value)
        {
            // find the node
            pair<optional<Node *>, optional<Node *>> p = Search(value);
            // optional<Node *> node = p.first;
            // optional<Node *> parent = p.parent;
            assert(p.first.has_value());
            Node* node = p.first.value();
            // parent == nullptr, root
            Node* parent = nullptr;
            if(p.second.has_value()) {
                parent = p.second.value();
            }
            // leaf node
            if (!node->left && !node->right)
            {
                if(parent) {
                    if(parent->left.get() == node) {
                        parent->left.reset(nullptr); 
                    }
                    else if (parent->right.get() == node) {
                        parent->right.reset(nullptr); 
                    } else {
                        assert(false);
                    }
                }
            }
        }

    protected:
        void InsertValue(T value)
        {
            cout << format("InsertValue: {}\n", value);
            // first node
            Node *node = new Node(value);
            if (!_root)
            {
                _root = std::move(unique_ptr<Node>(node));
                return;
            }
            Node *prev = nullptr;
            Node *curr = _root.get();
            while (curr)
            {
                // left
                if (curr->value > value)
                {
                    prev = curr;
                    curr = curr->left.get();
                }
                else if (curr->value < value)
                {
                    prev = curr;
                    curr = curr->right.get();
                }
            }
            // delegate the management to smart poitner: raii
            if (prev->value > value)
                prev->left = std::move(unique_ptr<Node>(node));
            else
                prev->right = std::move(unique_ptr<Node>(node));
        }

    private:
        unique_ptr<Node> _root;
    };

    template <typename T = int>
    ostream &operator<<(ostream &os, const BST_Iterative<T> &bst)
    {
        const auto &inorder = bst.InorderTraversal();
        for (const auto &v : inorder)
        {
            os << v << "\n";
        }
        return os;
    }
}