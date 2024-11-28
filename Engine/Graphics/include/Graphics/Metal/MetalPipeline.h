#pragma once

#include <Graphics/Model/Pipeline.h>
#include <Graphics/Utility/MetalRef.h>

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

private:
  MetalRef<MTL::RenderPipelineState> m_pipeline;
};
} // namespace goala
