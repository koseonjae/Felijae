#pragma once

#include <Graphics/Model/Pipeline.h>

namespace MTL {
class RenderPipelineState;
class RenderPipelineDescriptor;
}

namespace goala {

class MetalDevice;

class MetalPipeline : public Pipeline {
 public:
  explicit MetalPipeline(MetalDevice* device, const PipelineDescription& description);

  void update() override;

  void render() override;

  MTL::RenderPipelineState* getPipeline();

  const MTL::RenderPipelineState* getPipeline() const;

 private:
  std::shared_ptr<MTL::RenderPipelineState> m_pipeline;
};

} // namespace goala
