#pragma once

#include <Model/Texture.h>
#include <Model/RenderPass.h>

#include <OpenGL/gl3.h>

#include <memory>

class OpenGLFrameBuffer {
 public:
  void initialize(std::shared_ptr<Texture> texture, GLuint attachmentIdx);
  void bind();

 private:
  GLuint m_handle;
};
