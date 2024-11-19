#include <OpenGL/OpenGLProgram.h>

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
      assert(false && "Shader compilation error");
    }
  } else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog
                << "\n -- --------------------------------------------------- -- " << std::endl;
      assert(false && "program linking error");
    }
  }
}

void OpenGLProgram::initialize(std::string_view vertexShaderStr, std::string_view fragShaderStr) {
  m_threadChecker.checkThread();
  assert(!m_initialized && "OpenGLProgram is already initialized");

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
  int program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  checkCompileErrors(program, "PROGRAM");
  m_program = program;

  glDeleteShader(vs);
  glDeleteShader(fs);

  m_initialized = true;
}

OpenGLProgram::~OpenGLProgram() {
  m_threadChecker.checkThread();
  if (!m_initialized)
    return;
  glDeleteProgram(m_program);
}

void OpenGLProgram::update() {
  m_threadChecker.checkThread();
  assert(m_initialized && "OpenGLProgram is not initialized");

  glUseProgram(m_program);

  decltype(m_generalTasks) generalTasks;
  decltype(m_textureTasks) textureTasks;
  {
    std::lock_guard<std::mutex> l(m_taskLock);
    generalTasks = std::move(m_generalTasks);
    textureTasks = std::move(m_textureTasks);
  }

  for (const auto& [name, task] : generalTasks)
    task(name);

  int index = 0;
  for (const auto& [name, task] : textureTasks)
    task(name, index++);
}

void OpenGLProgram::setUniform(std::string_view name, const glm::vec3& vec3) {
  std::lock_guard<std::mutex> l(m_taskLock);
  m_generalTasks.insert({name.data(), [=](std::string_view name) {
    GLint loc = glGetUniformLocation(m_program, name.data());
    assert(loc != -1 && "Invalid uniform location");
    glUniform3fv(loc, 1, glm::value_ptr(vec3));
  }});
}

void OpenGLProgram::setUniform(std::string_view name, const glm::mat4& mat4) {
  std::lock_guard<std::mutex> l(m_taskLock);
  m_generalTasks.insert({name.data(), [=](std::string_view name) {
    GLint loc = glGetUniformLocation(m_program, name.data());
    assert(loc != -1 && "Invalid uniform location");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat4));
  }});
}

void OpenGLProgram::setTexture(std::string_view name, std::shared_ptr<Texture> texture) {
  std::lock_guard<std::mutex> l(m_taskLock);
  m_textureTasks.insert({name.data(), [=, texture = std::move(texture)](std::string_view name, int index) {
    glActiveTexture(GL_TEXTURE0 + index);
    texture->bind();
    GLint loc = glGetUniformLocation(m_program, name.data());
    assert(loc != -1 && "Invalid texture location");
    glUniform1i(loc, index);
  }});
}
