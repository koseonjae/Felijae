#pragma once

#include <Model/Object.h>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <string_view>
#include <memory>

class OpenGLPipeline;
class OpenGLProgram;

class OpenGLModel {
 public:
  OpenGLModel() = default;

  ~OpenGLModel();

  void initialize(Object obj);

  void release();

  void update();

  void draw();

  void setProgram(std::unique_ptr<OpenGLProgram> program);

  void setPipeline(std::shared_ptr<OpenGLPipeline> pipeline);

 private:
  bool m_initialized = false;
  Object m_obj;
  GLuint m_vao;
  GLuint m_abo;
  GLuint m_eabo;

  std::unique_ptr<OpenGLProgram> m_program;
  std::shared_ptr<OpenGLPipeline> m_pipeline;
  glm::mat4 m_translate = glm::mat4(1.0);
};
