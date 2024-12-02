#pragma once

#include <Graphics/Model/Pipeline.h>
#include <Graphics/Utility/MetalRef.h>
#include <Metal/MTLDepthStencil.hpp>

namespace MTL {
class RenderPipelineState;
class RenderPipelineDescriptor;
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

  const MTL::DepthStencilState* getDepthStencilState() const;

private:
  void _initializeDepthTest(const DepthTest& desc);

private:
  MetalDevice* m_device = nullptr;
  MetalRef<MTL::RenderPipelineState> m_pipeline;
  MetalRef<MTL::DepthStencilState> m_depthStencilState;
};
} // namespace goala
