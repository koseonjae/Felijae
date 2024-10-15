cmake_minimum_required(VERSION 3.24)

project(PracticeGL)

set(CMAKE_CXX_STANDARD 17)

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_subdirectory(external)

add_executable(PracticeGL
    main.cpp
    OpenGL/OpenGLPipeline.cpp
    OpenGL/OpenGLPipeline.h
    OpenGL/OpenGLProgram.cpp
    OpenGL/OpenGLProgram.h
    OpenGL/OpenGLModel.cpp
    OpenGL/OpenGLModel.h
    Model/Triangle.cpp
    Model/Triangle.h
    Model/Object.h
    OpenGL/OpenGLTexture.cpp
    OpenGL/OpenGLTexture.h
    Utility/FileReader.cpp
    Utility/FileReader.h
    Utility/ImageLoader.cpp
    Utility/ImageLoader.h)

target_link_libraries(PracticeGL PRIVATE
    external
    glad::glad
    glfw
    glm::glm)

target_include_directories(PracticeGL PRIVATE
    ${CMAKE_SOURCE_DIR})
