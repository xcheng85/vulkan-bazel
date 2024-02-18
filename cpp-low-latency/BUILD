# cpp-low-latency/BUILD
load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "cpp-low-latency",
    srcs = glob(["*.cpp", "*.h", "**/*.h"]),
    deps = [
        "//third_party/spdlog",
    ],
    copts = ["--std=c++23"],
    linkopts = ["-lpthread"]
)
