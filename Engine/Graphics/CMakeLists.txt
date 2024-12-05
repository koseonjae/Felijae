add_library(Graphics STATIC
    src/Utility/ImageFormatUtil.cpp
    src/Utility/MetalBlitCopy.cpp
    src/Model/Uniforms.cpp
    src/OpenGL/OpenGLCommandEncoder.cpp
    src/OpenGL/OpenGLCommandQueue.cpp
    src/OpenGL/OpenGLCommandBuffer.cpp
    src/OpenGL/OpenGLDevice.cpp
    src/OpenGL/OpenGLPipeline.cpp
    src/OpenGL/OpenGLProgram.cpp
    src/OpenGL/OpenGLBuffer.cpp
    src/OpenGL/OpenGLTexture.cpp
    src/OpenGL/OpenGLFrameBuffer.cpp
    src/OpenGL/OpenGLRenderPass.cpp
    src/OpenGL/OpenGLUtils.cpp
    src/Metal/MetalCommandEncoder.cpp
    src/Metal/MetalCommandQueue.cpp
    src/Metal/MetalCommandBuffer.cpp
    src/Metal/MetalDevice.cpp
    src/Metal/MetalFence.cpp
    src/Metal/MetalBuffer.cpp
    src/Metal/MetalRenderPass.cpp
    src/Metal/MetalShader.cpp
    src/Metal/MetalTexture.cpp
    src/Metal/MetalPipeline.cpp

    include/Graphics/Utility/ImageFormatUtil.h
    include/Graphics/Utility/MetalBlitCopy.h
    include/Graphics/Utility/MetalRef.h
    include/Graphics/Model/CommandEncoder.h
    include/Graphics/Model/CommandQueue.h
    include/Graphics/Model/CommandBuffer.h
    include/Graphics/Model/Device.h
    include/Graphics/Model/Fence.h
    include/Graphics/Model/Shader.h
    include/Graphics/Model/Uniforms.h
    include/Graphics/Model/Rasterizer.h
    include/Graphics/Model/OutputMerger.h
    include/Graphics/Model/RenderPass.h
    include/Graphics/Model/Texture.h
    include/Graphics/OpenGL/OpenGLCommandEncoder.h
    include/Graphics/OpenGL/OpenGLCommandQueue.h
    include/Graphics/OpenGL/OpenGLCommandBuffer.h
    include/Graphics/OpenGL/OpenGLDevice.h
    include/Graphics/OpenGL/OpenGLPipeline.h
    include/Graphics/OpenGL/OpenGLProgram.h
    include/Graphics/OpenGL/OpenGLBuffer.h
    include/Graphics/OpenGL/OpenGLTexture.h
    include/Graphics/OpenGL/OpenGLFrameBuffer.h
    include/Graphics/OpenGL/OpenGLRenderPass.h
    include/Graphics/OpenGL/OpenGLUtils.h
    include/Graphics/Metal/MetalCommandEncoder.h
    include/Graphics/Metal/MetalCommandQueue.h
    include/Graphics/Metal/MetalCommandBuffer.h
    include/Graphics/Metal/MetalDevice.h
    include/Graphics/Metal/MetalFence.h
    include/Graphics/Metal/MetalBuffer.h
    include/Graphics/Metal/MetalRenderPass.h
    include/Graphics/Metal/MetalShader.h
    include/Graphics/Metal/MetalTexture.h
    include/Graphics/Metal/MetalPipeline.h)

target_include_directories(Graphics PUBLIC
    include)

find_package(OpenGL REQUIRED)

target_link_libraries(Graphics
    PUBLIC
    OpenGL::GL
    MetalCpp
    ShaderConverter # todo: Engine이 content를 받아서 (demo가 아닌) Engine 내부에서 pipeline을 만들도록 변경한 후에, ShaderConverting을 engine이 해주고, 그래픽스 레이어에선 ShaderConverter가 없도록 리팩토링
    Base)
