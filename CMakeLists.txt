cmake_minimum_required(VERSION 3.24)

project(PracticeGL)

set(CMAKE_CXX_STANDARD 17)

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(PracticeGL main.cpp)

target_link_libraries(PracticeGL PRIVATE
    glad::glad
    glfw
    glm::glm)
