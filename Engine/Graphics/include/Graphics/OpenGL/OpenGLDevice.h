#pragma once

#include <Graphics/Model/Device.h>

namespace goala {

class OpenGLDevice : public Device {
 public:
  OpenGLDevice() = default;

   std::shared_ptr<Pipeline> createPipeline(const PipelineDescription& desc) override;

  std::shared_ptr<Buffer> createBuffer(const BufferDescription& desc) override;

 std::shared_ptr<Texture> createTexture(TextureDescription desc) override;
};

} // namespace goala
