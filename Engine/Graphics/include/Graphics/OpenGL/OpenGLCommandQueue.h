#pragma once

#include <Graphics/Model/CommandQueue.h>

namespace goala {
class OpenGLDevice;

class OpenGLCommandQueue : public CommandQueue {
public:
  OpenGLCommandQueue(OpenGLDevice* device, CommandQueueDescription desc);
  std::shared_ptr<CommandBuffer> createCommandBuffer(CommandBufferDescription desc) override;

private:
  OpenGLDevice* m_device = nullptr;
};
} // namespace goala
