#pragma once

#include <Model/Texture.h>

#include <glad/glad.h>
#include <string_view>
#include <functional>

class OpenGLTexture final : public Texture, std::enable_shared_from_this<OpenGLTexture> {
 public:
  OpenGLTexture(std::string_view path, bool lazyLoading = false);
  ~OpenGLTexture();

  OpenGLTexture(const OpenGLTexture&) = delete;
  OpenGLTexture(OpenGLTexture&&) = delete;
  OpenGLTexture& operator=(const OpenGLTexture&) = delete;
  OpenGLTexture& operator=(OpenGLTexture&&) = delete;

  void bind() override;

 private:
  void _initIfNeeded();

 private:
  bool m_initialized = false;
  GLuint m_textureId;
  std::function<void()> m_initializer;
};
