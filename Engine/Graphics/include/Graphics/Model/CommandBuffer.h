#pragma once

#include <memory>

namespace goala {
class RenderPass;
class Pipeline;
class Texture;
class CommandEncoder;

struct CommandEncoderDescription;

struct CommandBufferDescription {};

class CommandBuffer {
public:
  virtual ~CommandBuffer() = default;

  virtual void present(Texture* texture) = 0;

  virtual void commit() = 0;

  virtual std::shared_ptr<CommandEncoder> createCommandEncoder(RenderPass* renderPass, CommandEncoderDescription desc) = 0;

  virtual void waitUntilScheduled() = 0;

  virtual void waitUntilCompleted() = 0;
};
} // namespace goala
