#pragma once

#include <Graphics/Model/Texture.h>
#include <Graphics/Utility/MetalRef.h>

#include <functional>

namespace MTL {
class Texture;
class SamplerState;
}

namespace goala {
class MetalDevice;

class MetalTexture : public Texture, public std::enable_shared_from_this<MetalTexture> {
public:
  MetalTexture(MetalDevice* device, TextureDescription textureDesc);

  void bind() override;

  MTL::Texture* getTextureHandle();

  const MTL::Texture* getTextureHandle() const;

  MTL::SamplerState* getSamplerHandle();
  const MTL::SamplerState* getSamplerHandle() const;

private:
  void _initIfNeeded();

private:
  MetalDevice* m_device = nullptr;
  MetalRef<MTL::Texture> m_texture;
  MetalRef<MTL::SamplerState> m_sampler;
  bool m_initialized = false;
};
} // namespace goala
