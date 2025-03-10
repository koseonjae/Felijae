#include <Graphics/Metal/MetalCommandQueue.h>
#include <Graphics/Metal/MetalCommandBuffer.h>
#include <Graphics/Metal/MetalDevice.h>

using namespace larco;

MetalCommandQueue::MetalCommandQueue(MetalDevice* device, CommandQueueDescription desc)
  : m_device(device)
  , m_commandQueue(makeMetalRef(device->getMTLDevice()->newCommandQueue())) {}

std::shared_ptr<CommandBuffer> MetalCommandQueue::createCommandBuffer(CommandBufferDescription desc) {
  auto commandBuffer = std::make_shared<MetalCommandBuffer>(m_device, this, std::move(desc));
  return commandBuffer;
}

MTL::CommandQueue* MetalCommandQueue::getMTLCommandQueue() {
  return m_commandQueue.get();
}

const MTL::CommandQueue* MetalCommandQueue::getMTLCommandQueue() const {
  return m_commandQueue.get();
}
