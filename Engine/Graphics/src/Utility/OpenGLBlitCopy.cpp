#include <Graphics/Utility/OpenGLBlitCopy.h>
#include <Graphics/OpenGL/OpenGLRenderPass.h>
#include <Base/Utility/TypeCast.h>

#include <OpenGL/gl3.h>

namespace goala {
void blitCopyFrameBufferToScreen(RenderPass* renderPass, int width, int height) {
  auto openGLRenderPass = SAFE_DOWN_CAST(OpenGLRenderPass*, renderPass);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, openGLRenderPass->getFrameBuffer(0).getHandle());
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

  glBlitFramebuffer(
    0, 0, width, height, // 소스 영역 (FBO)
    0, 0, width, height, // 대상 영역 (화면)
    GL_COLOR_BUFFER_BIT, GL_NEAREST);
}
}
