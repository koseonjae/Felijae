#pragma once

#include <Model/Model.h>
#include <Model/Object.h>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <string_view>
#include <memory>

class OpenGLPipeline;
class OpenGLProgram;

class OpenGLModel final : public Model {
 public:
  OpenGLModel() = default;

  ~OpenGLModel() override;

  void initialize(const Object& obj) override;

  void release() override;

  void update() override;

  void render() override;

 private:
  bool m_initialized = false;
  Object m_obj;
  GLuint m_vao = 0;
  GLuint m_abo = 0;
  GLuint m_eabo = 0;
  int m_indicesSize = 0;
};
