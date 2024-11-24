#pragma once

#include <Graphics/Model/RenderPass.h>
#include <Graphics/Utility/MetalRef.h>

namespace MTL {
class RenderPassDescriptor;
}

namespace goala {

class MetalRenderPass : public RenderPass {
 public:
  MetalRenderPass();

  void bind() override;

 MTL::RenderPassDescriptor* getPass() const;

 private:
  void _updateRenderPass();

 private:
  MetalRef<MTL::RenderPassDescriptor> m_pass;
};

} // namespace goala
