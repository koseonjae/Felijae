#pragma once

#include "SDLWrapper.h"

#include <SDL2/SDL.h>

namespace MTL {
class Device;
}

namespace CA {
class MetalLayer;
}

class MetalSDLWrapper : public SDLWrapper {
public:
  MetalSDLWrapper(int width, int height, MTL::Device* device);

  ~MetalSDLWrapper() override;

  std::tuple<int, int> getDrawableSize() const override;

  SDL_PixelFormatEnum getPixelFormat() const override;

private:
  void onBlitCopyAndSwapBuffer() override;

private:
  int m_width = 0;
  int m_height = 0;
  SDL_MetalView m_view;
  SDL_Window* m_window = nullptr;
  CA::MetalLayer* m_layer = nullptr;
};
