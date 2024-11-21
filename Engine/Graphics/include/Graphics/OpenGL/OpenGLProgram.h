#pragma once

#include <Base/Utility/ThreadChecker.h>
#include <Graphics/Model/Program.h>
#include <Graphics/OpenGL/OpenGLTexture.h>

#include <functional>
#include <glm/glm.hpp>
#include <string_view>
#include <unordered_map>

namespace goala {

class OpenGLProgram final : public Program {
 public:
  OpenGLProgram() = default;
  ~OpenGLProgram() override;

  OpenGLProgram(OpenGLProgram&&) = delete;
  OpenGLProgram& operator=(OpenGLProgram&&) = delete;

  OpenGLProgram(const OpenGLProgram&) = delete;
  OpenGLProgram& operator=(const OpenGLProgram&) = delete;

  void initialize(std::string_view vsPath, std::string_view fsPath) override;

  void bind() override;

  void setUniform(std::string_view name, const glm::vec3& vec3) override;

  void setUniform(std::string_view name, const glm::mat4& mat4) override;

  void setTexture(std::string_view name,
                  std::shared_ptr<Texture> texture) override;

 private:
  void _updateTasks();

 private:
  GLuint m_program = 0;
  bool m_initialized = false;
  std::mutex m_taskLock;
  std::unordered_map<std::string, std::function<void(std::string_view)>>
    m_generalTasks;
  std::unordered_map<std::string, std::function<void(std::string_view, int)>>
    m_textureTasks;
  ThreadChecker m_threadChecker;
  std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
};

} // namespace goala
