#pragma once

#include <memory>

namespace goala {
class VertexBuffer;
class Fence;
class Pipeline;
class ComputePipeline;
class Texture;
class CommandQueue;
class Shader;
class RenderPass;

struct VertexBufferDescription;
struct FenceDescription;
struct PipelineDescription;
struct ComputePipelineDescription;
struct TextureDescription;
struct CommandQueueDescription;
struct ShaderDescription;
struct RenderPassDescription;

class Device {
public:
  virtual ~Device() = default;

  virtual std::shared_ptr<Pipeline> createPipeline(PipelineDescription desc) = 0;

  virtual std::shared_ptr<ComputePipeline> createComputePipeline(ComputePipelineDescription desc) = 0;

  virtual std::shared_ptr<VertexBuffer> createVertexBuffer(VertexBufferDescription desc) = 0;

  virtual std::shared_ptr<Texture> createTexture(TextureDescription desc) = 0;

  virtual std::shared_ptr<Fence> createFence(FenceDescription desc) = 0;

  virtual std::shared_ptr<CommandQueue> createCommandQueue(CommandQueueDescription desc) = 0;

  virtual std::shared_ptr<Shader> createShader(ShaderDescription desc) = 0;

  virtual std::shared_ptr<RenderPass> createRenderPass(RenderPassDescription desc) = 0;
};
} // namespace goala
