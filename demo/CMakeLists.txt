set(GRAPHICS_DEMO demo_graphics)
set(GL_DEMO demo_sdl_gl)
set(METAL_DEMO demo_sdl_metal)
set(SHADER_DEMO demo_shader)
set(EXECUTABLE_INTERFACE executable_interface)

add_executable(${GRAPHICS_DEMO} demo_graphics.cpp)
add_executable(${GL_DEMO} demo_sdl_gl.cpp)
add_executable(${METAL_DEMO} demo_sdl_metal.cpp)
add_executable(${SHADER_DEMO} demo_shader.cpp)

add_subdirectory(SDLWrapper)

add_library(${EXECUTABLE_INTERFACE} INTERFACE)
target_compile_definitions(${EXECUTABLE_INTERFACE} INTERFACE DEMO_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(${EXECUTABLE_INTERFACE} INTERFACE SDLWrapper Engine)

target_link_libraries(${GRAPHICS_DEMO} PRIVATE ${EXECUTABLE_INTERFACE})
target_link_libraries(${GL_DEMO} PRIVATE ${EXECUTABLE_INTERFACE})
target_link_libraries(${METAL_DEMO} PRIVATE ${EXECUTABLE_INTERFACE})
target_link_libraries(${SHADER_DEMO} PRIVATE ${EXECUTABLE_INTERFACE})