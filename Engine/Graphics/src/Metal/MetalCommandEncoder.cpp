#include <Graphics/Metal/MetalCommandEncoder.h>
#include <Graphics/Metal/MetalPipeline.h>
#include <Graphics/Metal/MetalBuffer.h>

#include <Metal/triangle_types.h>
#include <Metal/Metal.hpp>

using namespace goala;

MetalCommandEncoder::MetalCommandEncoder(MTL::RenderCommandEncoder* encoder)
  : m_encoder(encoder) {}

void MetalCommandEncoder::encode(Pipeline* pipeline) {
  auto metalPipeline = dynamic_cast<MetalPipeline*>(pipeline);
  auto metalBuffer = dynamic_cast<MetalBuffer*>(metalPipeline->getBuffer());

  pipeline->getRasterizer()->encode(this);

  m_encoder->setRenderPipelineState(metalPipeline->getPipeline());
  m_encoder->setVertexBuffer(metalBuffer->getVertexHandle(), 0, AAPLVertexInputIndexVertices);
  m_encoder->drawIndexedPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle,
                                   metalBuffer->getIndicesSize(),
                                   MTL::IndexTypeUInt32,
                                   metalBuffer->getIndexHandle(),
                                   0);
}

void MetalCommandEncoder::endEncoding() {
  m_encoder->endEncoding();
}

MTL::RenderCommandEncoder* MetalCommandEncoder::getEncoder() {
  return m_encoder.get();
}

const MTL::RenderCommandEncoder* MetalCommandEncoder::getEncoder() const {
  return m_encoder.get();
}
