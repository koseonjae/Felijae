#pragma once

#include <Graphics/Model/Texture.h>

#include <OpenGL/gl3.h>
#include <string_view>
#include <functional>

class OpenGLTexture final : public Texture, std::enable_shared_from_this<OpenGLTexture> {
 public:
  OpenGLTexture() = default;
  ~OpenGLTexture() override;

  OpenGLTexture(const OpenGLTexture&) = delete;
  OpenGLTexture(OpenGLTexture&&) = delete;
  OpenGLTexture& operator=(const OpenGLTexture&) = delete;
  OpenGLTexture& operator=(OpenGLTexture&&) = delete;

  void initialize(File path, bool lazyLoading = false) override;

  void initialize(int width, int height, ImageFormat format, bool lazyLoading = false) override;

  void initialize(ImageData imageData, bool lazyLoading = false) override;

  void bind() override;

  uint32_t getHandle() const override;

 private:
  void _initIfNeeded();

 private:
  bool m_initialized = false;
  GLuint m_textureId = 0;
  std::function<void()> m_initializer;
};
