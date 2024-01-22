# Generic Object Pool

## Application

1. Bricks in out-of-core volume rendering

## module


## Generic with c++ templates


## memory allocator
same as all containers in stl

## Memory allocation and deallocation operators

### placement new operator
allocate new object on pre-allocated memory

new(ptr) class 

## Profiling
```shell
bazel run --copt='-pg' --cxxopt='-pg'  --linkopt='-pg' --compilation_mode=dbg //cpp-generic-object-pool

cd ./bazel-bin/cpp-generic-object-pool

# create gmon.out
./cpp-generic-object-pool 

gprof cpp-generic-object-pool gmon.out > gprof_analysis.out
```
