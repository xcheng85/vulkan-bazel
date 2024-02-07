# All fundamentals

## c++ language specific
### Iterator Traits (useful for generic c++ library development)
1. metadata about iterators
    
    a. get the underlying type of iterator

    b. type of iterator: bydirectional, read/write, etc. 

    c.

2. class template
3. class iterator_traits<T> in header <iterator>




## Design Pattern
### Mediator
1. iterator to the container and algorithm, in c++, tmplate class


## DP (optimize recursion algorithm)

avoid duplicate sub-problem calculation

memory storage as a cost

1. top-down 
more memory footprint

2. bottom-up 
start with base case
less memory footprint depends on the situation

2 int instead of o(n)

### C++ implementation
uncached value: 0 (not robust) and std::optional<int>

## greedy

## dfs

## bfs

## binary search
o(n) to o(logn)

## matrix

## Priority Queue with Heap
in c++, adaptor
used in algorithms to improve their efficiencies, such as Dijkstra’s algorithm for finding the shortest path in a graph and the A* search algorithm for pathfinding.

ADT, array, linked list and heap, bst

<queue>

emplace: is variadic
push: not

default: max heap in c++
 priority_queue<int, vector<int>, greater<int> > 

minheap
 priority_queue<int, vector<int>, greater<int> > 

customize with lamda

template with concept for pq

## Sort
### bubble sort
### insert sort
### merge sort: divide and conquer, in place or not in placle

golang: [0 : middle]
c++: 
method1: vector ref + range index "[)" or iterator, 
method2: just pass arround iterator, cbegin(), cend(), arithmatic on iterator

### quick sort vs merge sort
1. both divide and conquer
2. quick sort's split heavy but aggregate simpler, mergesort vice versa


## Prefix sum

## Simulation

## Graph 
### Disjoint set  [x] 
### minimum span tree [x] 
### Union find [x] 
### Topology sort [x]
### Shortest path [x]

## Sliding window

## Backtracking

## Trie [x]

prefix-based searching

any number of characters including alphabets, numbers, and special characters
Each path from the root to any node represents a word or string.
Each node of a Trie represents a string and each edge represents a character.

## BST

## Segment Tree [x]

## Memorization

## Binary Index Tree

## Geometry

## Hash function

## Data stream

## Bucket sort

## Suffix Array

## Counting sort

## Radix sort