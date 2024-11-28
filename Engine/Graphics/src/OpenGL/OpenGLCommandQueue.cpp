#include <Graphics/OpenGL/OpenGLCommandQueue.h>
#include <Graphics/OpenGL/OpenGLCommandBuffer.h>

using namespace goala;

OpenGLCommandQueue::OpenGLCommandQueue(OpenGLDevice* device, CommandQueueDescription desc)
  : m_device(device) {}

std::shared_ptr<CommandBuffer> OpenGLCommandQueue::createCommandBuffer(CommandBufferDescription desc) {
  auto commandBuffer = std::make_shared<OpenGLCommandBuffer>(m_device, this, std::move(desc));
  return commandBuffer;
}
