# System Programming

## Linux

## kernal
dynamic load and unload

### user and kernal space

### system library

### process and thread

## modern thread lib 

### old: pthread

### std::thread
header: <thread>

4 ways: 
global function
functor (class instance with operator())
lamda
member function of a class instance

std::ref

detach mode: daemon thread (remember the pod/node monitoring in the golang app)
join mode: explicity set

### jthread vs thread ? 
1. jthread: thread + raii (join in the dtor) avoid crash
2. cancelling
3. implicit join

### canceling thread

### retrieve result from thread
1. functor
2. future

### future values in thread
1. retrieve result
2. convenience of exception handling

### what is the counterpart of goroutine channel in c++ multi-threading lib ? 

golang bi-direction channel: 

promise: write

future: read

promise + future pair (one-time communication though)

future: template class

### std::async
1. std::promise
2. std::future
3. reference in lamda
4. communication pattern (vs golang goroutine, channels waitgroup, select)
   producer-consumer 

std::async: task, high level
pthread
multi-threading, 
memory management 
networking and storage 
performance and scale

data structures, algorithms, and distributed systems.

multi-threading, concurrency