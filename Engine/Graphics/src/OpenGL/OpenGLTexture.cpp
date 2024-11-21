#include <Graphics/OpenGL/OpenGLTexture.h>
#include <Graphics/OpenGL/OpenGLUtils.h>
#include <Base/Utility/ImageLoader.h>

#include <cassert>

OpenGLTexture::~OpenGLTexture() {
  if (!m_initialized)
    return;
  glDeleteTextures(1, &m_textureId);
}

void OpenGLTexture::_initIfNeeded() {
  if (m_initialized)
    return;
  if (!m_initializer)
    assert(false && "Texture is not initialized");
  m_initializer();
  m_initializer = {};
}

void OpenGLTexture::initialize(File path, bool lazyLoading) {
  // todo: imageData쪽에 path를 넣어서 나중에 로딩되도록 할 수 있게 해야하나?
  auto image = ImageLoader::load(path);
  assert(image.format == ImageFormat::RGB);
  initialize(image, lazyLoading);
}

void OpenGLTexture::initialize(int width, int height, ImageFormat format, bool lazyLoading) {
  ImageData image{
      .pixel = {},
      .width = width,
      .height = height,
      .format = format
  };
  initialize(image, lazyLoading);
}

void OpenGLTexture::initialize(ImageData imageData, bool lazyLoading) {
  assert(!m_initializer && "Texture is initialized twice");

  m_initializer = [=, image = std::move(imageData)]() {
    assert(!m_initialized);

    GLint format = getGLFormat(image.format);
    auto data = image.pixel.empty() ? nullptr : image.pixel.data();

    GLuint textureId = 0;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, format, image.width, image.height, 0, format, GL_UNSIGNED_BYTE, data);

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

void OpenGLTexture::bind() {
  _initIfNeeded();
  glBindTexture(GL_TEXTURE_2D, m_textureId);
}

uint32_t OpenGLTexture::getHandle() const {
  return m_textureId;
}