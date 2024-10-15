#include "OpenGLProgram.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

#include <iostream>
#include <cassert>
#include <string_view>

void checkCompileErrors(GLuint shader, std::string_view type) {
  GLint success;
  GLchar infoLog[1024];
  if (type != "PROGRAM") {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog
                << "\n -- --------------------------------------------------- -- " << std::endl;
    }
  } else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog
                << "\n -- --------------------------------------------------- -- " << std::endl;
    }
  }
}

void OpenGLProgram::initialize(std::string_view vertexShaderStr, std::string_view fragShaderStr) {
  if (m_program == -1)
    glDeleteProgram(m_program);
  const char *vs_str = vertexShaderStr.data();
  const char *fs_str = fragShaderStr.data();

  // Compile vertex shader
  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &vs_str, nullptr);
  glCompileShader(vs);
  checkCompileErrors(vs, "VERTEX");

  // Compile fragment shader
  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &fs_str, nullptr);
  glCompileShader(fs);
  checkCompileErrors(fs, "FRAGMENT");

  // Link shaders into program
  m_program = glCreateProgram();
  glAttachShader(m_program, vs);
  glAttachShader(m_program, fs);
  glLinkProgram(m_program);
  checkCompileErrors(m_program, "PROGRAM");

  glDeleteShader(vs);
  glDeleteShader(fs);
}

OpenGLProgram::~OpenGLProgram() {
  if (m_program == -1)
    return;
  glDeleteProgram(m_program);
}

void OpenGLProgram::update() {
  assert(m_program != -1);
  glUseProgram(m_program);

  decltype(m_generalTasks) generalTasks;
  decltype(m_textureTasks) textureTasks;
  {
    std::lock_guard<std::mutex> l(m_taskLock);
    generalTasks = std::move(m_generalTasks);
    textureTasks = std::move(m_textureTasks);
  }

  for (const auto &[name, task] : generalTasks)
    task();

  int index = 0;
  for (const auto &[name, task] : textureTasks)
    task(index++);
}

void OpenGLProgram::setUniform(const std::string &name, const glm::mat4 &mat4) {
  std::lock_guard<std::mutex> l(m_taskLock);
  m_generalTasks.insert({name, [=]() {
    GLint loc = glGetUniformLocation(m_program, name.data());
    assert(loc != -1);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat4));
  }});
}

void OpenGLProgram::setUniform(const std::string &name, std::shared_ptr<OpenGLTexture> texture) {
  std::lock_guard<std::mutex> l(m_taskLock);
  m_textureTasks.insert({name, [=, texture = move(texture)](int index) {
    glActiveTexture(GL_TEXTURE0 + index);
    texture->bind();
    GLint loc = glGetUniformLocation(m_program, name.data());
    assert(loc != -1);
    glUniform1i(loc, index);
  }});
}
