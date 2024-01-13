# cpp-06/BUILD
load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "cpp-06",
    srcs = glob(["*.cpp"]),
    deps = [
        "//third_party/spdlog",
    ],
    copts = ["--std=c++23"],
)