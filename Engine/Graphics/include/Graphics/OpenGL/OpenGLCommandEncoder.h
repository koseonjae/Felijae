#pragma once

#include <Graphics/Model/CommandEncoder.h>

#include <functional>

namespace goala {
class OpenGLRenderPass;
class OpenGLCommandBuffer;

class OpenGLCommandEncoder : public CommandEncoder {
public:
  OpenGLCommandEncoder(OpenGLCommandBuffer* commandBuffer, OpenGLRenderPass* renderPass, CommandEncoderDescription desc);

  void endEncoding() override;

  void encodeDraw(Pipeline* pipeline) override;

  void setEncodedCallback(std::function<void(std::function<void()>)>&& callback);

private:
  OpenGLRenderPass* m_renderPass = nullptr;
  std::function<void(std::function<void()>)> m_encodedCallback;
  std::function<void()> m_encoded;
  bool m_isEnd = false;
};
} // namespace goala
