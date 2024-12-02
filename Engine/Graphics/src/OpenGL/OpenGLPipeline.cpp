#include <Graphics/OpenGL/OpenGLPipeline.h>

#include <OpenGL/gl3.h>

using namespace goala;

OpenGLPipeline::OpenGLPipeline(OpenGLDevice* device, PipelineDescription desc)
  : Pipeline(std::move(desc)) {}

void OpenGLPipeline::update() {}

void OpenGLPipeline::render() {
  assert(m_desc.outputMerger && m_desc.program && m_desc.buffer && "There is empty pipeline");
  _bindViewport();
  _bindCulling();
  m_desc.outputMerger->bind(nullptr);
  m_desc.program->bind(m_desc.uniforms.get());
  m_desc.buffer->bind();
  m_desc.buffer->draw();
}

void OpenGLPipeline::_bindCulling() {
  const auto& culling = m_desc.rasterizer.culling;

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

void OpenGLPipeline::_bindViewport() {
  const auto& viewport = m_desc.rasterizer.viewport;

  assert(viewport.minX >= 0 && viewport.minY >= 0 && "Invalid viewport minX, minY");
  assert(viewport.width > 0 && viewport.height > 0 && "Invalid viewport width, height");
  glViewport(viewport.minX, viewport.minY, viewport.width, viewport.height);

  assert(0.f <= viewport.minZ && viewport.minZ <= 1.f && "Invalid viewport minZ");
  assert(0.f <= viewport.maxZ && viewport.maxZ <= 1.f && "Invalid viewport maxZ");
  glDepthRangef(viewport.minZ, viewport.maxZ);
}
