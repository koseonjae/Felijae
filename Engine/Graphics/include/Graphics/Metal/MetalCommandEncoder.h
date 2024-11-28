#pragma once

#include <Graphics/Model/CommandEncoder.h>
#include <Graphics/Utility/MetalRef.h>

namespace MTL {
class RenderCommandEncoder;
}

namespace goala {
class MetalCommandBuffer;
class MetalRenderPass;

class MetalCommandEncoder : public CommandEncoder {
public:
  MetalCommandEncoder(MetalCommandBuffer* commandBuffer, MetalRenderPass* renderPass, CommandEncoderDescription desc);

  // void encode(Pipeline* pipeline) override;
  //
  // void draw() override;
  //
  // void updateDependency(const std::vector<std::shared_ptr<Fence>>& waitFences,
  //                       const std::vector<std::shared_ptr<Fence>>& signalFences) override;

  void endEncoding() override;

  void encodeDraw(Pipeline* pipeline) override;

  void updateDependency(const std::vector<std::shared_ptr<Fence>>& waitFences,
                        const std::vector<std::shared_ptr<Fence>>& signalFences) override;

  MTL::RenderCommandEncoder* getEncoder();

  const MTL::RenderCommandEncoder* getEncoder() const;

private:
  MetalRef<MTL::RenderCommandEncoder> m_encoder;
};
} // namespace goala
