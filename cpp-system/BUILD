# cpp-system/BUILD
load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "cpp-system",
    srcs = glob(["*.cpp"]),
    deps = [
        "//third_party/spdlog",
    ],
    copts = ["--std=c++23"],
    linkopts = ["-lpthread"]
)
