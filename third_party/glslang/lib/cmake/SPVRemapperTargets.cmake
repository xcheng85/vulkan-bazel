
            message(WARNING "Using `SPVRemapperTargets.cmake` is deprecated: use `find_package(glslang)` to find glslang CMake targets.")

            if (NOT TARGET glslang::SPVRemapper)
                include("/home/xiao/github.com/xcheng85/glslang/build/install/lib/cmake/glslang/glslang-targets.cmake")
            endif()

            add_library(SPVRemapper ALIAS glslang::SPVRemapper)
        