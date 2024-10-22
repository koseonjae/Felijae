#pragma once

#include <Model/Pipeline.h>

class OpenGLPipeline : public Pipeline {
 public:
  void bind() override;

 private:
  void _bindRasterizer();
  void _bindCulling();
  void _bindViewport();
  void _bindOutputMerger();
};
