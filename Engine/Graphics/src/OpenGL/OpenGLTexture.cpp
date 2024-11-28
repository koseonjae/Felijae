#include <Base/Utility/ImageLoader.h>
#include <Graphics/OpenGL/OpenGLTexture.h>
#include <Graphics/OpenGL/OpenGLUtils.h>

#include <cassert>

using namespace goala;

OpenGLTexture::OpenGLTexture(OpenGLDevice* device, TextureDescription desc)
  : m_imageData(std::move(desc.imageData)) {
  if (desc.loadType == TextureLoadType::LAZY)
    return;
  _initialize();
}

OpenGLTexture::~OpenGLTexture() {
  if (!m_initialized)
    return;
  auto handle = getHandle();
  glDeleteTextures(1, &handle);
}

void OpenGLTexture::_initIfNeeded() {
  if (m_initialized)
    return;
  _initialize();
}

void OpenGLTexture::_initialize() {
  GLuint format = getGLFormat(m_imageData.format);
  auto data = m_imageData.pixel.empty() ? nullptr : m_imageData.pixel.data();

  GLuint textureId = 0;
  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);
  glTexImage2D(GL_TEXTURE_2D, 0, format, m_imageData.width, m_imageData.height, 0, format, GL_UNSIGNED_BYTE, data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  m_handle = textureId;

  m_imageData.pixel.clear(); // todo: Add option to keep cpu memory

  m_initialized = true;
}

void OpenGLTexture::bind() {
  _initIfNeeded();
  glBindTexture(GL_TEXTURE_2D, m_handle);
  static_assert(std::is_same<GLuint, uint32_t>::value, "GLuint type is not same with uint32_t");
}
