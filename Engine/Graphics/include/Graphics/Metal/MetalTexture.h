#pragma once

#include <Graphics/Model/Texture.h>
#include <Graphics/Utility/MetalRef.h>

namespace MTL {
class Texture;
}

namespace goala {

class MetalDevice;

class MetalTexture : public Texture {
 public:
  MetalTexture(MetalDevice* device);

  void initialize(File path, bool lazyLoading = false) override;

  void initialize(int width, int height, ImageFormat format, bool lazyLoading = false) override;

  void initialize(ImageData imageData, bool lazyLoading = false) override;

  void bind() override;

  MTL::Texture* getHandle();

  const MTL::Texture* getHandle() const;

 private:
  MetalDevice* m_device = nullptr;
  MetalRef<MTL::Texture> m_texture; // todo: 부모 클래스의 m_handle은 없애고 각 플랫폼별로 관리하는게 나을듯
};

} // namespace goala
