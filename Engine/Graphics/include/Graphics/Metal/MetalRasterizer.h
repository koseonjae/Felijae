#pragma once

#include <Graphics/Model/Rasterizer.h>

namespace MTL {
class RenderPipelineDescriptor;
}

namespace goala {

class MetalRasterizer : public Rasterizer {
 public:
  void bind(void* descriptor) override;
  void encode(CommandEncoder* encoder) override;

 private:
  void _updateCulling(MTL::RenderPipelineDescriptor* descriptor);
  void _updateViewPort(MTL::RenderPipelineDescriptor* descriptor);
};

} // namespace goala
