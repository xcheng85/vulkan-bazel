load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "core",
    srcs = [],
    hdrs = ["app-root.h", "db.h", "mux.h"],
)

cc_binary(
    name = "cpp-webserver",
    srcs = ["web-app.h","main.cpp"],
    deps = [
        ":core",
    ],
)

cc_test(
  name = "web-app-test",
  srcs = ["web-app-test.cc", "web-app.h"],
  copts = ["-Iexternal/gtest/include"],
    deps = [
        ":core",
        "@gtest//:gtest_main"
    ],
) 
