#include <SDLWrapper/MetalSDLWrapper.h>

#include <cassert>

#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalLayer.hpp>

namespace {
SDL_PixelFormatEnum getSDLPixelFormat(MTL::PixelFormat metalFormat) {
  switch (metalFormat) {
    case MTL::PixelFormatRGBA8Unorm:
      return SDL_PIXELFORMAT_RGBA8888;
    case MTL::PixelFormatBGRA8Unorm:
      return SDL_PIXELFORMAT_BGRA8888;
    default:
      assert(false && "format not supported");
  }
}
}

MetalSDLWrapper::MetalSDLWrapper(int width, int height, MTL::Device* device)
  : m_width(width)
  , m_height(height) {
  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "metal");
  SDL_InitSubSystem(SDL_INIT_VIDEO);
  m_window = SDL_CreateWindow("SDL Metal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_METAL);

  int drawableWidth, drawableHeight;
  SDL_Metal_GetDrawableSize(m_window, &drawableWidth, &drawableHeight);
  assert(width == m_width && height == m_height && "Created window drawable size is not same with viewport");

  m_view = SDL_Metal_CreateView(m_window);
  m_layer = static_cast<CA::MetalLayer*>(SDL_Metal_GetLayer(m_view)); // swapchain
  m_layer->setFramebufferOnly(false);
  m_layer->setDevice(device);
  m_layer->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
}

MetalSDLWrapper::~MetalSDLWrapper() {
  SDL_Metal_DestroyView(m_view);
  SDL_DestroyWindow(m_window);
  SDL_Quit();
}

std::tuple<int, int> MetalSDLWrapper::getDrawableSize() const {
  return {m_width, m_height};
}

SDL_PixelFormatEnum MetalSDLWrapper::getPixelFormat() const {
  return getSDLPixelFormat(m_layer->pixelFormat());
}

void MetalSDLWrapper::onBlitCopyAndSwapBuffer() {
  auto drawable = m_layer->nextDrawable();
  m_blitCopyCallback(drawable);
}
