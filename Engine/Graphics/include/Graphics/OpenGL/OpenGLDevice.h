#pragma once

#include <Graphics/Model/Device.h>

namespace goala {

class OpenGLDevice : public Device {
 public:
  OpenGLDevice() = default;

   std::shared_ptr<Pipeline> createPipeline(PipelineDescription desc) override;

  std::shared_ptr<Buffer> createBuffer(BufferDescription desc) override;

 std::shared_ptr<Texture> createTexture(TextureDescription desc) override;
};

} // namespace goala
