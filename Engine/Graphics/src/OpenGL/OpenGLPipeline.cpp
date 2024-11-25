#include <Graphics/OpenGL/OpenGLPipeline.h>

using namespace goala;

void OpenGLPipeline::update() {
}

void OpenGLPipeline::render() {
  assert(m_rasterizer && m_outputMerger && m_program && m_buffer && "There is empty pipeline");
  m_rasterizer->bind(nullptr);
  m_outputMerger->bind();
  m_program->bind();
  m_buffer->bind();
  m_buffer->draw();
}
