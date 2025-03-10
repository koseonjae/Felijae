#pragma once

#include <Graphics/Model/Pipeline.h>

namespace larco {
class OpenGLDevice;
class OpenGLProgram;

class OpenGLPipeline : public Pipeline {
public:
  OpenGLPipeline(OpenGLDevice* device, PipelineDescription desc);
  ~OpenGLPipeline() override = default;

  void render();

private:
  void _bindCulling();
  void _bindViewport();
  void _bindDepthTest();
  void _bindAlphaBlending();
  void _initializeProgram();
  void _initializeVertexBuffer();

private:
  OpenGLDevice* m_device = nullptr;
  std::shared_ptr<OpenGLProgram> m_program;
};
} // namespace larco
