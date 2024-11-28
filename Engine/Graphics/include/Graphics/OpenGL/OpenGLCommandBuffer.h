#pragma once

#include <Graphics/Model/CommandBuffer.h>

#include <functional>

namespace goala {
class OpenGLDevice;
class OpenGLCommandQueue;

class OpenGLCommandBuffer : public CommandBuffer {
public:
  OpenGLCommandBuffer(OpenGLDevice* device, OpenGLCommandQueue* queue, CommandBufferDescription desc);
  void encode(RenderPass* renderPass, Pipeline* pipeline) override;

  void present(Texture* texture) override;

  void commit() override;

  void addDependency(CommandBuffer* before) override;

private:
  OpenGLDevice* m_device = nullptr;
  OpenGLCommandQueue* m_queue = nullptr;
  std::function<void()> m_encoded;
};
} // namespace goala
