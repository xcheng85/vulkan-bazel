# C++ Cross-platform

## Concept
modular
Dependency inversion
platform-independent library
Build system
Testing tools

Conditional compilation: 
1. preprocessor directives
2. constexpr if


## Bazel config
1. config .bazelrc

2. Run bazel build command
```shell
# linux
bazel build --compilation_mode=dbg --config=linux //cpp-cross-platform
bazel build --compilation_mode=dbg --config=windows //cpp-cross-platform
```
