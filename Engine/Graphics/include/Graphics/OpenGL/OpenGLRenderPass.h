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
  void _updateFrameBuffers();
  void _updateRenderPass();

private:
  OpenGLDevice* m_device;
  std::vector<OpenGLFrameBuffer> m_frameBuffers;
};
} // namespace goala
