#include <Graphics/Metal/MetalCommandBuffer.h>
#include <Graphics/Metal/MetalCommandEncoder.h>
#include <Graphics/Metal/MetalCommandQueue.h>
#include <Graphics/Metal/MetalPipeline.h>
#include <Graphics/Metal/MetalRenderPass.h>
#include <Base/Utility/TypeCast.h>

#include <Metal/Metal.hpp>

using namespace goala;

MetalCommandBuffer::MetalCommandBuffer(MetalDevice* device, MetalCommandQueue* queue, CommandBufferDescription cmdBuf)
  : m_device(device)
  , m_queue(queue)
  , m_cmdBuf(makeMetalRef(queue->getMTLCommandQueue()->commandBuffer())) {}

void MetalCommandBuffer::present(Texture* texture) {
  assert(false && "Only offscreen rendering is supported");
}

void MetalCommandBuffer::commit() {
  m_cmdBuf->commit();
}

std::shared_ptr<CommandEncoder> MetalCommandBuffer::createCommandEncoder(RenderPass* renderPass, CommandEncoderDescription desc) {
  auto metalRenderPass = SAFE_DOWN_CAST(MetalRenderPass*, renderPass);
  auto commandEncoder = std::make_shared<MetalCommandEncoder>(this, metalRenderPass, std::move(desc));
  return commandEncoder;
}

void MetalCommandBuffer::waitUntilCompleted() {
  m_cmdBuf->waitUntilCompleted();
}

void MetalCommandBuffer::waitUntilScheduled() {
  m_cmdBuf->waitUntilScheduled();
}

MTL::CommandBuffer* MetalCommandBuffer::getCommandBuffer() {
  return m_cmdBuf.get();
}

const MTL::CommandBuffer* MetalCommandBuffer::getCommandBuffer() const {
  return m_cmdBuf.get();
}
