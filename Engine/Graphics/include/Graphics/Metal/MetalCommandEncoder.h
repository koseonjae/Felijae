#pragma once

#include <Graphics/Model/CommandEncoder.h>
#include <Graphics/Utility/MetalRef.h>

namespace MTL {
class RenderCommandEncoder;
class Buffer;
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

  MTL::RenderCommandEncoder* getEncoder();

  const MTL::RenderCommandEncoder* getEncoder() const;

  void setBufferTemp(MetalRef<MTL::Buffer> buffer, int offset, int index);

private:
  MetalRef<MTL::RenderCommandEncoder> m_encoder;
  std::unordered_map<int, MetalRef<MTL::Buffer>> m_mtlUniformBlocks;
};
} // namespace goala
