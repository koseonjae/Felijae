#include <Graphics/Metal/MetalCommandBuffer.h>
#include <Graphics/Metal/MetalCommandEncoder.h>
#include <Graphics/Metal/MetalTexture.h>
#include <Graphics/Metal/MetalPipeline.h>
#include <Graphics/Metal/MetalRenderPass.h>
#include <Graphics/Metal/MetalBuffer.h>

#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalDrawable.hpp>

using namespace goala;

MetalCommandBuffer::MetalCommandBuffer(MTL::CommandBuffer* cmdBuf) {
  m_cmdBuf = MetalRef(cmdBuf);
}

void MetalCommandBuffer::encode(RenderPass* renderPass, Pipeline* pipeline) {
  // todo: dynamic_cast 너무 구리다..
  auto metalRenderPass = dynamic_cast<MetalRenderPass*>(renderPass);

  auto encoder = std::make_shared<MetalCommandEncoder>(m_cmdBuf->renderCommandEncoder(metalRenderPass->getPass()));
  encoder->encode(pipeline);
  encoder->endEncoding();
}

void MetalCommandBuffer::present(Texture* texture) {
  m_cmdBuf->presentDrawable(texture->getHandle<CA::MetalDrawable*>());
}

void MetalCommandBuffer::commit() {
  m_cmdBuf->commit();
}
