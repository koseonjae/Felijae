#pragma once

#include <functional>
#include <SDL2/SDL_pixels.h>

enum class Graphics {
  OpenGL3, Metal, Vulkan, DX11, DX12, Undefined
};

class SDLWrapper {
public:
  virtual ~SDLWrapper() = default;

  void loop();

  void setUpdateCallback(std::function<void()> updateCallback);

  void setRenderCallback(std::function<void()> renderCallback);

  /**
   *
   * @param blitCopyCallback
   * drawable param
   * opengl : nullptr
   * metal  : CA::MetalDrawable*
   */
  void setBlitCopyCallback(std::function<void(void* drawable)> blitCopyCallback);

  virtual std::tuple<int, int> getDrawableSize() const = 0;

  virtual SDL_PixelFormatEnum getPixelFormat() const = 0;

private:
  virtual void onBlitCopyAndSwapBuffer() = 0;

private:
  std::function<void()> m_updateCallback;
  std::function<void()> m_renderCallback;

protected:
  std::function<void(void*)> m_blitCopyCallback;
};
