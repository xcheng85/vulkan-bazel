# app1/BUILD
load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "vulkan-engine-app-101",
    srcs = glob(["*.cpp"]),
    deps = [
        "//third_party/glfw",
        "//third_party/glm",
        "//third_party/spdlog",
        "//third_party/boost-ext-di",
        "//engine/core",
        "//third_party/volk",
        "@rules_vulkan//vulkan:vulkan_cc_library"
    ],
    copts = ["--std=c++23"],
)