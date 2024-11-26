#include <Graphics/Metal/MetalFence.h>

#include <Metal/Metal.hpp>

using namespace goala;

MetalFence::MetalFence(MTL::Fence* fence)
  : m_fence(fence) {}

MTL::Fence* MetalFence::getMTLFence() {
  return m_fence.get();
}

const MTL::Fence* MetalFence::getMTLFence() const {
  return m_fence.get();
}