# Lamda 

## concept
1. lamda: compile-time
2. closure: runtime
3. closure class: compile-time

## application

1. stl algorithm
2. smart pointers custom deleter
3. multi-threading
4. callback
5. interface adaption function
6. context-specific functions

## capture mode
1. default by ref: [&] dangling 
2. explicit by ref: [&var] dangling
3. default by value: [=] dangling when pointer is used
4. explict by value: [var]
5. cannot capture static var and avoid them being changed outside the scope

## Move objects + Closure
### init capture

## generic lamda with perfect forwarding
1. inspect the lvaue or rvalue
    decltype(x)
2. decltype(auto)'s power