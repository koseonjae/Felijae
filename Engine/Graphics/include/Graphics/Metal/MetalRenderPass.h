#pragma once

#include <Graphics/Model/RenderPass.h>
#include <Graphics/Utility/MetalRef.h>

namespace MTL {
class RenderPassDescriptor;
}

namespace goala {
class MetalDevice;

class MetalRenderPass : public RenderPass {
public:
  MetalRenderPass(MetalDevice* device, RenderPassDescription desc);

  MTL::RenderPassDescriptor* getPass();
  const MTL::RenderPassDescriptor* getPass() const;

private:
  MetalDevice* m_device = nullptr;
  MetalRef<MTL::RenderPassDescriptor> m_pass;
};
} // namespace goala
