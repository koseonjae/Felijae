#pragma once

#include <Model/OutputMerger.h>

class OpenGLOutputMerger : public OutputMerger {
 public:
  void bind() override;

 private:
  void _bindDepthTest();
  void _bindAlphaBlending();
};
