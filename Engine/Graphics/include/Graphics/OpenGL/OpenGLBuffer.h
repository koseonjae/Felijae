#pragma once

#include <Graphics/Model/Buffer.h>
#include <OpenGL/gl3.h>

namespace goala {
class OpenGLDevice;

class OpenGLBuffer final : public Buffer {
public:
  OpenGLBuffer(OpenGLDevice* device, BufferDescription desc);

  ~OpenGLBuffer() override;

  void bind() override;

  void draw() override;

private:
  bool m_initialized = false;
  GLuint m_vao = 0;
  GLuint m_abo = 0;
  GLuint m_eabo = 0;
  int m_indicesSize = 0;
};
} // namespace goala
