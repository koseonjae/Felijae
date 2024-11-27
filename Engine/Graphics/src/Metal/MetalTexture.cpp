#include <Graphics/Metal/MetalTexture.h>
#include <Graphics/Utility/ImageFormatUtil.h>
#include <Graphics/Metal/MetalDevice.h>

#include <Metal/Metal.hpp>

#include <cassert>

using namespace goala;

MetalTexture::MetalTexture(MetalDevice* device, TextureDescription desc)
  : m_device(device), m_imageData(std::move(desc.imageData)) {
  if (desc.loadType == TextureLoadType::LAZY)
    return;
  _initIfNeeded();
}

void MetalTexture::_initIfNeeded() {
  if (m_initialized)
    return;

  auto metalDesc = MTL::TextureDescriptor::alloc()->init();

  metalDesc->setWidth(m_imageData.width);
  metalDesc->setHeight(m_imageData.height);

  auto metalImageFormat = getMetalImageFormat(m_imageData.format);
  metalDesc->setPixelFormat(metalImageFormat);

  // todo: 일단은 draw하고, blit copy를 위한 src로도 쓰이기 때문에 모두 세팅 -> 이후 parameterize
  // TextureUsageRenderTarget : 렌더패스에 사용되는 목적
  // TextureUsageShaderWrite : Compute Shader or 렌더패스에 사용되지 않는 graphics shader write
  metalDesc->setUsage(MTL::TextureUsageRenderTarget | MTL::TextureUsageShaderWrite | MTL::TextureUsageShaderRead);

  // todo: 일단 blit하려면 GPU에서만 접근하면 되니 Private 설정 -> 이후 parameterize
  // MTLStorageModeShared     : CPU, GPU 모두 접근 가능
  // MTLStorageModePrivate    : GPU only
  // MTLStorageModeMemoryless : CPU, GPU 모두 접근 불가. 렌더링 도중에만 유효한 메몰
  // MTLStorageModeManaged    : CPU와 GPU 간에 자동으로 데이터가 관리
  metalDesc->setStorageMode(MTL::StorageModePrivate);

  // todo: default 쓰레기값이니 이후에 초기화 옵션 파라미터 추가
  auto texture = makeMetalRef(m_device->getMTLDevice()->newTexture(metalDesc));
  assert(texture.get() && "Failed to create Metal texture");

  m_texture = std::move(texture);
  m_imageData.pixel.clear(); // todo: cpu image buffer 유지할지에 대한 옵션 추가
  m_initialized = true;
}

void MetalTexture::bind() {
  assert(false && "To be supported");
}

MTL::Texture* MetalTexture::getHandle() {
  assert(m_texture.get() && "MetalTexture is not initialized");
  return m_texture.get();
}

const MTL::Texture* MetalTexture::getHandle() const {
  assert(m_texture.get() && "MetalTexture is not initialized");
  return m_texture.get();
}
