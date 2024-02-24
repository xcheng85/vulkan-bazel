# Web server framework

mimic web server skeleton 

1. app, ioc container (root)
2. db
3. mux

## Build

```shell
bazel build --strip=never --compilation_mode=dbg //cpp-webserver
```

## GTest
```shell
bazel test --cxxopt=-std=c++20 --test_output=all --strip=never --compilation_mode=dbg //cpp-webserver

bazel test --cxxopt=-std=c++20 --test_output=all //cpp-webserver/test:test

```

## GMock
mock action
https://google.github.io/googletest/reference/actions.html

1. return
2. throw