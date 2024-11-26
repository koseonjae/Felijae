#include <Graphics/Metal/MetalDevice.h>
#include <Graphics/Metal/MetalPipeline.h>

using namespace goala;

MetalDevice::MetalDevice(MTL::Device* device) : m_device(makeMetalRef(device)) {}

std::shared_ptr<Pipeline> MetalDevice::createPipeline(const PipelineDescription& desc) {
  auto pipeline = std::make_shared<MetalPipeline>(this, desc);
  return pipeline;
}

MTL::Device* MetalDevice::getMTLDevice() { return m_device.get(); }

const MTL::Device* MetalDevice::getMTLDevice() const { return m_device.get(); }
