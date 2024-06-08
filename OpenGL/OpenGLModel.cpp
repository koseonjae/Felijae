#include "OpenGLModel.h"

OpenGLModel::~OpenGLModel() {
  glDeleteBuffers(1, &m_abo);
  glDeleteBuffers(1, &m_eabo);
  glDeleteVertexArrays(1, &m_vao);
}

void OpenGLModel::initialize(Object obj) {
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

  m_obj = std::move(obj);
}

void OpenGLModel::bind() {
  glBindVertexArray(m_abo);
}
void OpenGLModel::draw() {
  glDrawElements(GL_TRIANGLES, m_obj.indices.size(), GL_UNSIGNED_INT, nullptr);
}
