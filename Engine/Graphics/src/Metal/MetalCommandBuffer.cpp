#include <Graphics/Metal/MetalCommandBuffer.h>
#include <Graphics/Metal/MetalTexture.h>
#include <Graphics/Metal/MetalPipeline.h>
#include <Graphics/Metal/MetalRenderPass.h>
#include <Graphics/Metal/MetalBuffer.h>

#include <Metal/triangle_types.h>
#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalDrawable.hpp>

using namespace goala;

MetalCommandBuffer::MetalCommandBuffer(MTL::CommandBuffer* cmdBuf) {
  m_cmdBuf = MetalRef(cmdBuf);
}

void MetalCommandBuffer::encode(RenderPass* renderPass, Pipeline* pipeline) {
  // todo: dynamic_cast 너무 구리다..
  auto metalPipeline = dynamic_cast<MetalPipeline*>(pipeline);
  auto metalRenderPass = dynamic_cast<MetalRenderPass*>(renderPass);
  auto metalBuffer = dynamic_cast<MetalBuffer*>(metalPipeline->getBuffer());

  auto encoder = m_cmdBuf->renderCommandEncoder(metalRenderPass->getPass());
  encoder->setRenderPipelineState(metalPipeline->getPipeline());
  encoder->setVertexBuffer(metalBuffer->getVertexHandle(), 0, AAPLVertexInputIndexVertices);
  encoder->drawIndexedPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle,
                                 metalBuffer->getIndicesSize(),
                                 MTL::IndexTypeUInt32,
                                 metalBuffer->getIndexHandle(),
                                 0);
  encoder->endEncoding();
}

void MetalCommandBuffer::present(Texture* texture) {
  m_cmdBuf->presentDrawable(texture->getHandle<CA::MetalDrawable*>());
}

void MetalCommandBuffer::commit() {
  m_cmdBuf->commit();
}
