#pragma once

#include <memory>

namespace goala {
class CommandBuffer;
struct CommandBufferDescription;

struct CommandQueueDescription {};

class CommandQueue {
public:
  virtual ~CommandQueue() = default;

  virtual std::shared_ptr<CommandBuffer> createCommandBuffer(CommandBufferDescription desc) = 0;
};
} // namespace goala
