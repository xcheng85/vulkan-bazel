# gprof + bazel

## Update on build command
```shell
bazel build — copt=”-pg” — cxxopt=”-pg” — linkopt=“-pg”
bazel build --copt='-pg' --cxxopt='-pg'  --linkopt='-pg' //cpp-generic-object-pool

```
