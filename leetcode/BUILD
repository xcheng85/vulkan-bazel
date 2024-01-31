# leetcode/BUILD
load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "leetcode",
    srcs = glob(["*.cpp", "*.h", "**/*.h"]),
    deps = [
        "//third_party/spdlog",
    ],
    copts = ["--std=c++23"],
    linkopts = ["-pg"]
)