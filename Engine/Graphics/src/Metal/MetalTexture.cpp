#include <Graphics/Metal/MetalTexture.h>

#include <Metal/Metal.hpp>

#include <cassert>

using namespace goala;

void MetalTexture::initialize(File path, bool lazyLoading) {
  assert(false && "To be supported");
}

void MetalTexture::initialize(int width, int height, ImageFormat format, bool lazyLoading) {
  assert(false && "To be supported");
}

void MetalTexture::initialize(ImageData imageData, bool lazyLoading) {
  assert(false && "To be supported");
}

void MetalTexture::initialize(void* externalHandle) {
  m_externalHandle = static_cast<MTL::Texture*>(externalHandle);
  m_externalHandleInitialized;
}

void MetalTexture::bind() {
  assert(false && "To be supported");
}
