#include "OpenGLTexture.h"
#include <Utility/ImageLoader.h>
#include <cassert>

OpenGLTexture::OpenGLTexture(std::string_view path, bool lazyLoading) {
  assert(!m_initializer && "Texture is initialized twice");

  m_initializer = [=]() {
    assert(!m_initialized);

    auto image = ImageLoader::load(path);
    assert(image.format == ImageForat::RGB);

    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.heigth, 0, GL_RGB, GL_UNSIGNED_BYTE, image.pixel.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    m_textureId = textureId;
    m_initialized = true;
  };

  if (!lazyLoading)
    _initIfNeeded();
}

OpenGLTexture::~OpenGLTexture() {
  if (!m_initialized)
    return;
  glDeleteTextures(1, &m_textureId);
}

void OpenGLTexture::_initIfNeeded() {
  if (!m_initialized) {
    if (!m_initializer)
      assert(false && "Texture is not initialized");
    m_initializer();
    m_initializer = {};
  }
}

void OpenGLTexture::bind() {
  _initIfNeeded();
  glBindTexture(GL_TEXTURE_2D, m_textureId);
}
