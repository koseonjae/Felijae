#include <Graphics/OpenGL/OpenGLPipeline.h>

void OpenGLPipeline::update() {
}

void OpenGLPipeline::render() {
  assert(m_renderPass && m_rasterizer && m_outputMerger && m_program && m_buffer && "There is empty pipeline");
  m_renderPass->bind();
  m_rasterizer->bind();
  m_outputMerger->bind();
  m_program->bind();
  m_buffer->bind();
  m_buffer->draw();
}
