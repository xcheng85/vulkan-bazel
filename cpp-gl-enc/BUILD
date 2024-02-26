# cpp-low-latency/BUILD
load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "cpp-gl-enc",
    srcs = glob(["*.cpp", "*.h"]),
    deps = [
        "//third_party/spdlog",
    ],
    copts = ["--std=c++23"],
    linkopts = ["-lpthread", "-lGL", "-lglut", "-lGLU", "-lGLEW", "-lX11", "-lEGL"]
)
