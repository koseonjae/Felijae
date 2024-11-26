#include <Graphics/Metal/MetalTexture.h>
#include <Graphics/Utility/ImageFormatUtil.h>
#include <Graphics/Metal/MetalDevice.h>

#include <Metal/Metal.hpp>

#include <cassert>

using namespace goala;

MetalTexture::MetalTexture(MetalDevice* device)
  : m_device(device) {}

void MetalTexture::initialize(File path, bool lazyLoading) {
  assert(false && "To be supported");
}

void MetalTexture::initialize(int width, int height, ImageFormat format, bool lazyLoading) {
  auto desc = MTL::TextureDescriptor::alloc()->init();

  desc->setWidth(width);
  desc->setHeight(height);

  auto metalImageFormat = getMetalImageFormat(format);
  desc->setPixelFormat(metalImageFormat);

  // todo: 일단은 draw하고, blit copy를 위한 src로도 쓰이기 때문에 모두 세팅 -> 이후 parameterize
  // TextureUsageRenderTarget : 렌더패스에 사용되는 목적
  // TextureUsageShaderWrite : Compute Shader or 렌더패스에 사용되지 않는 graphics shader write
  desc->setUsage(MTL::TextureUsageRenderTarget | MTL::TextureUsageShaderWrite | MTL::TextureUsageShaderRead);

  // todo: 일단 blit하려면 GPU에서만 접근하면 되니 Private 설정 -> 이후 parameterize
  // MTLStorageModeShared     : CPU, GPU 모두 접근 가능
  // MTLStorageModePrivate    : GPU only
  // MTLStorageModeMemoryless : CPU, GPU 모두 접근 불가. 렌더링 도중에만 유효한 메몰
  // MTLStorageModeManaged    : CPU와 GPU 간에 자동으로 데이터가 관리
  desc->setStorageMode(MTL::StorageModePrivate);

  // todo: default 쓰레기값이니 이후에 초기화 옵션 파라미터 추가
  auto texture = MetalRef(m_device->getMTLDevice()->newTexture(desc));
  assert(texture.get() && "Failed to create Metal texture");

  m_texture = std::move(texture);
}

void MetalTexture::initialize(ImageData imageData, bool lazyLoading) {
  assert(false && "To be supported");
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
