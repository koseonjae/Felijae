#pragma once

#include <Graphics/Model/CommandBuffer.h>

#include <functional>

namespace goala {

class OpenGLCommandBuffer : public CommandBuffer {
 public:
  void encode(Pipeline* pipeline) override;

  void present(Texture* texture) override;

  void commit() override;
 private:
  std::function<void()> m_encoded;
};

} // namespace goala
