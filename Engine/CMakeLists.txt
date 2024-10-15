add_library(Engine STATIC
    src/OpenGL/OpenGLPipeline.cpp
    src/OpenGL/OpenGLProgram.cpp
    src/OpenGL/OpenGLModel.cpp
    src/Model/Triangle.cpp
    src/OpenGL/OpenGLTexture.cpp
    src/Utility/FileReader.cpp
    src/Utility/ImageLoader.cpp

    include/OpenGL/OpenGLPipeline.h
    include/OpenGL/OpenGLProgram.h
    include/OpenGL/OpenGLModel.h
    include/Model/Triangle.h
    include/Model/Object.h
    include/OpenGL/OpenGLTexture.h
    include/Utility/FileReader.h
    include/Utility/ImageLoader.h)

target_include_directories(Engine PUBLIC
    include)

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_subdirectory(external)

target_link_libraries(Engine
    PUBLIC
    external
    PUBLIC
    glad::glad
    glfw
    glm::glm)
