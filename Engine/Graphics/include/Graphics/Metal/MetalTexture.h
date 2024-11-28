#pragma once

#include <Graphics/Model/Texture.h>
#include <Graphics/Utility/MetalRef.h>

#include <functional>

namespace MTL {
class Texture;
}

namespace goala {
class MetalDevice;

class MetalTexture : public Texture, public std::enable_shared_from_this<MetalTexture> {
public:
  MetalTexture(MetalDevice* device, TextureDescription textureDesc);

  void bind() override;

  MTL::Texture* getHandle();

  const MTL::Texture* getHandle() const;

private:
  void _initIfNeeded();

private:
  MetalDevice* m_device = nullptr;
  MetalRef<MTL::Texture> m_texture;
  bool m_initialized = false;
  ImageData m_imageData{};
};
} // namespace goala
