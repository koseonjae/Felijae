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
    include/Graphics/Model/Program.h
    include/Graphics/Model/Rasterizer.h
    include/Graphics/Model/OutputMerger.h
    include/Graphics/Model/RenderPass.h
    include/Graphics/Model/Texture.h)

target_include_directories(Graphics PUBLIC
    include)

find_package(OpenGL REQUIRED)

target_link_libraries(Graphics
    PRIVATE
    stb
    PUBLIC
    OpenGL::GL
    MetalCpp
    Base)
