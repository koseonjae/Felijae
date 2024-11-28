#include <Graphics/OpenGL/OpenGLCommandBuffer.h>
#include <Graphics/OpenGL/OpenGLCommandQueue.h>
#include <Graphics/OpenGL/OpenGLDevice.h>

#include <Graphics/Model/Pipeline.h>

using namespace goala;

OpenGLCommandBuffer::OpenGLCommandBuffer(OpenGLDevice* device, OpenGLCommandQueue* queue, CommandBufferDescription desc)
  : m_device(device)
  , m_queue(queue) {}

void OpenGLCommandBuffer::encode(RenderPass* renderPass, Pipeline* pipeline) {
  m_encoded = [pipeline]() {
    pipeline->render();
  };
}

void OpenGLCommandBuffer::present(Texture* texture) {
  // todo: swap buffer를 lambda로 감싸는 일을 하고, 이걸 commit해서 해야 할 듯
}

void OpenGLCommandBuffer::commit() {
  assert(m_encoded && "m_encoded is empty");
  m_encoded();
  m_encoded = nullptr;
}

void OpenGLCommandBuffer::addDependency(CommandBuffer* before) {
  assert(false && "need to implement");
}
