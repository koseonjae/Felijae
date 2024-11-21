#include <OpenGL/OpenGLModel.h>

#include <OpenGL/OpenGLProgram.h>
#include <OpenGL/OpenGLPipeline.h>
#include <OpenGL/OpenGLBuffer.h>
#include <glm/gtc/matrix_transform.hpp>

void OpenGLModel::update() {
  assert(m_pipeline);
  m_pipeline->update();
}

void OpenGLModel::render() {
  assert(m_pipeline);
  m_pipeline->render();
}
