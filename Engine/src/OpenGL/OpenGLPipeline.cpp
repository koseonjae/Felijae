#include "OpenGL/OpenGLPipeline.h"

#include <glad/glad.h>
#include <cassert>

void OpenGLPipeline::_bindCulling() {
  auto [enable, frontFace, cullMode] = m_rasterizer.getCulling().getCulling();
  if (!enable) {
    glDisable(GL_CULL_FACE);
    return;
  }

  glEnable(GL_CULL_FACE);

  if (frontFace == Culling::FrontFace::CCW)
    glFrontFace(GL_CCW);
  else
    glFrontFace(GL_CW);

  if (cullMode == Culling::CullMode::Back)
    glCullFace(GL_BACK);
  else
    glCullFace(GL_FRONT);
}

void OpenGLPipeline::_bindViewport() {
  auto [minX, minY, width, height, minZ, maxZ] = m_rasterizer.getViewport().getViewPort();
  assert(minX >= 0 && minY >= 0 && width > 0 && height > 0 && "Invalid viewport");
  glViewport(minX, minY, width, height);
  assert(0 <= minZ && minZ <= 1 && 0 <= maxZ && maxZ <= 1 && "Invalid viewport depth value");
  glDepthRangef(minZ, maxZ);
}

void OpenGLPipeline::_bindRasterizer() {
  _bindCulling();
  _bindViewport();
}

void OpenGLPipeline::_bindOutputMerger() {
  glClearColor(0.0, 1.0, 0.0, 1.0);
}

void OpenGLPipeline::bind() {
  _bindRasterizer();
  _bindOutputMerger();
}