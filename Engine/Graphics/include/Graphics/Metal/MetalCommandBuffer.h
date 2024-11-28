#pragma once

#include <Graphics/Model/CommandBuffer.h>
#include <Graphics/Utility/MetalRef.h>

#include <vector>

namespace MTL {
class CommandBuffer;
class Fence;
}

namespace goala {
class MetalDevice;
class MetalCommandQueue;
class Fence;
class RenderPass;

class MetalCommandBuffer : public CommandBuffer {
public:
  MetalCommandBuffer(MetalDevice* device, MetalCommandQueue* queue, CommandBufferDescription desc);

  void present(Texture* texture) override;

  void commit() override;

  std::shared_ptr<CommandEncoder> createCommandEncoder(RenderPass* renderPass, CommandEncoderDescription desc) override;

  void waitUntilCompleted() override;

  void waitUntilScheduled() override;

  MTL::CommandBuffer* getCommandBuffer();

  const MTL::CommandBuffer* getCommandBuffer() const;

private:
  MetalDevice* m_device = nullptr;
  MetalCommandQueue* m_queue = nullptr;
  MetalRef<MTL::CommandBuffer> m_cmdBuf;
};
} // namespace goala
