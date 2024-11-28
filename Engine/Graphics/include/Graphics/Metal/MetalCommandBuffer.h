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

class MetalCommandBuffer : public CommandBuffer {
public:
  MetalCommandBuffer(MetalDevice* device, MetalCommandQueue* queue, CommandBufferDescription desc);

  void encode(RenderPass* renderPass, Pipeline* pipeline) override;

  void present(Texture* texture) override;

  void commit() override;

  void addDependency(CommandBuffer* before) override;

  MTL::CommandBuffer* getCommandBuffer();

  const MTL::CommandBuffer* getCommandBuffer() const;

private:
  void _addSignalFence(std::shared_ptr<Fence> fence);
  void _addWaitFence(std::shared_ptr<Fence> fence);

private:
  MetalDevice* m_device = nullptr;
  MetalCommandQueue* m_queue = nullptr;
  MetalRef<MTL::CommandBuffer> m_cmdBuf;
  std::vector<std::shared_ptr<Fence>> m_waitFences;
  std::vector<std::shared_ptr<Fence>> m_signalFences;
};
} // namespace goala
