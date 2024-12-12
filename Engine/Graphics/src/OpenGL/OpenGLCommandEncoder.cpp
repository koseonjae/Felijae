#include <Graphics/OpenGL/OpenGLCommandEncoder.h>
#include <Graphics/OpenGL/OpenGLRenderPass.h>
#include <Graphics/Model/Pipeline.h>

#include <cassert>

#include "Base/Utility/TypeCast.h"
#include "Graphics/OpenGL/OpenGLPipeline.h"

using namespace goala;

OpenGLCommandEncoder::OpenGLCommandEncoder(OpenGLCommandBuffer* commandBuffer, OpenGLRenderPass* renderPass, CommandEncoderDescription desc)
  : m_renderPass(renderPass) {}

void OpenGLCommandEncoder::setEncodedCallback(std::function<void(std::function<void()>)>&& callback) {
  assert(!m_encodedCallback && "Encoded callback can not be set twice");
  m_encodedCallback = std::move(callback);
}

void OpenGLCommandEncoder::endEncoding() {
  assert(m_isEnd == false && "endEncoding can not be called twice");
  m_isEnd = true;

  m_encodedCallback([commands = std::move(m_commands), renderPass = m_renderPass]() {
    assert(renderPass && "Render pass can not be null");
    renderPass->bind();
    for (auto& command : commands)
      command();
  });
}

void OpenGLCommandEncoder::encodeDraw(Pipeline* pipeline) {
  m_commands.emplace_back([pipeline]() {
    SAFE_DOWN_CAST(OpenGLPipeline*, pipeline)->render();
  });
}
