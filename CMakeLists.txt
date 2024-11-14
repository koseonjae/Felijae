cmake_minimum_required(VERSION 3.24)

project(PracticeGL)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -x objective-c++")

add_subdirectory(Engine)

add_executable(demo_glfw_gl
    demo_glfw_gl.cpp)

set(demo_sdl2_srcs demo_sdl_metal.cpp)
#add_compiled_metal_sources(demo_sdl2_srcs triangle.metal)

add_executable(demo_sdl_metal ${demo_sdl2_srcs})

find_package(SDL2 CONFIG REQUIRED)

target_link_libraries(demo_glfw_gl PRIVATE
    Engine)

target_link_libraries(demo_sdl_metal PRIVATE
    Engine
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>)
