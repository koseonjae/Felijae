#include <Graphics/OpenGL/OpenGLDevice.h>
#include <Graphics/OpenGL/OpenGLCommandQueue.h>
#include <Graphics/OpenGL/OpenGLPipeline.h>
#include <Graphics/OpenGL/OpenGLBuffer.h>
#include <Graphics/OpenGL/OpenGLTexture.h>
#include <Graphics/Model/Fence.h>

#include "Graphics/OpenGL/OpenGLRenderPass.h"

using namespace goala;

std::shared_ptr<Pipeline> OpenGLDevice::createPipeline(PipelineDescription desc) {
  auto pipeline = std::make_shared<OpenGLPipeline>(this, std::move(desc));
  return pipeline;
}

std::shared_ptr<Buffer> OpenGLDevice::createBuffer(VertexBufferDescription desc) {
  auto buffer = std::make_shared<OpenGLBuffer>(this, std::move(desc));
  return buffer;
}

std::shared_ptr<Texture> OpenGLDevice::createTexture(TextureDescription desc) {
  auto texture = std::make_shared<OpenGLTexture>(this, desc);
  return texture;
}

std::shared_ptr<Fence> OpenGLDevice::createFence(FenceDescription desc) {
  assert(false && "OpenGL Fence is not supported");
}

std::shared_ptr<CommandQueue> OpenGLDevice::createCommandQueue(CommandQueueDescription desc) {
  auto queue = std::make_shared<OpenGLCommandQueue>(this, std::move(desc));
  return queue;
}

std::shared_ptr<Shader> OpenGLDevice::createShader(ShaderDescription desc) {
  assert(false && "OpenGL Shader is not supported");
}

std::shared_ptr<RenderPass> OpenGLDevice::createRenderPass(RenderPassDescription desc) {
  auto renderPass = std::make_shared<OpenGLRenderPass>(this, std::move(desc));
  return renderPass;
}
