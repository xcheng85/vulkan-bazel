# Cuda

## Cuda + bazel

```shell
bazel build --@rules_cuda//cuda:compiler=nvcc --@rules_cuda//cuda:copts=-allow-unsupported-compiler --sandbox_debug //cpp-cuda-basic

nvcc main.cu -o main -allow-unsupported-compiler

bazel build --@rules_cuda//cuda:archs=compute_61:compute_61,sm_61

```
