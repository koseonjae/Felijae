#pragma once

#include <memory>

namespace larco {
class CommandBuffer;

class Renderer {
public:
  virtual ~Renderer() = default;

  virtual void update() = 0;

  virtual void render(std::shared_ptr<CommandBuffer>& cmdBuf) = 0;
};
} // namespace larco
