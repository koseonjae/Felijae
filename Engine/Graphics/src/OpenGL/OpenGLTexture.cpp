#include <Base/Utility/ImageLoader.h>
#include <Graphics/OpenGL/OpenGLTexture.h>
#include <Graphics/OpenGL/OpenGLUtils.h>

#include <cassert>

using namespace goala;

OpenGLTexture::~OpenGLTexture() {
  if (m_externalHandleInitialized)
    return;
  if (!m_initialized)
    return;
  auto handle = getHandle<GLuint>();
  glDeleteTextures(1, &handle);
}

void OpenGLTexture::_initIfNeeded() {
  if (m_externalHandleInitialized)
    return;
  if (m_initialized)
    return;
  if (!m_initializer) assert(false && "Texture is not initialized");
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
    .format = format};
  initialize(image, lazyLoading);
}

void OpenGLTexture::initialize(ImageData imageData, bool lazyLoading) {
  assert(!m_initializer && "Texture is initialized twice");
  assert(!m_externalHandleInitialized && "Already initialized with external handle");

  auto weak = weak_from_this();
  m_initializer = [weak, image = std::move(imageData)]() {
    auto self = weak.lock();
    if (!self)
      return;
    assert(!self->m_initialized);

    GLuint format = getGLFormat(image.format);
    auto data = image.pixel.empty() ? nullptr : image.pixel.data();

    GLuint textureId = 0;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, format, image.width, image.height, 0, format,
                 GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    self->m_handle = textureId;
    self->m_initialized = true;
  };

  if (!lazyLoading) _initIfNeeded();
}

void OpenGLTexture::bind() {
  _initIfNeeded();
  glBindTexture(GL_TEXTURE_2D, getHandle<GLuint>());
  static_assert(std::is_same<GLuint, uint32_t>::value, "GLuint type is not same with uint32_t");
}
