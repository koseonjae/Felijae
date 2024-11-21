#include <Graphics/OpenGL/OpenGLRenderPass.h>

#include <OpenGL/gl3.h>

#include <cassert>

void OpenGLRenderPass::_updateFrameBuffers() {
  if (!m_dirty)
    return;
  m_frameBuffers.clear();
  int colorAttachmentCnt = 0;
  for (const auto& attachment : m_attachments) {
    auto& frameBuffer = m_frameBuffers.emplace_back();
    if (attachment.type == AttachmentType::Color)
      frameBuffer.initialize(attachment.texture, GL_COLOR_ATTACHMENT0 + colorAttachmentCnt++);
    else if (attachment.type == AttachmentType::Depth)
      frameBuffer.initialize(attachment.texture, GL_DEPTH_ATTACHMENT);
    else if (attachment.type == AttachmentType::Stencil)
      frameBuffer.initialize(attachment.texture, GL_STENCIL_ATTACHMENT);
    else
      assert(false && "Undefined attachment type");
  }
  m_dirty = false;
}

void OpenGLRenderPass::bind() {
  _updateFrameBuffers();
  for (auto& frameBuffer : m_frameBuffers)
    frameBuffer.bind();
  _updateRenderPass();
}

void OpenGLRenderPass::_updateRenderPass() {
  GLbitfield clearBit = GL_NONE;

  for (const auto& attachment : m_attachments) {
    assert(attachment.type != AttachmentType::Undefined && "AttachmentType is not defined");
    assert(attachment.loadFunc != LoadFunc::Undefined && "LoadFunc is not defined");
    assert(attachment.storeFunc != StoreFunc::Undefined && "StoreFunc is not defined");

    if (attachment.loadFunc == LoadFunc::Clear) {
      if (attachment.type == AttachmentType::Color) {
        const auto& color = std::get<ClearColor>(attachment.clear);
        assert(color.r >= 0.0f && color.g >= 0.0f && color.b >= 0.0f && color.a >= 0.0f && "ClearColor is not defined");
        glClearColor(color.r, color.g, color.b, color.a);
        clearBit |= GL_COLOR_BUFFER_BIT;
      }
      else if (attachment.type == AttachmentType::Depth) {
        const auto& depth = std::get<ClearDepth>(attachment.clear);
        assert(depth.depth >= 0.0f && "ClearDepth is not defined");
        glClearDepth(static_cast<double>(depth.depth));
        clearBit |= GL_DEPTH_BUFFER_BIT;
      }
      else if (attachment.type == AttachmentType::Stencil) {
        const auto& stencil = std::get<ClearStencil>(attachment.clear);
        assert(stencil.s != INT_MAX && "ClearStencil is not defined");
        glClearStencil(stencil.s);
        clearBit |= GL_STENCIL_BUFFER_BIT;
      }
    }
    else if (attachment.loadFunc == LoadFunc::DontCare) {
      // do nothing
    }

    if (attachment.storeFunc != StoreFunc::Store) {
      assert(false && "OpenGL only supports StoreFunc::Store");
    }
  }
  glClear(clearBit);
}
