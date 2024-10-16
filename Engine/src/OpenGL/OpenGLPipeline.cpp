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
  assert(m_minX >= 0 && m_minY >= 0 && m_width > 0 && m_height > 0 && "Invalid viewport");
  glViewport(m_minX, m_minY, m_width, m_height);
  assert(0 <= m_minZ && m_minZ <= 1 && 0 <= m_maxZ && m_maxZ <= 1 && "Invalid viewport depth value");
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
