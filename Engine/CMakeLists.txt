add_library(Engine STATIC
    src/OpenGL/OpenGLPipeline.cpp
    src/OpenGL/OpenGLProgram.cpp
    src/OpenGL/OpenGLModel.cpp
    src/Model/Triangle.cpp
    src/Model/Object.cpp
    src/Model/Camera.cpp
    src/Model/Scene.cpp
    src/OpenGL/OpenGLTexture.cpp
    src/Utility/FileReader.cpp
    src/Utility/ImageLoader.cpp
    src/Utility/ThreadChecker.cpp

    include/OpenGL/OpenGLPipeline.h
    include/OpenGL/OpenGLProgram.h
    include/OpenGL/OpenGLModel.h
    include/Model/Triangle.h
    include/Model/Object.h
    include/Model/Model.h
    include/Model/Program.h
    include/Model/Pipeline.h
    include/Model/Light.h
    include/Model/Camera.h
    include/Model/Scene.h
    include/Model/Texture.h
    include/OpenGL/OpenGLTexture.h
    include/Utility/FileReader.h
    include/Utility/ImageLoader.h
    include/Utility/ThreadChecker.h)

target_include_directories(Engine PUBLIC
    include)

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(tinyobjloader CONFIG REQUIRED)

add_subdirectory(external)

target_link_libraries(Engine
    PUBLIC
    external
    PUBLIC
    glad::glad
    glfw
    glm::glm
    tinyobjloader::tinyobjloader)
