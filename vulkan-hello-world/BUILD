# app1/BUILD
load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "vulkan-hello-world",
    srcs = glob(["*.cpp"]),
    deps = [
        "//third_party/glfw",
        "//third_party/glm",
        "@rules_vulkan//vulkan:vulkan_cc_library"
    ],
    copts = ["--std=c++23"],
)