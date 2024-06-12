#pragma once

#include <glad/glad.h>
#include <string_view>
#include <functional>

class OpenGLTexture : std::enable_shared_from_this<OpenGLTexture> {
 public:
  OpenGLTexture(std::string_view path);
  ~OpenGLTexture();

  OpenGLTexture(const OpenGLTexture&) = delete;
  OpenGLTexture(OpenGLTexture&&) = delete;
  OpenGLTexture& operator=(const OpenGLTexture&) = delete;
  OpenGLTexture& operator=(OpenGLTexture&&) = delete;

  void bind();

 private:
  void _initIfNeeded();

 private:
  bool m_initialized = false;
  GLuint m_textureId;
  std::function<void()> m_initializer;
};
