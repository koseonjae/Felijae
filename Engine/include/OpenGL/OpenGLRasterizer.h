#pragma once

#include <Model/Rasterizer.h>

class OpenGLRasterizer : public Rasterizer {
 public:
  void bind() override;

 private:
  void _bindCulling();
  void _bindViewport();
};
