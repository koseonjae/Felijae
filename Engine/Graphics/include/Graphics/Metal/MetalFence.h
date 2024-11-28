#pragma once

#include <Graphics/Model/Fence.h>
#include <Graphics/Utility/MetalRef.h>

namespace MTL {
class Fence;
}

namespace goala {
class MetalDevice;

class MetalFence : public Fence {
public:
  MetalFence(MetalDevice* device, FenceDescription fenceDesc);

  MTL::Fence* getMTLFence();

  const MTL::Fence* getMTLFence() const;

private:
  MetalRef<MTL::Fence> m_fence;
};
} // namespace goala
