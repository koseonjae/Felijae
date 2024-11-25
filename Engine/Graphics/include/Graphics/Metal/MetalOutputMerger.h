#pragma once

#include <Graphics/Model/OutputMerger.h>

namespace MTL {
class RenderPipelineDescriptor;
class RenderCommandEncoder;
}

namespace goala {

class MetalDevice;

class MetalOutputMerger : public OutputMerger {
 public:
  MetalOutputMerger(MetalDevice* device);
  void bind(void* descriptor) override;
  void encode(CommandEncoder* encoder) override;

 private:
  void _updateAlphaBlend(MTL::RenderPipelineDescriptor* encoder);
  void _updateDepthTest(MTL::RenderCommandEncoder* encoder);

 private:
  MetalDevice* m_device = nullptr;
};

} // namespace goala
