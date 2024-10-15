#include "OpenGL/OpenGLModel.h"

#include <OpenGL/OpenGLProgram.h>
#include <OpenGL/OpenGLPipeline.h>
#include <glm/gtc/matrix_transform.hpp>

OpenGLModel::~OpenGLModel() {
  release();
}

void OpenGLModel::initialize(Object obj) {
  release();

  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  // VBO
  glGenBuffers(1, &m_abo);
  glBindBuffer(GL_ARRAY_BUFFER, m_abo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * obj.vertices.size(), obj.vertices.data(), GL_STATIC_DRAW);

  // EBO
  glGenBuffers(1, &m_eabo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eabo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * obj.indices.size(), obj.indices.data(), GL_STATIC_DRAW);

  // Attribute 0 : Position
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, pos));

  // Attribute 1 : Normal
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, nor));

  // Attribute 2 : TexCoord
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, texCoord));

  m_obj = std::move(obj);

  m_initialized = true;
}

void OpenGLModel::release() {
  if (!m_initialized)
    return;
  glDeleteBuffers(1, &m_abo);
  glDeleteBuffers(1, &m_eabo);
  glDeleteVertexArrays(1, &m_vao);
  m_initialized = false;
}

void OpenGLModel::update() {
  assert(m_pipeline);
  assert(m_program);

  m_pipeline->bind();
  m_program->update();
}

void OpenGLModel::draw() {
  assert(m_initialized);
  glBindVertexArray(m_abo);

  assert(m_program);
  m_program->update();

  glDrawElements(GL_TRIANGLES, m_obj.indices.size(), GL_UNSIGNED_INT, nullptr);
}

void OpenGLModel::setProgram(std::unique_ptr<OpenGLProgram> program) {
  m_program = std::move(program);
}

void OpenGLModel::setPipeline(std::shared_ptr<OpenGLPipeline> pipeline) {
  m_pipeline = std::move(pipeline);
}
