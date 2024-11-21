#pragma once

#include <Graphics/Model/RenderPass.h>
#include <Graphics/OpenGL/OpenGLFrameBuffer.h>

class OpenGLRenderPass : public RenderPass {
 public:
  void bind() override;
  const OpenGLFrameBuffer& getFrameBuffer(int idx) const { return m_frameBuffers[idx]; }

 private:
  void _updateFrameBuffers();
  void _updateRenderPass();

 private:
  std::vector<OpenGLFrameBuffer> m_frameBuffers;
};
