#pragma once

#include <glad/glad.h>
#include <string_view>

class OpenGLProgram {
 public:
  OpenGLProgram() = default;
  ~OpenGLProgram();

  void initialize(std::string_view vsPath, std::string_view fsPath);

  void bind();

 private:
  GLuint m_program = -1;
};
