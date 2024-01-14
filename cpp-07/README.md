# Concurrent C++

## Concept
1. Thread-based
Deal with thread exhaustion, oversubscription, load balancing

no feedback

access to raw pthread handle
2. Task-based
c++ standard library 

feedback for result or exception

like async await in typescript

no access to raw handle in os level

3. Software thread vs Hardware thread

hardware thread: cpu core

software thread: goroutine

slice

context switch

cache miss

## Thread-based approach
1. lamda: compiler create closure class (functor equivilant)
2. functor
3. inline function

## functor arguments
lvalue must be wrapped std::ref()

## When need to consider synchorinzation
mutable shared data members

## Mutex and Lock
Lock is the RAII idiom of mutex: release mutex in the dtor
Lock: avoid deadlock

all the locks are using templates

1. lock_guard
2. unique_lock
3. shared_lock
4. scoped_lock: advanced version of lock_guard: variadic mutexes