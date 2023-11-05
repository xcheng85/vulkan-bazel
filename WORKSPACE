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
    build_file = "@//third_party/boost-ext-di:boost-ext-di.BUILD",
    remote = "https://github.com/boost-ext/di.git",
    tag = "v1.3.0"
)

git_repository(
    name = "spdlog",
    build_file = "@//third_party/spdlog:spdlog.BUILD",
    remote = "https://github.com/gabime/spdlog.git",
    tag = "v1.12.0"
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
