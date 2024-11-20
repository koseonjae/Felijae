#include <OpenGL/OpenGLModel.h>

#include <OpenGL/OpenGLProgram.h>
#include <OpenGL/OpenGLPipeline.h>
#include <OpenGL/OpenGLBuffer.h>
#include <glm/gtc/matrix_transform.hpp>

void OpenGLModel::update() {
  assert(m_pipeline);
  assert(m_program);

  m_pipeline->bind();
  m_program->update();
}

void OpenGLModel::render() {
  m_buffer->bind();
  m_buffer->draw();
}
