#include <Graphics/Metal/MetalRenderPass.h>
#include <Graphics/Metal/MetalTexture.h>

#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalDrawable.hpp>

using namespace goala;

MetalRenderPass::MetalRenderPass() : m_pass(MTL::RenderPassDescriptor::renderPassDescriptor()) {}

void MetalRenderPass::update() {
  _updateRenderPass();
}

void MetalRenderPass::_updateRenderPass() {
  if (!m_dirty)
    return;

  for (const auto& attachment : m_attachments) {
    assert(attachment.type != AttachmentType::Undefined && "AttachmentType is not defined");
    assert(attachment.loadFunc != LoadFunc::Undefined && "LoadFunc is not defined");
    assert(attachment.storeFunc != StoreFunc::Undefined && "StoreFunc is not defined");

    auto colorAttachment = m_pass->colorAttachments()->object(0);
    auto depthAttachment = m_pass->depthAttachment();
    auto stencilAttachment = m_pass->stencilAttachment();

    if (attachment.loadFunc == LoadFunc::Clear) {
      if (attachment.type == AttachmentType::Color) {
        const auto& color = std::get<ClearColor>(attachment.clear);
        assert(color.r >= 0.0f && color.g >= 0.0f && color.b >= 0.0f && color.a >= 0.0f && "ClearColor is not defined");

        colorAttachment->setLoadAction(MTL::LoadAction::LoadActionClear);
        colorAttachment->setStoreAction(MTL::StoreAction::StoreActionStore);
        colorAttachment->setClearColor(MTL::ClearColor(color.r, color.g, color.b, color.a));
      }
      else if (attachment.type == AttachmentType::Depth) {
        const auto& depth = std::get<ClearDepth>(attachment.clear);
        assert(depth.depth >= 0.0f && "ClearDepth is not defined");
      }
      else if (attachment.type == AttachmentType::Stencil) {
        const auto& stencil = std::get<ClearStencil>(attachment.clear);
        assert(stencil.s != INT_MAX && "ClearStencil is not defined");
      }
    }
    else if (attachment.loadFunc == LoadFunc::DontCare) {
      if (attachment.type == AttachmentType::Color)
        colorAttachment->setLoadAction(MTL::LoadAction::LoadActionDontCare);
      else if (attachment.type == AttachmentType::Depth)
        depthAttachment->setLoadAction(MTL::LoadAction::LoadActionDontCare);
      else if (attachment.type == AttachmentType::Stencil)
        stencilAttachment->setLoadAction(MTL::LoadAction::LoadActionDontCare);
    }
    else
      assert(false && "Undefined load function");

    if (attachment.storeFunc == StoreFunc::Store) {
      if (attachment.type == AttachmentType::Color)
        colorAttachment->setStoreAction(MTL::StoreAction::StoreActionStore);
      else if (attachment.type == AttachmentType::Depth)
        depthAttachment->setStoreAction(MTL::StoreAction::StoreActionStore);
      else if (attachment.type == AttachmentType::Stencil)
        stencilAttachment->setStoreAction(MTL::StoreAction::StoreActionStore);
    }
    else if (attachment.storeFunc == StoreFunc::DontCare) {
      if (attachment.type == AttachmentType::Color)
        colorAttachment->setStoreAction(MTL::StoreAction::StoreActionDontCare);
      else if (attachment.type == AttachmentType::Depth)
        depthAttachment->setStoreAction(MTL::StoreAction::StoreActionDontCare);
      else if (attachment.type == AttachmentType::Stencil)
        stencilAttachment->setStoreAction(MTL::StoreAction::StoreActionDontCare);
    }
    else
      assert(false && "Undefined store function");

    MTL::Texture* textureHandle = static_pointer_cast<MetalTexture>(attachment.texture)->getHandle();
    colorAttachment->setTexture(textureHandle); // todo: move
  }

  m_dirty = false;
}

MTL::RenderPassDescriptor* MetalRenderPass::getPass() {
  return m_pass.get();
}

const MTL::RenderPassDescriptor* MetalRenderPass::getPass() const {
  return m_pass.get();
}
