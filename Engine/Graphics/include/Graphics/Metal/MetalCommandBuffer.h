#pragma once

#include <Graphics/Model/CommandBuffer.h>
#include <Graphics/Utility/MetalRef.h>

#include <memory>
#include <vector>

namespace MTL {
class CommandBuffer;
class Fence;
}

namespace goala {

class MetalDevice;
class Fence;

class MetalCommandBuffer : public CommandBuffer {
 public:
  explicit MetalCommandBuffer(MetalDevice* device, MTL::CommandBuffer* cmdBuf);

  void encode(RenderPass* renderPass, Pipeline* pipeline) override;

  void present(Texture* texture) override;

  void commit() override;

  void addDependency(CommandBuffer* before) override;

 private:
  void _addSignalFence(std::shared_ptr<Fence> fence);
  void _addWaitFence(std::shared_ptr<Fence> fence);

 private:
  MetalDevice* m_device = nullptr;
  MetalRef<MTL::CommandBuffer> m_cmdBuf;
  std::vector<std::shared_ptr<Fence>> m_waitFences;
  std::vector<std::shared_ptr<Fence>> m_signalFences;
};

} // namespace goala
