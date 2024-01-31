# Cpp iterator with iterator concept

## constraints in c++

concepts

### concepts in stl
headers: <concepts>

1. derived class constraints: (interface)
2. iterator type constraints: 
3. constraints on function and class (template-based)
4. operator based contraints: 

## type traits (meta about any type) 
type traits + template to generate code based on type on "compile time"

1. compile time
2. <type_traits>
3. existing type_traits to be used
4. use constexpr in the rightplace 

   is_const
   is_object

## type + traits + iterator
iterator_traits
1. header: <iterator>
2. class template: iterator_traits


## Template + recursion 
vector 


## iterator types
1. random access: += , such as: vector
2. Forward: ++, forward_list, 
3. Bidirectional: list --

4. input iterator: read stream
5. output iterator: write stream


## apated iterator type

1. insert iterator: 
2. back insert iterator
3. front insert iterator
4. move iterator: avoid memotry copy



## container reference pattern
store smart pointer 
reference


## key pointer for stl containers
1. all are template class with more than 2 template types
2. all have tempalate types with default values
3. sorted container with std::less template function, means custom class must have operator<
4. better impl: copy ctor, copy assignment and move ctor and move assignemnt with noexcept

### Container: vector
is constexpr
iterate: 
1. range based for loop with const/non-const attr

2. iterator: cbegin() or begin(): 

3. loop + predict: 

std::for_reach

for subrange: such as merge sort, etc. 
for existed stl algorithms

### vector + member function

back()
push_back(): passing rvalue is better, avoid copying, use std::move
pop_back()
emplace_back() // in place memory, variadic params, perfect forwarding

erase() with iterator, two iterators
insert() with iterator of dst and two iterator of src

clear()

### vector + performance 
emplace_back best
push_back + rvalue 2nd
push_back + lvalue: copy occurs

### move or copy vector ? 


### when do you need to make sure vector size be fittable ? 
insert: does not need to. 

copy without adapator iterator: ensure target and src vector size match
copy with adapter iterator: no need 

### global function on vector
erase_if: fp, predict

### vector + reference case (memory dangling issue)
1. header: <functional>
2. std::ref

### Container: list


## Alogorithm on iterator

### fp with predict on the entire container (no iter pairs)

1. erase op

erase_if 

2. remove op  (remove then erase idiom)
partition two groups, and return the iterator where the rest can be erased

### fp with predict + 1 iterator pair
1. find_if and find_if_not 

find: requirs operator == 
find_if_not: only the first elemet not equal

find_first_of;

2. accumulate (like reducer)
needs <numeric>

init value, and lamda

mutable to lamda

for custom class, require operator+ overload

3. all_of

4. any_of

5. none_of

6. count_if

7. generate by predict

math function, x is the discrete value, y is calculated based on match

8. replace with predict: () in place

9. reverse(): how to reverse the array use in-place

10. partition(): in place, group into 2

11. sort(): in place 

12. stable_sort(): in place

### fp with predict + 1.5 iterator pair
Transform like map in javascript

1. transform in-place
2. copy: needs to resize enough space
3. copy_backwards: becareful if the size are different between source and dst container
4. copy_if:
5. move:
6. move_backwards:
7. move_if
8. reverse_copy:
9. nth_element: pivot at nth, requirs operator <

### fp with predict + 2 iterator pair
Search match algorithm
Copy algorithm

1. search: find first subsequence 

2. find_end: find last subsequence

3. equal: subrange comparision of same type (full range use ==, operator== overload is must)
       comparion of different container with iterator as adaptor

4. mismatch: return the iterator 

5. transform: not in place
need to resize enough space >= 

### op with return to be erase itr.
remove_if: in place
remove_copy_if 
unique: in place, sort is a pre-requisite
unqiue_copy

partition_copy: two iterators with two groups, 


### search 

1. binary_search()
sorted, and than divide and conquer

operator < and operator=

pair of iterator for ranges and a target 

2. lowerbound() 
usage find the place to insert to a sorted container


3. upperbound()


### set algorithm

1. how to merge two sorted containers ? 
two pair of iterators
1 iterator as dst

since sorted, two pointers 

### find the min/max in a container
1 pair of iterator

operator < is needed
min_element
max_element
minmax_element 


### paralle + algorithm
1. for_reach() completed independent

2. all_of() divide can be parallel
last step &&

3. copy() copy subrange by each thread

4. count_if() each thread have a local sum
last step to merge

5. find(): find the first
each thread return one iterator if there is any
return the first one

6. replace(): each thread complete independent

7. search(): subrange match, ? 

8. transform(): complete independent

### Numeric Reduce

accumulate vs reduce


1. accumulate: not paralle
need operator+

2. reduce(): paralle
no need for operator+
std::pplus

