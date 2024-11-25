#pragma once

#include <Engine/Renderer/Renderer.h>

namespace goala {

class ForwardRenderer : public Renderer {
 public:
  void update() override;

  void render(std::shared_ptr<CommandBuffer> cmdBuf) override;
};

} // namespace goala
