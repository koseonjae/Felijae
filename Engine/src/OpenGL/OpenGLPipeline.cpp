#include "OpenGL/OpenGLPipeline.h"

#include <glad/glad.h>

void Culling::bind() {
  if (!m_enable) {
    glDisable(GL_CULL_FACE);
    return;
  }

  glEnable(GL_CULL_FACE);

  if (m_frontFace == FrontFace::CCW)
    glFrontFace(GL_CCW);
  else
    glFrontFace(GL_CW);

  if (m_cullMode == CullMode::Back)
    glCullFace(GL_BACK);
  else
    glCullFace(GL_FRONT);
}

void Viewport::bind() {
  glViewport(m_minX, m_minY, m_width, m_height);
  glDepthRangef(m_minZ, m_maxZ);
}

void Rasterizer::bind() {
  culling.bind();
  viewport.bind();
}

void OutputMerger::bind() {

}

void OpenGLPipeline::bind() {
  rasterizer.bind();
  outputMerger.bind();
}
