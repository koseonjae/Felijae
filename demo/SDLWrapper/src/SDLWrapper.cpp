#include <SDLWrapper/SDLWrapper.h>

#include <Foundation/NSAutoreleasePool.hpp>
#include <SDL2/SDL.h>

#include <cassert>

void SDLWrapper::loop() {
  assert(m_updateCallback != nullptr);
  assert(m_renderCallback != nullptr);
  assert(m_blitCopyCallback != nullptr);

  SDL_Event e;

  while (true) {
    while (SDL_PollEvent(&e) != 0) {
      switch (e.type) {
        case SDL_QUIT: {
          return;
        }
      }
    }

    auto pool = NS::AutoreleasePool::alloc()->init();

    m_updateCallback();

    m_renderCallback();

    onBlitCopyAndSwapBuffer();

    pool->release();
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
