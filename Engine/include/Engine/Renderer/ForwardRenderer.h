#pragma once

#include <Engine/Renderer/GraphicsRenderer.h>

namespace goala {
class ForwardRenderer : public GraphicsRenderer {
public:
  void update() override;

  void render(std::shared_ptr<CommandBuffer> cmdBuf) override;
};
} // namespace goala
