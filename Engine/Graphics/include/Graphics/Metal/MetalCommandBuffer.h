#pragma once

#include <Graphics/Model/CommandBuffer.h>
#include <Graphics/Utility/MetalRef.h>

namespace MTL {
class CommandBuffer;
}

namespace goala {

class MetalCommandBuffer : public CommandBuffer {
 public:
  explicit MetalCommandBuffer(MTL::CommandBuffer* cmdBuf);

  void encode(Pipeline* pipeline) override;

  void present(Texture* texture) override;

  void commit() override;

 private:
  MetalRef<MTL::CommandBuffer> m_cmdBuf;
};

} // namespace goala
