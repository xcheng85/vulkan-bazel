load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "core",
    srcs = glob(["*.cpp"]),
    hdrs = glob(["*.h"]),
    deps = [
        "//third_party/spdlog",
        "//third_party/boost-ext-di",
        "//third_party/glfw",
    ],
    visibility = ["//visibility:public"],
    copts = ["--std=c++23"],
)
