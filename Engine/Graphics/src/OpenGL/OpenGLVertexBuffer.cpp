#include <Graphics/OpenGL/OpenGLVertexBuffer.h>
#include <Base/Object/Object.h>

using namespace goala;

OpenGLVertexBuffer::OpenGLVertexBuffer(OpenGLDevice* device, VertexBufferDescription desc) {
  auto& obj = desc.object;

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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));

  // Attribute 1 : Normal
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, nor));

  // Attribute 2 : TexCoord
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

  glBindVertexArray(0);

  m_indicesSize = static_cast<int>(obj.indices.size());

  m_initialized = true;
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
  if (!m_initialized)
    return;
  glDeleteBuffers(1, &m_abo);
  glDeleteBuffers(1, &m_eabo);
  glDeleteVertexArrays(1, &m_vao);
  m_initialized = false;
}

void OpenGLVertexBuffer::draw() {
  assert(m_initialized && "OpenGLBuffer is not initialized");
  glBindVertexArray(m_vao);
  glDrawElements(GL_TRIANGLES, m_indicesSize, GL_UNSIGNED_INT, nullptr);
  glBindVertexArray(0);
}
