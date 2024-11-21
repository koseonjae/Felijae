#include <OpenGL/OpenGLRasterizer.h>

#include <OpenGL/gl3.h>

#include <cassert>

void OpenGLRasterizer::bind() {
  _bindCulling();
  _bindViewport();
}

void OpenGLRasterizer::_bindCulling() {
  const auto [enable, frontFace, cullMode] = getCulling().getVariables();
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

void OpenGLRasterizer::_bindViewport() {
  const auto [minX, minY, width, height, minZ, maxZ] = getViewport().getVariables();
  assert(minX >= 0 && minY >= 0 && width > 0 && height > 0 && "Invalid viewport");
  glViewport(minX, minY, width, height);
  assert(0 <= minZ && minZ <= 1 && 0 <= maxZ && maxZ <= 1 && "Invalid viewport depth value");
  glDepthRangef(minZ, maxZ);
}
