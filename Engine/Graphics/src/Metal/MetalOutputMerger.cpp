#include <Graphics/Metal/MetalOutputMerger.h>
#include <Graphics/Metal/MetalCommandEncoder.h>
#include <Graphics/Metal/MetalDevice.h>

#include <Metal/Metal.hpp>

using namespace goala;

MetalOutputMerger::MetalOutputMerger(MetalDevice* device)
  : m_device(device) {}

void MetalOutputMerger::bind(void* descriptor) {
  auto mtlDescriptor = static_cast<MTL::RenderPipelineDescriptor*>(descriptor);
  _updateAlphaBlend(mtlDescriptor);
}

void MetalOutputMerger::encode(CommandEncoder* encoder) {
  _updateDepthTest(encoder);
}

void MetalOutputMerger::_updateAlphaBlend(MTL::RenderPipelineDescriptor* descriptor) {
  auto& alphaBlend = getAlphaBlend();
  auto* colorAttachment = descriptor->colorAttachments()->object(0);
  if (!alphaBlend.enable) {
    colorAttachment->setBlendingEnabled(false);
    return;
  }

  if (alphaBlend.fragmentBlendFunc == AlphaBlend::BlendFunc::SRC_ALPHA) {
    colorAttachment->setSourceRGBBlendFactor(MTL::BlendFactorSourceAlpha);
    colorAttachment->setSourceAlphaBlendFactor(MTL::BlendFactorSourceAlpha);
  }
  else if (alphaBlend.fragmentBlendFunc == AlphaBlend::BlendFunc::ONE_MINUS_SRC_ALPHA) {
    colorAttachment->setSourceRGBBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);
    colorAttachment->setSourceAlphaBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);
  }
  else
    assert(false && "Undefined fragment blend function");

  if (alphaBlend.pixelBlendFunc == AlphaBlend::BlendFunc::SRC_ALPHA) {
    colorAttachment->setDestinationRGBBlendFactor(MTL::BlendFactorSourceAlpha);
    colorAttachment->setDestinationAlphaBlendFactor(MTL::BlendFactorSourceAlpha);
  }
  else if (alphaBlend.pixelBlendFunc == AlphaBlend::BlendFunc::ONE_MINUS_SRC_ALPHA) {
    colorAttachment->setDestinationRGBBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);
    colorAttachment->setDestinationAlphaBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);
  }
  else
    assert(false && "Undefined pixel blend function");

  if (alphaBlend.blendEquation == AlphaBlend::BlendEquation::Add) {
    colorAttachment->setRgbBlendOperation(MTL::BlendOperationAdd);
    colorAttachment->setAlphaBlendOperation(MTL::BlendOperationAdd);
  }
  else
    assert(false && "Undefined blend equation");
}

void MetalOutputMerger::_updateDepthTest(CommandEncoder* encoder) {
  auto& depthTest = getDepthTest();

  if (!depthTest.enable)
    return;

  auto descriptor = MTL::DepthStencilDescriptor::alloc()->init();

  if (depthTest.depthFunc == DepthTest::DepthTestFunc::Less)
    descriptor->setDepthCompareFunction(MTL::CompareFunctionLess);
  else
    assert(false && "Undefined depth function");

  descriptor->setDepthWriteEnabled(depthTest.updateDepthMask);

  auto depthStencilState = m_device->getMTLDevice()->newDepthStencilState(descriptor);

  auto metalEncoder = dynamic_cast<MetalCommandEncoder*>(encoder);
  auto mtlEncoder = metalEncoder->getEncoder();
  mtlEncoder->setDepthStencilState(depthStencilState);
}
