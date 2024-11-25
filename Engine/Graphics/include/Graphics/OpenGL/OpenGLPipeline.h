#pragma once

#include <Graphics/Model/Pipeline.h>

namespace goala {

class OpenGLPipeline : public Pipeline {
 public:
  ~OpenGLPipeline() override = default;
  void update() override;
  void render() override;
};

} // namespace goala
