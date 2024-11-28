#pragma once

#include <Graphics/Model/CommandQueue.h>
#include <Graphics/Utility/MetalRef.h>
#include <Metal/MTLCommandQueue.hpp>

namespace goala {
class MetalDevice;

class MetalCommandQueue : public CommandQueue {
public:
  MetalCommandQueue(MetalDevice* device, CommandQueueDescription desc);

  std::shared_ptr<CommandBuffer> createCommandBuffer(CommandBufferDescription desc) override;

  MTL::CommandQueue* getMTLCommandQueue();

  const MTL::CommandQueue* getMTLCommandQueue() const;

private:
  MetalDevice* m_device = nullptr;
  MetalRef<MTL::CommandQueue> m_commandQueue;
};
} // namespace goala
