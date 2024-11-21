add_library(Engine STATIC
    src/OpenGL/OpenGLPipeline.cpp
    src/OpenGL/OpenGLRasterizer.cpp
    src/OpenGL/OpenGLOutputMerger.cpp
    src/OpenGL/OpenGLProgram.cpp
    src/OpenGL/OpenGLModel.cpp
    src/OpenGL/OpenGLBuffer.cpp
    src/OpenGL/OpenGLTexture.cpp
    src/OpenGL/OpenGLFrameBuffer.cpp
    src/OpenGL/OpenGLRenderPass.cpp
    src/OpenGL/OpenGLUtils.cpp
    src/Metal/MetalDevice.cpp
    src/Metal/MetalBuffer.cpp
    src/Metal/MetalShader.cpp
    src/Model/Triangle.cpp
    src/Model/Object.cpp
    src/Model/Camera.cpp
    src/Model/Scene.cpp
    src/Utility/FileReader.cpp
    src/Utility/ImageLoader.cpp
    src/Utility/ThreadChecker.cpp

    include/OpenGL/OpenGLPipeline.h
    include/OpenGL/OpenGLRasterizer.h
    include/OpenGL/OpenGLOutputMerger.h
    include/OpenGL/OpenGLProgram.h
    include/OpenGL/OpenGLModel.h
    include/OpenGL/OpenGLBuffer.h
    include/OpenGL/OpenGLTexture.h
    include/OpenGL/OpenGLFrameBuffer.h
    include/OpenGL/OpenGLRenderPass.h
    include/OpenGL/OpenGLUtils.h
    include/Metal/MetalDevice.h
    include/Metal/MetalBuffer.h
    include/Metal/MetalShader.h
    include/Model/Triangle.h
    include/Model/Object.h
    include/Utility/ImageFormat.h
    include/Model/Model.h
    include/Model/Program.h
    include/Model/Rasterizer.h
    include/Model/OutputMerger.h
    include/Model/RenderPass.h
    include/Model/Light.h
    include/Model/Camera.h
    include/Model/Scene.h
    include/Model/Texture.h
    include/Utility/FileReader.h
    include/Utility/ImageLoader.h
    include/Utility/ThreadChecker.h)

target_include_directories(Engine PUBLIC
    include)

find_package(OpenGL REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(tinyobjloader CONFIG REQUIRED)

add_subdirectory(external)
add_subdirectory(graphics)

target_link_libraries(Engine
    PUBLIC
    external
    PUBLIC
    glm::glm
    OpenGL::GL
    metal
    tinyobjloader::tinyobjloader)
