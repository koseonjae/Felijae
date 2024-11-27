#include <Graphics/Metal/MetalDevice.h>
#include <Graphics/Metal/MetalPipeline.h>
#include <Graphics/Metal/MetalBuffer.h>

#include "Graphics/Metal/MetalTexture.h"

using namespace goala;

MetalDevice::MetalDevice(MTL::Device* device) : m_device(makeMetalRef(device)) {}

std::shared_ptr<Pipeline> MetalDevice::createPipeline(const PipelineDescription& desc) {
  auto pipeline = std::make_shared<MetalPipeline>(this, desc);
  return pipeline;
}

std::shared_ptr<Buffer> MetalDevice::createBuffer(const BufferDescription& desc) {
  auto buffer = std::make_shared<MetalBuffer>(this, desc);
  return buffer;
}

std::shared_ptr<Texture> MetalDevice::createTexture(TextureDescription desc) {
  auto texture = std::make_shared<MetalTexture>(this, std::move(desc));
  return texture;
}

MTL::Device* MetalDevice::getMTLDevice() { return m_device.get(); }

const MTL::Device* MetalDevice::getMTLDevice() const { return m_device.get(); }
