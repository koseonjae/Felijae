#pragma once

#include <Graphics/Model/Texture.h>

namespace MTL {
class Texture;
}

namespace goala {

using TextureType = std::variant<uint32_t, void*>;

class MetalTexture : public Texture {
 public:
  void initialize(File path, bool lazyLoading) override;

  void initialize(int width, int height, ImageFormat format, bool lazyLoading) override;

  void initialize(ImageData imageData, bool lazyLoading) override;

  void initialize(void* externalHandle) override;

  void bind() override;

 private:
  MTL::Texture* m_externalHandle = nullptr;
  bool m_externalHandleInitialized = false;
};

} // namespace goala
