#include <Graphics/Metal/MetalBuffer.h>
#include <Graphics/Metal/MetalPipeline.h>
#include <Graphics/Metal/MetalShader.h>
#include <Graphics/Utility/ImageFormatUtil.h>
#include <Base/Utility/TypeCast.h>

#include <Metal/Metal.hpp>
#include <Metal/triangle_types.h>

namespace goala {
MetalPipeline::MetalPipeline(MetalDevice* device, PipelineDescription desc)
  : Pipeline(std::move(desc))
  , m_device(device) {
  auto pipelineDesc = MTL::RenderPipelineDescriptor::alloc()->init();

  for (const auto& shader : m_desc.shaders) {
    auto metalShader = std::static_pointer_cast<MetalShader>(shader);
    assert(metalShader && "Failed to cast to MetalShader");
    switch (metalShader->getShaderType()) {
      case ShaderType::VERTEX: {
        pipelineDesc->setVertexFunction(metalShader->getFunction());
        break;
      }
      case ShaderType::FRAGMENT: {
        pipelineDesc->setFragmentFunction(metalShader->getFunction());
        break;
      }
      case ShaderType::COMPUTE: {
        assert(false && "Compute shader is not supported");
        break;
      }
      case ShaderType::UNDEFINED: {
        assert(false && "Shader type is not defined");
        break;
      }
    }
  }

  auto metalBuffer = std::static_pointer_cast<MetalBuffer>(m_desc.buffer);
  pipelineDesc->setVertexDescriptor(metalBuffer->getVertexDescriptor());

  auto colorAttachmentDesc = pipelineDesc->colorAttachments()->object(0);
  colorAttachmentDesc->setPixelFormat(getMetalImageFormat(m_desc.format));

  _initializeDepthTest();
  _initializeAlphaBlend(pipelineDesc);

  NS::Error* err = nil;
  m_pipeline = makeMetalRef(device->getMTLDevice()->newRenderPipelineState(pipelineDesc, &err));
  assert(m_pipeline && "Failed to create pipeline");
}

void MetalPipeline::update() {}

void MetalPipeline::render() {}

MTL::RenderPipelineState* MetalPipeline::getPipeline() {
  return m_pipeline.get();
}

const MTL::RenderPipelineState* MetalPipeline::getPipeline() const {
  return m_pipeline.get();
}

void MetalPipeline::encode(MTL::RenderCommandEncoder* encoder) {
  auto metalBuffer = SAFE_DOWN_CAST(MetalBuffer*, getBuffer());
  _encodeViewport(encoder);
  _encodeCulling(encoder);
  encoder->setDepthStencilState(m_depthStencilState.get());
  encoder->setRenderPipelineState(m_pipeline.get());
  encoder->setVertexBuffer(metalBuffer->getVertexHandle(), 0, AAPLVertexInputIndexVertices);
  encoder->drawIndexedPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle,
                                 metalBuffer->getIndicesSize(),
                                 MTL::IndexTypeUInt32,
                                 metalBuffer->getIndexHandle(),
                                 0);
}

void MetalPipeline::_encodeViewport(MTL::RenderCommandEncoder* encoder) {
  const auto& viewport = m_desc.rasterizer.viewport;
  MTL::Viewport mltViewport = {
    .originX = static_cast<double>(viewport.minX),
    .originY = static_cast<double>(viewport.minY),
    .width = static_cast<double>(viewport.width),
    .height = static_cast<double>(viewport.height),
    .znear = static_cast<double>(viewport.minZ),
    .zfar = static_cast<double>(viewport.maxZ),
  };
  encoder->setViewport(mltViewport);
}

void MetalPipeline::_encodeCulling(MTL::RenderCommandEncoder* encoder) {
  const auto& culling = m_desc.rasterizer.culling;

  if (!culling.enable) {
    encoder->setCullMode(MTL::CullModeNone);
    return;
  }

  if (culling.cullMode == Culling::CullMode::Front)
    encoder->setCullMode(MTL::CullModeFront);
  else
    encoder->setCullMode(MTL::CullModeBack);

  if (culling.frontFace == Culling::FrontFace::CCW)
    encoder->setFrontFacingWinding(MTL::WindingCounterClockwise);
  else
    encoder->setFrontFacingWinding(MTL::WindingClockwise);
}

void MetalPipeline::_initializeDepthTest() {
  const auto& depthTest = m_desc.outputMerger.depthTest;
  if (!depthTest.enable)
    return;

  auto descriptor = MTL::DepthStencilDescriptor::alloc()->init();

  if (depthTest.depthFunc == DepthTest::DepthTestFunc::Less)
    descriptor->setDepthCompareFunction(MTL::CompareFunctionLess);
  else
    assert(false && "Undefined depth function");

  descriptor->setDepthWriteEnabled(depthTest.updateDepthMask);

  m_depthStencilState = makeMetalRef(m_device->getMTLDevice()->newDepthStencilState(descriptor));
}

void MetalPipeline::_initializeAlphaBlend(MTL::RenderPipelineDescriptor* descriptor) {
  const auto& alphaBlend = m_desc.outputMerger.alphaBlend;

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
} // namespace goala
