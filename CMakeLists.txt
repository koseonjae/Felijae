cmake_minimum_required(VERSION 3.24)

project(PracticeGL)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(Engine)

add_executable(PracticeGL
    main.cpp)

target_link_libraries(PracticeGL PRIVATE
    Engine)
