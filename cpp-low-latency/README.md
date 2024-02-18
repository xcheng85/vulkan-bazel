# Low latency system with c++

## threads on linux
thread affinity (pthread)

## Memory pool

### overload operator new and operator delete

new class


### placement new operator (key to memory pool)
invoke constructor on the allocated piece of memory

## Lock-free queue (communication between threads)

### pattern: one write and one read

atomic: 

mutex, lock: heavy , 


## Logger multi-thread logger

mainthread --> logger thread (lock-free queue for communication)

separate thread to log to files: i/o slow, string format slow

## tcp/ip network programming (socket programming)

### concept

1. network interface: 
point of connection between a computer and a network.

list all ni
```shell
ls /sys/class/net
ip link
ip addr
```

2. Hardware timestamping and software timestamping
Hardware support from NIC

Exchange gateway connectivity (ideally with options exchanges).
Networking protocols familiarity (tcp/ip/udp, multicast, sequencer architecture, kernel bypass).

matching engine development.
Familiar with high performance computing & memory management.

Software Engineer, Backend, 7+ Years of Experience
Snap Inc.

Experience using various profling tools (e.g., gdb, Nsight, Valgrind, fame graph) to optimize the code.

Software Engineer, Robot Cloud Platform