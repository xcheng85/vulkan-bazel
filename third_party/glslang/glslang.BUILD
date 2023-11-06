licenses(["notice"])

package(default_visibility = ["//visibility:public"])

include_files = [
    "include/glslang/Include/arrays.h",
    "include/glslang/Include/BaseTypes.h",
    "include/glslang/Include/Common.h",
    "include/glslang/Include/ConstantUnion.h",
    "include/glslang/Include/glslang_c_interface.h",
    "include/glslang/Include/glslang_c_shader_types.h",
    "include/glslang/Include/InfoSink.h",
    "include/glslang/Include/InitializeGlobals.h",
    "include/glslang/Include/intermediate.h",
    "include/glslang/Include/PoolAlloc.h",
    "include/glslang/Include/ResourceLimits.h",
    "include/glslang/Include/ShHandle.h",
    "include/glslang/Include/SpirvIntrinsics.h",
    "include/glslang/Include/Types.h",

    "include/glslang/Public/ResourceLimits.h",
    "include/glslang/Public/resource_limits_c.h"
]

lib_files = [
    "lib/libGenericCodeGen.a",
    "lib/libglslang-default-resource-limits.a",
    "lib/libglslang.a",
    "lib/libHLSL.a",
    "lib/libMachineIndependent.a",
    "lib/libOGLCompiler.a",
    "lib/libOSDependent.a",
    "lib/libSPIRV.a",
    "lib/libSPIRV-Tools.a",
    "lib/libSPIRV-Tools-opt.a",
    "lib/libSPVRemapper.a",
    "lib/libSPIRV-Tools-shared.so"
]

cc_library(
    name = "glslang",
    srcs = lib_files,
    hdrs = include_files,
    includes=["include"],
    linkstatic = 1,
)