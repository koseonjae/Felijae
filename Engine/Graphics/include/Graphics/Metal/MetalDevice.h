#pragma once

#include <Graphics/Model/Device.h>
#include <Graphics/Utility/MetalRef.h>

#include <Metal/MTLDevice.hpp>

namespace goala {
class MetalDevice : public Device {
public:
  MetalDevice();

  std::shared_ptr<Pipeline> createPipeline(PipelineDescription desc) override;

  std::shared_ptr<ComputePipeline> createComputePipeline(ComputePipelineDescription desc) override;

  std::shared_ptr<VertexBuffer> createVertexBuffer(VertexBufferDescription desc) override;

  std::shared_ptr<Texture> createTexture(TextureDescription desc) override;

  std::shared_ptr<Fence> createFence(FenceDescription desc) override;

  std::shared_ptr<CommandQueue> createCommandQueue(CommandQueueDescription desc) override;

  std::shared_ptr<Shader> createShader(ShaderDescription desc) override;

  std::shared_ptr<RenderPass> createRenderPass(RenderPassDescription desc) override;

  MTL::Device* getMTLDevice();

  const MTL::Device* getMTLDevice() const;

private:
  MetalRef<MTL::Device> m_device;
};
} // namespace goala
