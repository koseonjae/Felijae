#pragma once

#include <Model/Texture.h>
#include <Model/RenderPass.h>

#include <OpenGL/gl3.h>

#include <memory>

class OpenGLFrameBuffer {
 public:
  ~OpenGLFrameBuffer();
  void initialize(const std::shared_ptr<Texture>& texture, GLuint attachmentIdx);
  void bind();
  GLuint getHandle() const { return m_handle; }

 private:
  GLuint m_handle;
  bool m_initialized = false;
};
