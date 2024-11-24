#pragma once

#include <Graphics/Model/Texture.h>

namespace MTL {
class Texture;
}

namespace goala {

class MetalTexture : public Texture {
 public:
  void initialize(File path, bool lazyLoading) override;

  void initialize(int width, int height, ImageFormat format, bool lazyLoading) override;

  void initialize(ImageData imageData, bool lazyLoading) override;

  void bind() override;
};

} // namespace goala
