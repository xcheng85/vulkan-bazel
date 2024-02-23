# Unit test with gtest

## Bazel config

```shellk
```

## Run unit test

```shell
bazel build --compilation_mode=dbg //cpp-low-latency
bazel test --cxxopt=-std=c++20 --test_output=all //cpp-gtest
bazel test --cxxopt=-std=c++14 --test_output=all cpp-gtest


# --features=fully_static_link will have problem on gtest
# be aware of linking config in .bazelrc

```
