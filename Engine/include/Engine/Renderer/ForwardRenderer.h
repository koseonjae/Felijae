#pragma once

#include <Engine/Renderer/Renderer.h>

class ForwardRenderer : public Renderer {
 public:
  void update() override;

  void render() override;
};
