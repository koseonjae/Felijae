#pragma once

#include "Model/Object.h"
#include "OpenGLProgram.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <string_view>

class OpenGLModel {
 public:
  OpenGLModel() = default;

  ~OpenGLModel();

  void initialize(Object obj);

  void release();

  void update();

  void draw();

  void setProgram(std::unique_ptr<OpenGLProgram> program);

 private:
  bool m_initialized = false;
  Object m_obj;
  GLuint m_vao;
  GLuint m_abo;
  GLuint m_eabo;

  std::unique_ptr<OpenGLProgram> m_program;
  glm::mat4 m_translate = glm::mat4(1.0);
};
