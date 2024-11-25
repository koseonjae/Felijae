#pragma once

#include <Graphics/Model/CommandEncoder.h>
#include <Graphics/Utility/MetalRef.h>

namespace MTL {
class RenderCommandEncoder;
}

namespace goala {

class MetalCommandEncoder : public CommandEncoder {
 public:
  explicit MetalCommandEncoder(MTL::RenderCommandEncoder* encoder);

   void encode(Pipeline* pipeline) override;

  MTL::RenderCommandEncoder* getEncoder();

  const MTL::RenderCommandEncoder* getEncoder() const;

 private:
  MetalRef<MTL::RenderCommandEncoder> m_encoder;
};

} // namespace goala
