#pragma once

#include <Graphics/Model/Device.h>

namespace goala {
class OpenGLDevice : public Device {
public:
  OpenGLDevice() = default;

  std::shared_ptr<Pipeline> createPipeline(PipelineDescription desc) override;

  std::shared_ptr<ComputePipeline> createComputePipeline(ComputePipelineDescription desc) override;

  std::shared_ptr<Buffer> createBuffer(VertexBufferDescription desc) override;

  std::shared_ptr<Texture> createTexture(TextureDescription desc) override;

  std::shared_ptr<Fence> createFence(FenceDescription desc) override;

  std::shared_ptr<CommandQueue> createCommandQueue(CommandQueueDescription desc) override;

  std::shared_ptr<Shader> createShader(ShaderDescription desc) override;

  std::shared_ptr<RenderPass> createRenderPass(RenderPassDescription desc) override;
};
} // namespace goala
