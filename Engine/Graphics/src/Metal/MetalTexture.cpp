#include <Graphics/Metal/MetalTexture.h>
#include <Graphics/Utility/ImageFormatUtil.h>
#include <Graphics/Metal/MetalDevice.h>

#include <Metal/Metal.hpp>

#include <cassert>

using namespace goala;

namespace {
MTL::TextureUsage getMTLTextureUsage(TextureUsage usage) {
  MTL::TextureUsage result = MTL::TextureUsageUnknown;
  if ((usage | TextureUsage::READ) == TextureUsage::READ)
    result |= MTL::TextureUsageShaderRead;
  if ((usage | TextureUsage::WRITE) == TextureUsage::WRITE)
    result |= MTL::TextureUsageShaderWrite;
  if ((usage | TextureUsage::RENDER_TARGET) == TextureUsage::RENDER_TARGET)
    result |= MTL::TextureUsageRenderTarget;
  return result;
}
}

MetalTexture::MetalTexture(MetalDevice* device, TextureDescription desc)
  : Texture(std::move(desc))
  , m_device(device) {
  if (desc.loadType == TextureLoadType::LAZY)
    return;
  _initIfNeeded();
}

void MetalTexture::_initIfNeeded() {
  if (m_initialized)
    return;

  auto metalDesc = makeMetalRef(MTL::TextureDescriptor::alloc()->init());

  metalDesc->setWidth(m_desc.imageData.width);
  metalDesc->setHeight(m_desc.imageData.height);

  auto metalImageFormat = getMetalImageFormat(m_desc.imageData.format);
  metalDesc->setPixelFormat(metalImageFormat);

  // todo: usage, storage parameterize
  metalDesc->setUsage(getMTLTextureUsage(m_desc.usage));
  metalDesc->setStorageMode(MTL::StorageModeShared);

  auto texture = makeMetalRef(m_device->getMTLDevice()->newTexture(metalDesc.get()));
  assert(texture.get() && "Failed to create Metal texture");

  if (!m_desc.imageData.pixel.empty()) {
    MTL::Region region = MTL::Region::Make2D(0, 0, m_desc.imageData.width, m_desc.imageData.height);
    texture->replaceRegion(region,
                           0, // mipmap level
                           m_desc.imageData.pixel.data(),
                           m_desc.imageData.width * m_desc.imageData.channels);
    m_desc.imageData.pixel.clear(); // todo: cpu image buffer 유지할지에 대한 옵션 추가
  }
  m_texture = std::move(texture);

  auto samplerDesc = makeMetalRef(MTL::SamplerDescriptor::alloc()->init());
  samplerDesc->setMinFilter(MTL::SamplerMinMagFilterLinear);
  samplerDesc->setMagFilter(MTL::SamplerMinMagFilterLinear);
  samplerDesc->setMipFilter(MTL::SamplerMipFilterLinear);
  samplerDesc->setSAddressMode(MTL::SamplerAddressModeClampToEdge);
  samplerDesc->setTAddressMode(MTL::SamplerAddressModeClampToEdge);
  m_sampler = makeMetalRef(m_device->getMTLDevice()->newSamplerState(samplerDesc.get()));

  m_initialized = true;
}

void MetalTexture::bind() {
  assert(false && "To be supported");
}

MTL::Texture* MetalTexture::getTextureHandle() {
  assert(m_texture.get() && "MetalTexture is not initialized");
  return m_texture.get();
}

const MTL::Texture* MetalTexture::getTextureHandle() const {
  assert(m_texture.get() && "MetalTexture is not initialized");
  return m_texture.get();
}

MTL::SamplerState* MetalTexture::getSamplerHandle() {
  return m_sampler.get();
}

const MTL::SamplerState* MetalTexture::getSamplerHandle() const {
  return m_sampler.get();
}
