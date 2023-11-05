# vulkan-bazel
repositories of vulkan with bazel

## install bazel
```shell
sudo apt install apt-transport-https curl gnupg -y
curl -fsSL https://bazel.build/bazel-release.pub.gpg | gpg --dearmor >bazel-archive-keyring.gpg
sudo mv bazel-archive-keyring.gpg /usr/share/keyrings
echo "deb [arch=amd64 signed-by=/usr/share/keyrings/bazel-archive-keyring.gpg] https://storage.googleapis.com/bazel-apt stable jdk1.8" | sudo tee /etc/apt/sources.list.d/bazel.list
sudo apt update && sudo apt install bazel
sudo apt update && sudo apt full-upgrade
bazel --version
```

## vulkan
```shell
touch WORKSPACE
# tools
# 1. git_repository
# 2. custom bazel rules: vulkan_repositories
# 3. http_archive

# cpp version
touch .bazelrc


mkdir -p third_party third_party/glm

# how to build the glm library
# cc_library
# glob
touch third_party/glm/glm.BUILD 

# alias
touch third_party/glm/BUILD

# for glfw
mkdir -p third_party/glfw
touch third_party/glfw/glfw.BUILD
touch third_party/glfw/BUILD

# for boost di
mkdir -p third_party/boost-ext-di
touch third_party/boost-ext-di/boost-ext-di.BUILD
touch third_party/boost-ext-di/BUILD

# for spdlog
mkdir -p third_party/spdlog
touch third_party/spdlog/spdlog.BUILD
touch third_party/spdlog/BUILD

# for simplest vulkan app
mkdir -p vulkan-hello-world
touch vulkan-hello-world/main.cpp vulkan-hello-world/BUILD

# build and run 
bazel build //vulkan-hello-world
bazel run //vulkan-hello-world

bazel query "//vulkan-hello-world:*"
bazel query --notool_deps --noimplicit_deps "deps(//vulkan-hello-world:main.cpp)" --output graph
```

## build shared library

### 1. core

```shell
# platform, application and window
mkdir -p engine engine/core
cd engine/core
touch BUILD platform.h platform.cpp window.h window.cpp application.h application.cpp
```

## build application upon library
```shell
mkdir -p vulkan-engine-app-101
touch vulkan-engine-app-101/BUILD vulkan-engine-app-101/main.cpp
bazel build //vulkan-engine-app-101
bazel run //vulkan-engine-app-101
```

## volk for bazel
```shell
# You can use volk in header-only fashion. Include volk.h wherever you want to use Vulkan functions. In exactly one source file, define VOLK_IMPLEMENTATION before including volk.h. Do not build volk.c at all in this case. This method of integrating volk makes it possible to set the platform defines mentioned above with arbitrary (preprocessor) logic in your code.

mkdir -p third_party/volk
touch third_party/volk/volk.BUILD
touch third_party/volk/BUILD

#define VK_NO_PROTOTYPES // for volk
#define VOLK_IMPLEMENTATION // for volk
#include "volk.h"
```