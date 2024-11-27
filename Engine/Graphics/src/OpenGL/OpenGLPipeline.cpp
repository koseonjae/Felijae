#include <Graphics/OpenGL/OpenGLPipeline.h>

using namespace goala;

OpenGLPipeline::OpenGLPipeline(OpenGLDevice* device, PipelineDescription desc)
  : Pipeline(std::move(desc)) {}

void OpenGLPipeline::update() {}

void OpenGLPipeline::render() {
  assert(m_desc.rasterizer && m_desc.outputMerger && m_desc.program && m_desc.buffer && "There is empty pipeline");
  m_desc.rasterizer->bind(nullptr);
  m_desc.outputMerger->bind(nullptr);
  m_desc.program->bind(m_desc.uniforms.get());
  m_desc.buffer->bind();
  m_desc.buffer->draw();
}
