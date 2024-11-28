#include <Graphics/Metal/MetalDevice.h>
#include <Graphics/Metal/MetalCommandQueue.h>
#include <Graphics/Metal/MetalPipeline.h>
#include <Graphics/Metal/MetalBuffer.h>
#include <Graphics/Metal/MetalFence.h>

#include "Graphics/Metal/MetalTexture.h"

using namespace goala;

MetalDevice::MetalDevice(): m_device(makeMetalRef(MTL::CreateSystemDefaultDevice())) {}

std::shared_ptr<Pipeline> MetalDevice::createPipeline(PipelineDescription desc) {
  auto pipeline = std::make_shared<MetalPipeline>(this, std::move(desc));
  return pipeline;
}

std::shared_ptr<Buffer> MetalDevice::createBuffer(BufferDescription desc) {
  auto buffer = std::make_shared<MetalBuffer>(this, std::move(desc));
  return buffer;
}

std::shared_ptr<Texture> MetalDevice::createTexture(TextureDescription desc) {
  auto texture = std::make_shared<MetalTexture>(this, std::move(desc));
  return texture;
}

std::shared_ptr<Fence> MetalDevice::createFence(FenceDescription desc) {
  auto fence = std::make_shared<MetalFence>(this, std::move(desc));
  return fence;
}

std::shared_ptr<CommandQueue> MetalDevice::createCommandQueue(CommandQueueDescription desc) {
  auto commandQueue = std::make_shared<MetalCommandQueue>(this, std::move(desc));
  return commandQueue;
}

MTL::Device* MetalDevice::getMTLDevice() { return m_device.get(); }

const MTL::Device* MetalDevice::getMTLDevice() const { return m_device.get(); }
