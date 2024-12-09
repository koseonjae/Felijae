set(GRAPHICS_DEMO demo_graphics)
set(COMPUTE_DEMO demo_compute)
set(SHADER_DEMO demo_shader_converter)

set(EXECUTABLE_INTERFACE executable_interface)

add_executable(${GRAPHICS_DEMO} demo_graphics.cpp)
add_executable(${COMPUTE_DEMO} demo_compute.cpp)
add_executable(${SHADER_DEMO} demo_shader_converter.cpp)

add_subdirectory(SDLWrapper)

add_library(${EXECUTABLE_INTERFACE} INTERFACE)
target_compile_definitions(${EXECUTABLE_INTERFACE} INTERFACE DEMO_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(${EXECUTABLE_INTERFACE} INTERFACE SDLWrapper Engine)

target_link_libraries(${GRAPHICS_DEMO} PRIVATE ${EXECUTABLE_INTERFACE})
target_link_libraries(${COMPUTE_DEMO} PRIVATE ${EXECUTABLE_INTERFACE})
target_link_libraries(${SHADER_DEMO} PRIVATE ${EXECUTABLE_INTERFACE})
