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
    // T must support operator<<
    template <typename T = int>
        requires requires(T x) { cout << x; }
    class BST_Iterative
    {
    public:
        struct Node
        {
            T value;
            unique_ptr<Node> left;
            unique_ptr<Node> right;

            ~Node()
            {
                cout << "dtor: " << value << "\n";
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
            Node *node = p.first.value();
            // parent == nullptr, root
            Node *parent = nullptr;
            bool isLeft;
            if (p.second.has_value())
            {
                parent = p.second.value();
                if (parent->left.get() == node)
                {
                    isLeft = true;
                }
                else
                {
                    isLeft = false;
                }
            }
            // case 1:  leaf node
            if (!node->left && !node->right)
            {
                if (parent)
                {
                    isLeft ? parent->left.reset(nullptr) : parent->right.reset(nullptr);
                }
                else
                {
                    // it is the last element in the bst and it is root
                    _root.reset(nullptr);
                }
            }
            else if (!node->left || !node->right)
            {
                auto remain = node->left ? move(node->left) : move(node->right);
                // case2: internal node has one child
                if (parent)
                {
                    if (isLeft)
                    {
                        parent->left = move(remain);
                    }
                    else
                    {
                        parent->right = move(remain);
                    }
                }
                else
                {
                    // update root
                    _root = move(remain);
                }
            }
            else
            {
                // case 3:
                Node *inorderSuccessorParent = node;
                // Find inorder successor of node
                Node *inorderSuccessor = node->right.get();
                while (inorderSuccessor->left != nullptr)
                {
                    inorderSuccessorParent = inorderSuccessor;
                    inorderSuccessor = inorderSuccessor->left.get();
                }
                // there is no left successor,
                // the node->right is the one to replace node.
                if (inorderSuccessorParent != node)
                {
                    // node
                    //     p
                    //   s
                    //     r
                    node->value = inorderSuccessor->value;
                    inorderSuccessorParent->left = std::move(inorderSuccessor->right);
                }
                else
                {
                    // p
                    //   s
                    //     r
                    node->value = inorderSuccessor->value;
                    inorderSuccessorParent->right = std::move(inorderSuccessor->right);
                }
            }
        }

        // bfs
        void bfs()
        {
            // level
            queue<pair<Node *, int>> bfs;
            bfs.push({_root.get(), 0});

            while (!bfs.empty())
            {
                auto &[curr, level] = bfs.front();
                bfs.pop();
                if(curr->left){
                    bfs.push({curr->left.get(), level + 1});
                }
                if(curr->right){
                    bfs.push({curr->right.get(), level + 1});
                }

                cout << format("level: {}, {}\n", level, curr->value);
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