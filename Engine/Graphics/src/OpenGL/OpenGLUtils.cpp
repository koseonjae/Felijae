#include <Graphics/OpenGL/OpenGLUtils.h>

#include <cassert>

using namespace goala;

GLuint getGLFormat(ImageFormat format) {
  switch (format) {
    case ImageFormat::RGB:
      return GL_RGB;
    case ImageFormat::RGBA:
      return GL_RGBA;
    case ImageFormat::BGRA:
      return GL_BGRA;
    default: {
    }
  }
  assert(false && "Undefined format");
  return GL_RGB;
}
