
                message(WARNING "Using `glslangtestsTargets.cmake` is deprecated: use `find_package(glslang)` to find glslang CMake targets.")

                if (NOT TARGET glslang::glslangtests)
                    include("/home/xiao/github.com/xcheng85/glslang/build/install/lib/cmake/glslang/glslang-targets.cmake")
                endif()

                add_library(glslangtests ALIAS glslang::glslangtests)
            