#include <Graphics/OpenGL/OpenGLModel.h>

#include <Graphics/OpenGL/OpenGLPipeline.h>
#include <Graphics/OpenGL/OpenGLBuffer.h>
#include <glm/gtc/matrix_transform.hpp>

void OpenGLModel::update() {
  assert(m_pipeline);
  m_pipeline->update();
}

void OpenGLModel::render() {
  assert(m_pipeline);
  m_pipeline->render();
}
