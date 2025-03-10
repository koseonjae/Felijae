#pragma once

#include <Graphics/Model/ComputeCommandEncoder.h>
#include <Graphics/Utility/MetalRef.h>

namespace MTL {
class ComputeCommandEncoder;
}

namespace larco {
class MetalCommandBuffer;
class ComputePipeline;

class MetalComputeCommandEncoder : public ComputeCommandEncoder {
public:
  MetalComputeCommandEncoder(MetalCommandBuffer* commandBuffer, ComputeCommandEncoderDescription description);

  void encodeCompute(ComputePipeline* pipeline) override;

  void endEncoding() override;

private:
  MetalRef<MTL::ComputeCommandEncoder> m_encoder;
};
}
