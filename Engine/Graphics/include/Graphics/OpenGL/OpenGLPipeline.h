#pragma once

#include <Graphics/Model/Pipeline.h>

namespace goala {

class OpenGLPipeline : public Pipeline {
 public:
  ~OpenGLPipeline() override = default;
  void update() override;
  void render() override;

 private:
  void _bindOutputMerger();
  void _bindDepthTest();
  void _bindAlphaBlending();
};

} // namespace goala
