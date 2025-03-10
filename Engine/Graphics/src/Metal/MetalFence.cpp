#include <Graphics/Metal/MetalFence.h>
#include <Graphics/Metal/MetalDevice.h>

#include <Metal/Metal.hpp>

using namespace larco;

MetalFence::MetalFence(MetalDevice* device, FenceDescription desc)
  : m_fence(makeMetalRef(device->getMTLDevice()->newFence())) {}

MTL::Fence* MetalFence::getMTLFence() {
  return m_fence.get();
}

const MTL::Fence* MetalFence::getMTLFence() const {
  return m_fence.get();
}
