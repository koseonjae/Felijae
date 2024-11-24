#pragma one

#include <Graphics/Model/Pipeline.h>
#include <Graphics/Utility/MetalRef.h>

namespace MTL {
class RenderPipelineState;
}

namespace goala {

class MetalDevice;

class MetalPipeline : public Pipeline {
 public:
  explicit MetalPipeline(MetalDevice* device);

  void initialize(const PipelineDescription& description);

  void update() override;

  void render() override;

  MTL::RenderPipelineState* getPipeline() const;

 private:
  MetalDevice* m_device = nullptr;
  MetalRef<MTL::RenderPipelineState> m_pipeline;
};

} // namespace goala
