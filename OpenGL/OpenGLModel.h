#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

struct Vertex {
  glm::vec3 pos;
};

struct Object {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
};

class OpenGLModel {
 public:
  OpenGLModel() = default;
  ~OpenGLModel();
  void initialize(Object obj);
  void bind();
  void draw();

 private:
  Object m_obj;
  GLuint m_vao;
  GLuint m_abo;
  GLuint m_eabo;
};
