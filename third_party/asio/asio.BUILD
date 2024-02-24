# third_party/asio/asio.BUILD
load("@rules_cc//cc:defs.bzl", "cc_library")

# hpp, ipp for asio
cc_library(
    name = "asio",
    hdrs = glob(["include/**/*.*", "include/**/*.h"]),
    includes = ["include"],
    visibility = ["//visibility:public"],
)
