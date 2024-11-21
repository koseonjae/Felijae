add_library(Base STATIC
    include/Base/Object.h
    include/Base/Triangle.h

    src/Triangle.cpp
    src/Object.cpp)

find_package(glm CONFIG REQUIRED)
find_package(tinyobjloader CONFIG REQUIRED)

target_include_directories(Base PUBLIC
    include)

target_link_libraries(Base PUBLIC
    stb
    glm::glm
    tinyobjloader::tinyobjloader)
