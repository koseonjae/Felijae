#include <Graphics/Metal/MetalCommandEncoder.h>
#include <Graphics/Metal/MetalCommandBuffer.h>
#include <Graphics/Metal/MetalPipeline.h>
#include <Graphics/Metal/MetalRenderPass.h>
#include <Graphics/Metal/MetalBuffer.h>
#include <Graphics/Metal/MetalFence.h>

#include <Metal/triangle_types.h>
#include <Metal/Metal.hpp>

using namespace goala;

MetalCommandEncoder::MetalCommandEncoder(CommandBuffer* commandBuffer, RenderPass* renderPass) {
  auto metalCommandBuffer = dynamic_cast<MetalCommandBuffer*>(commandBuffer);
  auto metalRenderPass = dynamic_cast<MetalRenderPass*>(renderPass);
  auto encoder = metalCommandBuffer->getCommandBuffer()->renderCommandEncoder(metalRenderPass->getPass());
  m_encoder = makeMetalRef(encoder);
}

void MetalCommandEncoder::encode(Pipeline* pipeline) {
  auto metalPipeline = dynamic_cast<MetalPipeline*>(pipeline);
  auto metalBuffer = dynamic_cast<MetalBuffer*>(metalPipeline->getBuffer());

  pipeline->getRasterizer()->encode(this);
  pipeline->getOutputMerger()->encode(this);

  m_encoder->setRenderPipelineState(metalPipeline->getPipeline());
  m_encoder->setVertexBuffer(metalBuffer->getVertexHandle(), 0, AAPLVertexInputIndexVertices);
  m_encoder->drawIndexedPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle,
                                   metalBuffer->getIndicesSize(),
                                   MTL::IndexTypeUInt32,
                                   metalBuffer->getIndexHandle(),
                                   0);
  m_encoder->endEncoding();
}

void MetalCommandEncoder::updateDependency(const std::vector<std::shared_ptr<Fence>>& signalFences,
                                           const std::vector<std::shared_ptr<Fence>>& waitFences) {
  for (auto& fence : waitFences) {
    auto metalFence = std::dynamic_pointer_cast<MetalFence>(fence);
    m_encoder->waitForFence(metalFence->getMTLFence(), MTL::RenderStageFragment);
  }

  for (auto& fence : signalFences) {
    auto metalFence = std::dynamic_pointer_cast<MetalFence>(fence);
    m_encoder->updateFence(metalFence->getMTLFence(), MTL::RenderStageFragment);
  }
}

MTL::RenderCommandEncoder* MetalCommandEncoder::getEncoder() {
  return m_encoder.get();
}

const MTL::RenderCommandEncoder* MetalCommandEncoder::getEncoder() const {
  return m_encoder.get();
}
