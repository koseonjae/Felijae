#pragma once

#include <Graphics/Model/CommandBuffer.h>

#include <functional>

namespace larco {
class OpenGLDevice;
class OpenGLCommandQueue;

class OpenGLCommandBuffer : public CommandBuffer, public std::enable_shared_from_this<OpenGLCommandBuffer> {
public:
  OpenGLCommandBuffer(OpenGLDevice* device, OpenGLCommandQueue* queue, CommandBufferDescription desc);

  void present(Texture* texture) override;

  void commit() override;

  std::shared_ptr<CommandEncoder> createCommandEncoder(RenderPass* renderPass, CommandEncoderDescription desc) override;

  std::shared_ptr<ComputeCommandEncoder> createComputeCommandEncoder(ComputeCommandEncoderDescription desc) override;

  void waitUntilCompleted() override;

  void waitUntilScheduled() override;

private:
  OpenGLDevice* m_device = nullptr;
  OpenGLCommandQueue* m_queue = nullptr;
  std::vector<std::function<void()>> m_encodedCommands;
  std::mutex m_cmdMutex;
  std::atomic<int> m_encoderCnt = 0;
  std::atomic<bool> m_commandComitted = false;
};
} // namespace larco
