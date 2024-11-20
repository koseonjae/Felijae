#include <OpenGL/OpenGLFrameBuffer.h>
#include <OpenGL/OpenGLRenderPass.h>

#include <cassert>

void OpenGLFrameBuffer::initialize(std::shared_ptr<Texture> texture, GLuint attachmentIdx) {
  GLuint handle;
  glGenFramebuffers(1, &handle);
  glBindFramebuffer(GL_FRAMEBUFFER, handle);

  texture->bind();
  glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentIdx, GL_TEXTURE_2D, texture->getHandle(), 0);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    assert(false && "Failed to create frame buffer");

  m_handle = handle;
}

void OpenGLFrameBuffer::bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
}
