#pragma once

namespace goala {
class RenderPass;
class Pipeline;
class Texture;

struct CommandBufferDescription {};

class CommandBuffer {
public:
  virtual ~CommandBuffer() = default;

  virtual void encode(RenderPass* renderPass, Pipeline* pipeline) = 0;

  virtual void present(Texture* texture) = 0;

  virtual void commit() = 0;

  virtual void addDependency(CommandBuffer* before) = 0;
};
} // namespace goala
