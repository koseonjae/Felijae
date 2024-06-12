#pragma once

#include "OpenGLTexture.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string_view>
#include <functional>
#include <unordered_map>

class OpenGLProgram {
 public:
  OpenGLProgram() = default;
  ~OpenGLProgram();

  OpenGLProgram(OpenGLProgram &&) = delete;
  OpenGLProgram &operator=(OpenGLProgram &&) = delete;

  OpenGLProgram(const OpenGLProgram &) = delete;
  OpenGLProgram &operator=(const OpenGLProgram &) = delete;

  void initialize(std::string_view vsPath, std::string_view fsPath);

  void update();

  void setUniform(const std::string &name, const glm::mat4 &mat4);

  void setUniform(const std::string &name, std::shared_ptr<OpenGLTexture> texture);

 private:
  GLuint m_program = -1;
  std::mutex m_taskLock;
  std::unordered_map<std::string, std::function<void()>> m_generalTasks;
  std::unordered_map<std::string, std::function<void(int index)>> m_textureTasks;
};
