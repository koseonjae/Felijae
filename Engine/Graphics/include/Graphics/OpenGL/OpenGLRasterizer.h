#pragma once

#include <Graphics/Model/Rasterizer.h>

namespace goala {
class OpenGLRasterizer : public Rasterizer {
public:
  void bind(void* descriptor) override;

private:
  void _bindCulling();
  void _bindViewport();
};
} // namespace goala
