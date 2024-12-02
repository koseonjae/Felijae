#include <Graphics/Metal/MetalCommandEncoder.h>
#include <Graphics/Metal/MetalCommandBuffer.h>
#include <Graphics/Metal/MetalPipeline.h>
#include <Graphics/Metal/MetalRenderPass.h>
#include <Graphics/Metal/MetalBuffer.h>
#include <Base/Utility/TypeCast.h>

#include <Metal/triangle_types.h>
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
  auto metalBuffer = SAFE_DOWN_CAST(MetalBuffer*, metalPipeline->getBuffer());

  pipeline->getRasterizer()->encode(this);

  m_encoder->setDepthStencilState(metalPipeline->getDepthStencilState());
  m_encoder->setRenderPipelineState(metalPipeline->getPipeline());
  m_encoder->setVertexBuffer(metalBuffer->getVertexHandle(), 0, AAPLVertexInputIndexVertices);
  m_encoder->drawIndexedPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle,
                                   metalBuffer->getIndicesSize(),
                                   MTL::IndexTypeUInt32,
                                   metalBuffer->getIndexHandle(),
                                   0);
}

MTL::RenderCommandEncoder* MetalCommandEncoder::getEncoder() {
  return m_encoder.get();
}

const MTL::RenderCommandEncoder* MetalCommandEncoder::getEncoder() const {
  return m_encoder.get();
}
