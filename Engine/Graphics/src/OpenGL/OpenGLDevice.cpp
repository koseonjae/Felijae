#include <Graphics/OpenGL/OpenGLDevice.h>
#include <Graphics/OpenGL/OpenGLPipeline.h>
#include <Graphics/OpenGL/OpenGLBuffer.h>
#include <Graphics/OpenGL/OpenGLTexture.h>

using namespace goala;

std::shared_ptr<Pipeline> OpenGLDevice::createPipeline(PipelineDescription desc) {
  auto pipeline = std::make_shared<OpenGLPipeline>(this, std::move(desc));
  return pipeline;
}

std::shared_ptr<Buffer> OpenGLDevice::createBuffer(BufferDescription desc) {
  auto buffer = std::make_shared<OpenGLBuffer>(this, std::move(desc));
  return buffer;
}

std::shared_ptr<Texture> OpenGLDevice::createTexture(TextureDescription desc) {
  auto texture = std::make_shared<OpenGLTexture>(this, desc);
  return texture;
}