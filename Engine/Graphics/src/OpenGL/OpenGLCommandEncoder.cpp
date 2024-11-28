#include <Graphics/OpenGL/OpenGLCommandEncoder.h>
#include <Graphics/Model/Pipeline.h>

#include <cassert>

using namespace goala;

OpenGLCommandEncoder::OpenGLCommandEncoder(OpenGLCommandBuffer* commandBuffer, OpenGLRenderPass* renderPass, CommandEncoderDescription) {}

void OpenGLCommandEncoder::setEncodedCallback(std::function<void(std::function<void()>)>&& callback) {
  assert(!m_encodedCallback && "Encoded callback can not be set twice");
  m_encodedCallback = std::move(callback);
}

void OpenGLCommandEncoder::endEncoding() {
  assert(m_isEnd == false && "endEncoding can not be called twice");
  m_isEnd = true;
  m_encodedCallback(std::move(m_encoded));
}

void OpenGLCommandEncoder::encodeDraw(Pipeline* pipeline) {
  m_encoded = [pipeline]() {
    pipeline->render();
  };
}
