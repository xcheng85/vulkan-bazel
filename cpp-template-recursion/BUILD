# cpp-template-recursion/BUILD
load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "cpp-template-recursion",
    srcs = glob(["*.cpp", "*.h"]),
    deps = [
        "//third_party/spdlog",
    ],
    copts = ["--std=c++23"],
    linkopts = ["-pg"]
)