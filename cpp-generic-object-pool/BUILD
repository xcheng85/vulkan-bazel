# cpp-generic-object-pool/BUILD
load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "cpp-generic-object-pool",
    srcs = glob(["*.cpp", "*.h"]),
    deps = [
        "//third_party/spdlog",
    ],
    copts = ["--std=c++23"],
    linkopts = ["-pg"]
)