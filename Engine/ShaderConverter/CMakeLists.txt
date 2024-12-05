add_library(ShaderConverter STATIC
    include/ShaderConverter/ShaderConverter.h

    src/ShaderConverter.cpp)

target_include_directories(ShaderConverter PUBLIC
    include)

find_package(glslang CONFIG REQUIRED)
find_package(spirv_cross_core CONFIG REQUIRED)
find_package(spirv_cross_glsl CONFIG REQUIRED)
find_package(spirv_cross_msl CONFIG REQUIRED)
find_package(spirv_cross_cpp CONFIG REQUIRED)
find_package(spirv_cross_util CONFIG REQUIRED)

target_link_libraries(ShaderConverter
    PUBLIC
    glslang::OSDependent glslang::glslang glslang::MachineIndependent glslang::GenericCodeGen glslang::glslang-default-resource-limits glslang::OGLCompiler glslang::SPVRemapper glslang::SPIRV glslang::HLSL
    spirv-cross-core
    spirv-cross-glsl
    spirv-cross-msl
    spirv-cross-cpp)
