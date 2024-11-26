#include <Graphics/OpenGL/OpenGLFrameBuffer.h>
#include <Graphics/OpenGL/OpenGLTexture.h>

#include <cassert>

using namespace goala;

OpenGLFrameBuffer::~OpenGLFrameBuffer() {
  if (!m_initialized) return;
  glDeleteFramebuffers(1, &m_handle);
};

void OpenGLFrameBuffer::initialize(const std::shared_ptr<Texture>& texture,
                                   GLuint attachmentIdx) {
  auto glTexture = static_pointer_cast<OpenGLTexture>(texture);

  GLuint handle;
  glGenFramebuffers(1, &handle);
  glBindFramebuffer(GL_FRAMEBUFFER, handle);
  glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentIdx, GL_TEXTURE_2D, static_pointer_cast<OpenGLTexture>(texture)->getHandle(), 0);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    assert(false && "Failed to create frame buffer");

  m_handle = handle;
  m_initialized = true;
}

void OpenGLFrameBuffer::bind() {
  assert(m_initialized && "FrameBuffer is not initialized");
  glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
}
