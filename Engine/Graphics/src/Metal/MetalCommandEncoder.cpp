#include <Graphics/Metal/MetalCommandEncoder.h>
#include <Graphics/Metal/MetalCommandBuffer.h>
#include <Graphics/Metal/MetalRenderPass.h>
#include <Base/Utility/TypeCast.h>
#include <Graphics/Metal/MetalPipeline.h>

#include <Metal/Metal.hpp>

using namespace goala;

MetalCommandEncoder::MetalCommandEncoder(MetalCommandBuffer* commandBuffer, MetalRenderPass* renderPass, CommandEncoderDescription desc) {
  auto encoder = commandBuffer->getCommandBuffer()->renderCommandEncoder(renderPass->getPass());
  m_encoder = makeMetalRef(encoder);
}

void MetalCommandEncoder::endEncoding() {
  m_encoder->endEncoding();
}

void MetalCommandEncoder::encodeDraw(Pipeline* pipeline) {
  auto metalPipeline = SAFE_DOWN_CAST(MetalPipeline*, pipeline);
  metalPipeline->encode(m_encoder.get());
}

MTL::RenderCommandEncoder* MetalCommandEncoder::getEncoder() {
  return m_encoder.get();
}

const MTL::RenderCommandEncoder* MetalCommandEncoder::getEncoder() const {
  return m_encoder.get();
}
