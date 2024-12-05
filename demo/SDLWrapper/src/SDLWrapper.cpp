#include <SDLWrapper/SDLWrapper.h>

#include <SDL2/SDL.h>

#include <cassert>

void SDLWrapper::loop() {
  assert(m_updateCallback != nullptr);
  assert(m_renderCallback != nullptr);
  assert(m_blitCopyCallback != nullptr);

  bool quit = false;
  SDL_Event e;

  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      switch (e.type) {
        case SDL_QUIT: {
          quit = true;
          break;
        }
      }
    }

    m_updateCallback();

    m_renderCallback();

    onBlitCopyAndSwapBuffer();
  }
}

void SDLWrapper::setUpdateCallback(std::function<void()> updateCallback) {
  m_updateCallback = std::move(updateCallback);
}

void SDLWrapper::setRenderCallback(std::function<void()> renderCallback) {
  m_renderCallback = std::move(renderCallback);
}

void SDLWrapper::setBlitCopyCallback(std::function<void(void*)> blitCopyCallback) {
  m_blitCopyCallback = std::move(blitCopyCallback);
}
