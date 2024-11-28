#pragma once

#include <Graphics/Model/Rasterizer.h>

namespace MTL {
class RenderPipelineDescriptor;
class RenderCommandEncoder;
}

namespace goala {
class MetalRasterizer : public Rasterizer {
public:
  void bind(void* descriptor) override;
  void encode(CommandEncoder* encoder) override;

private:
  void _updateCulling(MTL::RenderCommandEncoder* encoder);
  void _updateViewPort(MTL::RenderCommandEncoder* r);
};
} // namespace goala
