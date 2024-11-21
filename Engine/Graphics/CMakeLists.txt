add_library(Graphics STATIC
    src/OpenGL/OpenGLPipeline.cpp
    src/OpenGL/OpenGLRasterizer.cpp
    src/OpenGL/OpenGLOutputMerger.cpp
    src/OpenGL/OpenGLProgram.cpp
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
    src/Model/Model.cpp
    src/Utility/FileReader.cpp
    src/Utility/ImageLoader.cpp
    src/Utility/ThreadChecker.cpp

    include/Graphics/OpenGL/OpenGLPipeline.h
    include/Graphics/OpenGL/OpenGLRasterizer.h
    include/Graphics/OpenGL/OpenGLOutputMerger.h
    include/Graphics/OpenGL/OpenGLProgram.h
    include/Graphics/OpenGL/OpenGLBuffer.h
    include/Graphics/OpenGL/OpenGLTexture.h
    include/Graphics/OpenGL/OpenGLFrameBuffer.h
    include/Graphics/OpenGL/OpenGLRenderPass.h
    include/Graphics/OpenGL/OpenGLUtils.h
    include/Graphics/Metal/MetalDevice.h
    include/Graphics/Metal/MetalBuffer.h
    include/Graphics/Metal/MetalShader.h
    include/Graphics/Model/Triangle.h
    include/Graphics/Model/Object.h
    include/Graphics/Utility/ImageFormat.h
    include/Graphics/Utility/ImageData.h
    include/Graphics/Model/Model.h
    include/Graphics/Model/Program.h
    include/Graphics/Model/Rasterizer.h
    include/Graphics/Model/OutputMerger.h
    include/Graphics/Model/RenderPass.h
    include/Graphics/Model/Light.h
    include/Graphics/Model/Camera.h
    include/Graphics/Model/Scene.h
    include/Graphics/Model/Texture.h
    include/Graphics/Utility/FileReader.h
    include/Graphics/Utility/ImageLoader.h
    include/Graphics/Utility/ThreadChecker.h)

target_include_directories(Graphics PUBLIC
    include)

find_package(OpenGL REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(tinyobjloader CONFIG REQUIRED)

add_subdirectory(external)

target_link_libraries(Graphics
    PRIVATE
    stb
    PUBLIC
    glm::glm
    OpenGL::GL
    MetalCpp
    tinyobjloader::tinyobjloader)
