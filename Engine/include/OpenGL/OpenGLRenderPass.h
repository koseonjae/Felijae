#pragma once

#include <Model/RenderPass.h>
#include <OpenGL/OpenGLFrameBuffer.h>

class OpenGLRenderPass : public RenderPass {
 public:
  void bind() override;
  void render() override;

 private:
  void _updateFrameBuffers();

 private:
  std::vector<OpenGLFrameBuffer> m_frameBuffers;
};
