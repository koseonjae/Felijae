#include <Graphics/OpenGL/OpenGLDevice.h>
#include <Graphics/OpenGL/OpenGLPipeline.h>
#include <Graphics/OpenGL/OpenGLBuffer.h>

using namespace goala;

std::shared_ptr<Pipeline> OpenGLDevice::createPipeline(const PipelineDescription& desc) {
  auto pipeline = std::make_shared<OpenGLPipeline>(this, desc);
  return pipeline;
}

std::shared_ptr<Buffer> OpenGLDevice::createBuffer(const BufferDescription& desc) {
  auto buffer = std::make_shared<OpenGLBuffer>(this, desc);
  return buffer;
}