#pragma once

#include <Graphics/Model/Pipeline.h>

namespace goala {
class OpenGLDevice;

class OpenGLPipeline : public Pipeline {
public:
  OpenGLPipeline(OpenGLDevice* device, PipelineDescription desc);
  ~OpenGLPipeline() override = default;
  void update() override;
  void render() override;
};
} // namespace goala
