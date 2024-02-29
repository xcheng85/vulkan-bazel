load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_binary(
    name = "cpp-asio-tcp-server-async",
    srcs = ["main.cpp", "v1/server.h","v2/server.h"],
    deps = [
        "//third_party/asio",
        "//third_party/spdlog"
    ],
)

