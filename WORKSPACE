# WORKSPACE
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "rules_vulkan",
    remote = "https://github.com/jadarve/rules_vulkan.git",
    tag = "v0.0.11"
)

load("@rules_vulkan//vulkan:repositories.bzl", "vulkan_repositories")

vulkan_repositories()

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

GLM_VERSION = "0.9.9.8"

http_archive(
    name = "glm",
    build_file = "@//third_party/glm:glm.BUILD",
    sha256 = "4605259c22feadf35388c027f07b345ad3aa3b12631a5a316347f7566c6f1839",
    strip_prefix = "glm-{}".format(GLM_VERSION),
    urls = ["https://github.com/g-truc/glm/archive/{}.zip".format(GLM_VERSION)],
)

GLFW_VERSION = "3.3.5"

http_archive(
    name = "glfw",
    build_file = "@//third_party/glfw:glfw.BUILD",
    sha256 = "a89bb6074bc12bc12fcd322dcf848af81b679ccdc695f70b29ca8a9aa066684b",
    strip_prefix = "glfw-{}".format(GLFW_VERSION),
    urls = ["https://github.com/glfw/glfw/archive/{}.zip".format(GLFW_VERSION)],
)

git_repository(
    name = "boost-ext-di",

    remote = "https://github.com/boost-ext/di.git",
    tag = "v1.3.0"
)

git_repository(
    name = "spdlog",
    build_file = "@//third_party/spdlog:spdlog.BUILD",
    remote = "https://github.com/gabime/spdlog.git",
    tag = "v1.12.0"
)

git_repository(
    name = "volk",
    build_file = "@//third_party/volk:volk.BUILD",
    remote = "https://github.com/zeux/volk.git",
    tag = "vulkan-sdk-1.3.268.0"
)

git_repository(
    name = "gtest",
    build_file = "@//third_party/gtest:gtest.BUILD",
    remote = "https://github.com/google/googletest.git",
    tag = "v1.14.0"
)

git_repository(
    name = "vma",
    build_file = "@//third_party/vma:vma.BUILD",
    remote = "https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git",
    tag = "v3.0.1"
)



http_archive(
    name = "magic_enum",
    build_file = "@//third_party/magic_enum:magic_enum.BUILD",
    strip_prefix = "magic_enum-e55b9b54d5cf61f8e117cafb17846d7d742dd3b4",
    urls = ["https://github.com/Neargye/magic_enum/archive/e55b9b54d5cf61f8e117cafb17846d7d742dd3b4.zip"],
)

http_archive(
    name = "hedron_make_cc_https_easy",
    url = "https://github.com/hedronvision/bazel-make-cc-https-easy/archive/8df70a9d5eeb68c791412e097c4841e2083b2b47.tar.gz",
    strip_prefix = "bazel-make-cc-https-easy-8df70a9d5eeb68c791412e097c4841e2083b2b47",
)
load("@hedron_make_cc_https_easy//:workspace_setup.bzl", "hedron_make_cc_https_easy")
hedron_make_cc_https_easy()
load("@hedron_make_cc_https_easy//:transitive_workspace_setup.bzl", "hedron_keep_cc_https_easy")
hedron_keep_cc_https_easy()


all_content = """filegroup(
    name = "all",
    srcs = glob(["**"]),
    visibility = ["//visibility:public"]
)
"""

http_archive(
    name = "rules_foreign_cc",
    sha256 = "2a4d07cd64b0719b39a7c12218a3e507672b82a97b98c6a89d38565894cf7c51",
    strip_prefix = "rules_foreign_cc-0.9.0",
    url = "https://github.com/bazelbuild/rules_foreign_cc/archive/refs/tags/0.9.0.tar.gz",
)

load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")

rules_foreign_cc_dependencies()

http_archive(
    name = "opencv",
    build_file_content = all_content,
    sha256 = "a61e7a4618d353140c857f25843f39b2abe5f451b018aab1604ef0bc34cd23d5",
    strip_prefix = "opencv-4.5.3",
    urls = [
        "https://github.com/opencv/opencv/archive/4.5.3.zip",
    ],
)

http_archive(
    name = "glslang",
    build_file_content = all_content,
    strip_prefix = "glslang-vulkan-sdk-1.3.268.0",
    urls = [
        "https://github.com/KhronosGroup/glslang/archive/refs/tags/vulkan-sdk-1.3.268.0.zip"
    ],
)

http_archive(
    name = "asio",
    build_file = "@//third_party/asio:asio.BUILD",
    strip_prefix = "asio-asio-1-29-0/asio",
    urls = [
        "https://github.com/chriskohlhoff/asio/archive/refs/tags/asio-1-29-0.zip",
    ],
)

http_archive(
    name = "rules_cuda",
    sha256 = "2f8c8c8c85f727bec4423efecec12d3b751cb0a98bda99f0f9d351608a23b858",
    strip_prefix = "rules_cuda-v0.2.1",
    urls = ["https://github.com/bazel-contrib/rules_cuda/releases/download/v0.2.1/rules_cuda-v0.2.1.tar.gz"],
)
load("@rules_cuda//cuda:repositories.bzl", "register_detected_cuda_toolchains", "rules_cuda_dependencies")
rules_cuda_dependencies()
register_detected_cuda_toolchains()