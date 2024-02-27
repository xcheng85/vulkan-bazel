# I/O in cpp

## Concept: Streams
interface: template class

standard stream object:
<iostream>
1. cout
2. cin
3. cerr
4. clog

string i/o
<sstream>

1. istringstream

2. ostringstream

3. stringstream

app | ate | in | out 

file i/o
<fstream>
RAII

1. ifstream
2. ifstream
3. fstream

fail()
is_open()

in | out | ate | app | trunc | binary
tell
seek

>> in 
<< out 

## Manipulator

## Extend to create new stream
1. composition

2. inheritance

## Linux kernel async i/o
AIO

boost.asio: need linking to boost lib

asio: header only

https://registry.bazel.build/modules/asio

bazel workspace + bazel mod


## asio

### Asio feature exprimented
1. Sync + timer 
2. Async + timer + single thread (node.js style) (functor, lamda, class member)
3. Async + multiple thread + synchronization through strand ( serialised function invocation for any executor type ), thread without locking

head-only library
thousands of concurrent connections.
scatter-gather I/O
BSD socket API

support both sync and async

supported async based on:
1. cb
2. future
3. fibers
4. coroutine

## Design pattern (Concurrency without threading)
Proactor pattern implemented as Reactor:

Rely on kernel os call: epoll, select, kqueue
List of function pointers




## async agents: chained async operation , pipelined

## each async operation have customization: Associated Characteristics and Associators

## async -> function pointer as cb
1. function pointer
2. OS indicate async operation complete and enqueue (like future status ready)
3. caller to retrieve the result (like future.get()), blocking call

##  Promise.all in asio ? 
some asynchronous operations may be implemented in terms of multiple child agents that run concurrently.

## agent owns operation, operation use executor and allocator (for memory)
## agent owns cancellation slot and operaton can register
The asynchronous operations within an asynchronous agent use the agent's associated executor 

## completion token
1. lambda
2. future
3. await couroutin
4.  yield_context

### Examples of I/O Object
TCP socket:

### Lock-free multiple threads:
strands 

### L4 support
TCP Clients
TCP Server
UDP
ICMP

### Linux os signals

## UNIX Domain Sockets ? 

## fork() system call.

### How to handle long runing i/o operation without using asio ? 
thread and lock, conditional_var, future for communication



Asio provides the tools to manage these long running operations, without requiring programs to use concurrency models based on threads and explicit locking.