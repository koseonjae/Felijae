#pragma once

#include <Graphics/Model/Buffer.h>
#include <Graphics/Model/Object.h>

#include <OpenGL/gl3.h>
#include <glm/glm.hpp>

class OpenGLBuffer final : public Buffer {
 public:
  OpenGLBuffer() = default;

  ~OpenGLBuffer() override;

  void initialize(const Object& obj) override;

  void release() override;

  void bind() override;

  void draw() override;

 private:
  bool m_initialized = false;
  Object m_obj;
  GLuint m_vao = 0;
  GLuint m_abo = 0;
  GLuint m_eabo = 0;
  int m_indicesSize = 0;
};
