#include <Graphics/OpenGL/OpenGLRenderPass.h>
#include <OpenGL/gl3.h>

#include <cassert>

using namespace goala;

OpenGLRenderPass::OpenGLRenderPass(OpenGLDevice* device, RenderPassDescription desc)
  : m_device(device)
  , m_desc(std::move(desc)) {
  _updateFrameBuffers(m_desc);
}

void OpenGLRenderPass::bind() {
  for (auto& frameBuffer : m_frameBuffers)
    frameBuffer.bind();
  _updateRenderPass(m_desc);
}

void OpenGLRenderPass::_updateFrameBuffers(const RenderPassDescription& desc) {
  m_frameBuffers.clear();
  int colorAttachmentCnt = 0;
  for (const auto& attachment : desc.attachments) {
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
}

void OpenGLRenderPass::_updateRenderPass(const RenderPassDescription& desc) {
  GLbitfield clearBit = GL_NONE;

  for (const auto& attachment : desc.attachments) {
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
