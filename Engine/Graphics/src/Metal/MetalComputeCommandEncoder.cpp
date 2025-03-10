#include <Graphics/Metal/MetalComputeCommandEncoder.h>
#include <Metal/MTLCommandBuffer.hpp>
#include <Metal/MTLComputeCommandEncoder.hpp>

#include "Base/Utility/TypeCast.h"
#include "Graphics/Metal/MetalCommandBuffer.h"
#include "Graphics/Metal/MetalComputePipeline.h"

using namespace larco;

MetalComputeCommandEncoder::MetalComputeCommandEncoder(MetalCommandBuffer* commandBuffer, ComputeCommandEncoderDescription description) {
  m_encoder = makeMetalRef(commandBuffer->getCommandBuffer()->computeCommandEncoder());
}

void MetalComputeCommandEncoder::encodeCompute(ComputePipeline* pipeline) {
  auto mtlComputePipeline = SAFE_DOWN_CAST(MetalComputePipeline*, pipeline);
  mtlComputePipeline->encode(m_encoder.get());
}

void MetalComputeCommandEncoder::endEncoding() {
  m_encoder->endEncoding();
}
