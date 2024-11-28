#pragma once

#include <Graphics/Model/CommandEncoder.h>
#include <Graphics/Utility/MetalRef.h>

namespace MTL {
class RenderCommandEncoder;
}

namespace goala {
class CommandBuffer;
class RenderPass;

class MetalCommandEncoder : public CommandEncoder {
public:
  MetalCommandEncoder(CommandBuffer* commandBuffer, RenderPass* renderPass);

  void encode(Pipeline* pipeline) override;

  void updateDependency(const std::vector<std::shared_ptr<Fence>>& waitFences,
                        const std::vector<std::shared_ptr<Fence>>& signalFences) override;

  MTL::RenderCommandEncoder* getEncoder();

  const MTL::RenderCommandEncoder* getEncoder() const;

private:
  MetalRef<MTL::RenderCommandEncoder> m_encoder;
};
} // namespace goala
