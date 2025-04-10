#pragma once

#include <Graphics/Model/Texture.h>
#include <OpenGL/gl3.h>

#include <functional>
#include <string_view>

namespace larco {
class OpenGLDevice;

class OpenGLTexture final : public Texture, public std::enable_shared_from_this<OpenGLTexture> {
public:
  OpenGLTexture(OpenGLDevice* device, TextureDescription desc);
  ~OpenGLTexture() override;

  void bind() override;

  GLuint getHandle() const {
    return m_handle;
  }

private:
  void _initialize();
  void _initIfNeeded();

private:
  GLuint m_handle = 0;
  bool m_initialized = false;
};
} // namespace larco
