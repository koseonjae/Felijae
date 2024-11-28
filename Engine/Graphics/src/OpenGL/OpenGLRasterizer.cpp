#include <Graphics/OpenGL/OpenGLRasterizer.h>
#include <Graphics/OpenGL/OpenGLCommandEncoder.h>

#include <OpenGL/gl3.h>

#include <cassert>

using namespace goala;

void OpenGLRasterizer::bind(void* descriptor) {
  _bindCulling();
  _bindViewport();
}

void OpenGLRasterizer::encode(CommandEncoder* encoder) {}

void OpenGLRasterizer::_bindCulling() {
  const auto& culling = getCulling();

  if (!culling.enable) {
    glDisable(GL_CULL_FACE);
    return;
  }
  glEnable(GL_CULL_FACE);

  if (culling.frontFace == Culling::FrontFace::CCW)
    glFrontFace(GL_CCW);
  else
    glFrontFace(GL_CW);

  if (culling.cullMode == Culling::CullMode::Back)
    glCullFace(GL_BACK);
  else
    glCullFace(GL_FRONT);
}

void OpenGLRasterizer::_bindViewport() {
  const auto& viewport = getViewport();

  assert(viewport.minX >= 0 && viewport.minY >= 0 && "Invalid viewport minX, minY");
  assert(viewport.width > 0 && viewport.height > 0 && "Invalid viewport width, height");
  glViewport(viewport.minX, viewport.minY, viewport.width, viewport.height);

  assert(0.f <= viewport.minZ && viewport.minZ <= 1.f && "Invalid viewport minZ");
  assert(0.f <= viewport.maxZ && viewport.maxZ <= 1.f && "Invalid viewport maxZ");
  glDepthRangef(viewport.minZ, viewport.maxZ);
}
