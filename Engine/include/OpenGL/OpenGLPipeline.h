#pragma once

#include <Model/Pipeline.h>

class OpenGLPipeline : public Pipeline {
 public:
  ~OpenGLPipeline() override = default;
  void bind() override;

 private:
  void _bindRasterizer();
  void _bindCulling();
  void _bindViewport();
  void _bindOutputMerger();
  void _bindDepthTest();
  void _bindAlphaBlending();
};
