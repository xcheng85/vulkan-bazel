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
4. member function of a class instance

## Join vs Detach
Join: wait group in go 

## functor arguments
argument passed to thread are copy, std::ref to avoid copy

lvalue must be wrapped std::ref()

## When need to consider synchorinzation
mutable shared data members

## Pre-requisite of Atomic Template
### Trivially copyable Type
1. Trivially copyable class

## Atomic Template vs Mutex vs Lock
1. Atomic: no explicit synchronization code
2. Mutex: explicit 
3. Lock: RAII idom + Mutex

## Mutex and Lock
Lock is the RAII idiom of mutex: release mutex in the dtor
Lock: avoid deadlock

all the locks are using templates

1. lock_guard: for write, exclusive access
2. unique_lock:
3. shared_lock: multiple threads read
4. scoped_lock: advanced version of lock_guard: variadic mutexes, exclusive access

## Mutiple thread read and few thread write case
shared_timed_mutex + shared_lock

## how to retrieve result in thread-based api ? 
1. functor + class member + class get api
2. future

## handling exception in mult-threading env ? 
1. each thread must handle its own exception
2. main thread pass in a exception_ptr as reference
3. main thread rethrow exception

## Singeltion in multi-threading envrionment
std::call_once + std::once_flag

## lock multiple locks in one call ? 
1. std::lock
2. c++17: scoped_lock