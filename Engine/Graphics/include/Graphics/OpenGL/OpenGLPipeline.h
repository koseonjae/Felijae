#pragma once

#include <Graphics/Model/Pipeline.h>

namespace goala {
class OpenGLDevice;
class OpenGLProgram;

class OpenGLPipeline : public Pipeline {
public:
  OpenGLPipeline(OpenGLDevice* device, PipelineDescription desc);
  ~OpenGLPipeline() override = default;
  void update() override;
  void render() override;

private:
  void _bindCulling();
  void _bindViewport();
  void _bindDepthTest();
  void _bindAlphaBlending();
  void _initializeProgram();

private:
  std::shared_ptr<OpenGLProgram> m_program;
};
} // namespace goala
