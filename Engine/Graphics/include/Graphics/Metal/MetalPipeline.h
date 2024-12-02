#pragma once

#include <Graphics/Model/Pipeline.h>
#include <Graphics/Utility/MetalRef.h>
#include <Metal/MTLDepthStencil.hpp>

namespace MTL {
class RenderPipelineState;
class RenderPipelineDescriptor;
class RenderCommandEncoder;
}

namespace goala {
class MetalDevice;

class MetalPipeline : public Pipeline {
public:
  MetalPipeline(MetalDevice* device, PipelineDescription desc);

  void update() override;

  void render() override;

  MTL::RenderPipelineState* getPipeline();

  const MTL::RenderPipelineState* getPipeline() const;

  void encode(MTL::RenderCommandEncoder* encoder);

private:
  void _initializeDepthTest();
  void _initializeAlphaBlend(MTL::RenderPipelineDescriptor* descriptor);
  void _encodeViewport(MTL::RenderCommandEncoder* encoder);
  void _encodeCulling(MTL::RenderCommandEncoder* encoder);
  void _encodeUniformTextures(MTL::RenderCommandEncoder* encoder);
  void _encodeUniformVariables(MTL::RenderCommandEncoder* encoder);

private:
  MetalDevice* m_device = nullptr;
  MetalRef<MTL::RenderPipelineState> m_pipeline;
  MetalRef<MTL::DepthStencilState> m_depthStencilState;
};
} // namespace goala
