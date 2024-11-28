#include <Graphics/OpenGL/OpenGLCommandBuffer.h>
#include <Graphics/Model/Pipeline.h>
#include <Graphics/OpenGL/OpenGLCommandEncoder.h>
#include <Graphics/OpenGL/OpenGLRenderPass.h>

#include <OpenGL/gl3.h>

#include <thread>

using namespace goala;

OpenGLCommandBuffer::OpenGLCommandBuffer(OpenGLDevice* device, OpenGLCommandQueue* queue, CommandBufferDescription desc)
  : m_device(device)
  , m_queue(queue) {}

void OpenGLCommandBuffer::present(Texture* texture) {
  // todo: present도 m_encodedCommands에 넣어야 할듯
}

void OpenGLCommandBuffer::commit() {
  decltype(m_encodedCommands) encodedCommands;
  {
    std::lock_guard<std::mutex> l(m_cmdMutex);
    encodedCommands = std::move(m_encodedCommands);
  }
  assert(m_encoderCnt == encodedCommands.size() && "Some encoders have not yet finished encoding");

  for (auto& command : encodedCommands) {
    assert(command && "command is not encoded");
    command();
  }
  m_commandComitted = true;
}

std::shared_ptr<CommandEncoder> OpenGLCommandBuffer::createCommandEncoder(RenderPass* renderPass, CommandEncoderDescription desc) {
  auto glRenderPass = static_cast<OpenGLRenderPass*>(renderPass);
  auto encoder = std::make_shared<OpenGLCommandEncoder>(this, glRenderPass, std::move(desc));
  encoder->setEncodedCallback([weak = weak_from_this()](std::function<void()>&& command) {
    auto self = weak.lock();
    if (!self)
      return;
    std::lock_guard<std::mutex> l(self->m_cmdMutex);
    self->m_encodedCommands.push_back(std::move(command));
  });
  ++m_encoderCnt;
  return encoder;
}

void OpenGLCommandBuffer::waitUntilCompleted() {
  glFinish(); // todo: fence로 대체
}

void OpenGLCommandBuffer::waitUntilScheduled() {
  while (m_commandComitted == false)
    std::this_thread::yield();
}
