
        message(WARNING "Using `glslang-standaloneTargets.cmake` is deprecated: use `find_package(glslang)` to find glslang CMake targets.")

        if (NOT TARGET glslang::glslang-standalone)
            include("/home/xiao/github.com/xcheng85/glslang/build/install/lib/cmake/glslang/glslang-targets.cmake")
        endif()

        add_library(glslang-standalone ALIAS glslang::glslang-standalone)
    