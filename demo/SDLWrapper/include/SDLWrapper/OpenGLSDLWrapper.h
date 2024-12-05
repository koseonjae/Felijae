#pragma once

#include "SDLWrapper.h"

#include <SDL2/SDL.h>

class OpenGLSDLWrapper : public SDLWrapper {
public:
  OpenGLSDLWrapper(int width, int height);

  ~OpenGLSDLWrapper() override;

  std::tuple<int, int> getDrawableSize() const override;

  SDL_PixelFormatEnum getPixelFormat() const override;

private:
  void onBlitCopyAndSwapBuffer() override;

private:
  SDL_Window* m_window = nullptr;
  SDL_GLContext m_context;
  int m_width = 0;
  int m_height = 0;
};
