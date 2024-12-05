#pragma once

#include <functional>
#include <SDL2/SDL_pixels.h>

enum class Graphics {
  OpenGL3, Metal, Vulkan, DX11, DX12, Undefined
};

class SDLWrapper {
public:
  explicit SDLWrapper(Graphics graphics)
    : m_graphics(graphics) {}

  virtual ~SDLWrapper() = default;

  void loop();

  void setUpdateCallback(std::function<void()> updateCallback);

  void setRenderCallback(std::function<void()> renderCallback);

  void setBlitCopyCallback(std::function<void(void*)> blitCopyCallback);

  virtual std::tuple<int, int> getDrawableSize() const = 0;

  virtual SDL_PixelFormatEnum getPixelFormat() const = 0;

private:
  virtual void onBlitCopyAndSwapBuffer() = 0;

private:
  Graphics m_graphics = Graphics::Undefined;
  std::function<void()> m_updateCallback;
  std::function<void()> m_renderCallback;

protected:
  std::function<void(void*)> m_blitCopyCallback;
};
