#pragma once

#include "Model/Object.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

class OpenGLModel {
 public:
  OpenGLModel() = default;
  ~OpenGLModel();
  void initialize(Object obj);
  void release();
  void bind();
  void draw();

 private:
  bool m_initialized = false;
  Object m_obj;
  GLuint m_vao;
  GLuint m_abo;
  GLuint m_eabo;
};
