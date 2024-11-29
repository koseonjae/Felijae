#pragma once

#include <Graphics/Model/RenderPass.h>
#include <Graphics/OpenGL/OpenGLFrameBuffer.h>

namespace goala {
class OpenGLDevice;

class OpenGLRenderPass : public RenderPass {
public:
  OpenGLRenderPass(OpenGLDevice* device, RenderPassDescription desc);

  const OpenGLFrameBuffer& getFrameBuffer(int idx) const {
    return m_frameBuffers[idx];
  }

  void bind();

private:
  void _updateFrameBuffers(const RenderPassDescription& desc);
  void _updateRenderPass(const RenderPassDescription& desc);

private:
  OpenGLDevice* m_device;
  RenderPassDescription m_desc{};
  std::vector<OpenGLFrameBuffer> m_frameBuffers;
};
} // namespace goala
