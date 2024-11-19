cmake_minimum_required(VERSION 3.24)

project(PracticeGL)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(Engine)

set(GL_DEMO demo_sdl_gl)
set(METAL_DEMO demo_sdl_metal)

add_executable(${GL_DEMO} demo_sdl_gl.cpp)
add_executable(${METAL_DEMO} demo_sdl_metal.cpp)

find_package(SDL2 CONFIG REQUIRED)

target_link_libraries(${GL_DEMO} PRIVATE
    Engine
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>)

target_link_libraries(${METAL_DEMO} PRIVATE
    Engine
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>)
