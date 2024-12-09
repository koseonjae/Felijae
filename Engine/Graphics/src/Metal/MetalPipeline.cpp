#include <Graphics/Metal/MetalBuffer.h>
#include <Graphics/Metal/MetalPipeline.h>
#include <Graphics/Metal/MetalShader.h>
#include <Graphics/Utility/ImageFormatUtil.h>
#include <Graphics/Metal/MetalTexture.h>
#include <Base/Utility/TypeCast.h>
#include <ShaderConverter/ShaderConverter.h>
#include <Graphics/Metal/MetalCommandEncoder.h>

#include <Metal/Metal.hpp>

#include <glm/gtc/type_ptr.hpp>

namespace {
uint32_t getTypeSize(MTL::DataType dataType) {
  switch (dataType) {
    case MTL::DataType::DataTypeFloat: return sizeof(float);
    case MTL::DataType::DataTypeInt: return sizeof(int);
    case MTL::DataType::DataTypeUInt: return sizeof(unsigned int);
    case MTL::DataType::DataTypeBool: return sizeof(bool);

    case MTL::DataType::DataTypeFloat2: return sizeof(float) * 2;
    case MTL::DataType::DataTypeFloat3: return sizeof(float) * 3;
    case MTL::DataType::DataTypeFloat4: return sizeof(float) * 4;
    case MTL::DataType::DataTypeInt2: return sizeof(int) * 2;
    case MTL::DataType::DataTypeInt3: return sizeof(int) * 3;
    case MTL::DataType::DataTypeInt4: return sizeof(int) * 4;

    case MTL::DataType::DataTypeFloat2x2: return sizeof(float) * 2 * 2;
    case MTL::DataType::DataTypeFloat3x3: return sizeof(float) * 3 * 3;
    case MTL::DataType::DataTypeFloat4x4: return sizeof(float) * 4 * 4;

    default: {}
  }
  assert(false && "Unsupported data type");
}
}

namespace goala {
MetalPipeline::MetalPipeline(MetalDevice* device, PipelineDescription desc)
  : Pipeline(std::move(desc))
  , m_device(device) {
  auto pipelineDesc = MTL::RenderPipelineDescriptor::alloc()->init();
  _initializeAttachments(pipelineDesc);
  _initializeVertexBuffer(pipelineDesc);
  _initializeDepthStencilState();
  _initializeAlphaBlend(pipelineDesc);
  _initializeShaders(pipelineDesc);
  _initializePipeline(pipelineDesc);
}

MTL::RenderPipelineState* MetalPipeline::getPipeline() {
  return m_pipeline.get();
}

const MTL::RenderPipelineState* MetalPipeline::getPipeline() const {
  return m_pipeline.get();
}

void MetalPipeline::encode(MetalCommandEncoder* metalEncoder) {
  auto metalBuffer = SAFE_DOWN_CAST(MetalBuffer*, getVertexBuffer());
  auto encoder = metalEncoder->getEncoder();
  _encodeUniformTextures(encoder);
  _encodeUniformVariables(metalEncoder);
  _encodeViewport(encoder);
  _encodeCulling(encoder);

  if (m_depthStencilState.get())
    encoder->setDepthStencilState(m_depthStencilState.get());
  encoder->setRenderPipelineState(m_pipeline.get());
  encoder->setVertexBuffer(metalBuffer->getVertexHandle(), 0, VERTEX_BUFFER_INDEX);
  encoder->drawIndexedPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle,
                                 metalBuffer->getIndicesSize(),
                                 MTL::IndexTypeUInt32,
                                 metalBuffer->getIndexHandle(),
                                 0);
}

void MetalPipeline::_initializeVertexBuffer(MTL::RenderPipelineDescriptor* pipelineDesc) {
  m_vertexBuffer = m_device->createBuffer(m_desc.vertexBuffer);
  auto metalVertexBuffer = SAFE_DOWN_CAST(MetalBuffer*, m_vertexBuffer.get());
  pipelineDesc->setVertexDescriptor(metalVertexBuffer->getVertexDescriptor());
}

void MetalPipeline::_initializeAttachments(class MTL::RenderPipelineDescriptor* pipelineDesc) {
  auto colorAttachmentDesc = pipelineDesc->colorAttachments()->object(0);
  colorAttachmentDesc->setPixelFormat(getMetalImageFormat(m_desc.format));
}

void MetalPipeline::_initializeShaders(MTL::RenderPipelineDescriptor* pipelineDesc) {
  for (const auto& shader : m_desc.shaders) {
    // vulkan glsl to msl
    auto convertedShader = convertShader({
      .shaderSource = shader.source,
      .shaderType = getShaderConverterType(shader.type),
      .shaderConverterType = ShaderConverterTarget::MSL
    });
    ShaderDescription shaderDesc = {
      .source = convertedShader,
      .type = shader.type
    };
    auto shaderFunc = m_device->createShader(std::move(shaderDesc));
    m_shaders.push_back(std::move(shaderFunc));
  }

  for (const auto& shader : m_shaders) {
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

void MetalPipeline::_encodeUniformTextures(MTL::RenderCommandEncoder* encoder) {
  auto& uniforms = m_desc.uniforms;
  auto& textures = uniforms->getTextures();
  if (textures.empty())
    return;

  int fragSlot = 0;
  for (const auto& [name, texture] : textures) {
    // todo: distinguish pipeline
    auto metalTexture = SAFE_DOWN_CAST(MetalTexture*, texture.get());
    encoder->setFragmentTexture(metalTexture->getTextureHandle(), fragSlot);
    encoder->setFragmentSamplerState(metalTexture->getSamplerHandle(), fragSlot);
    ++fragSlot;
  }
}

void MetalPipeline::_encodeUniformVariables(MetalCommandEncoder* encoder) {
  auto& uniforms = m_desc.uniforms;
  auto& variables = uniforms->getUniforms();
  if (variables.empty())
    return;

  auto& uniformBlockBuffers = m_uniformBlockBuffers;
  const auto& reflectionsMap = m_uniformReflectionMap;
  assert(uniformBlockBuffers.size() == reflectionsMap.size());

  // todo: use Device::createDevice

  auto findReflection = [&reflectionsMap](const std::string& name) -> const UniformReflection& {
    for (const auto& [blockName, reflections] : reflectionsMap) {
      auto found = std::ranges::find_if(reflections, [&name](const auto& reflection) {
        return reflection.name == name;
      });
      if (found != reflections.end())
        return *found;
    }
    assert(false && "Reflection not found");
  };

  for (const auto& [name, variable] : variables) {
    const auto& reflection = findReflection(name);
    assert(uniformBlockBuffers.contains(reflection.blockName) && "Uniform block not found");
    auto& uniformBlock = uniformBlockBuffers[reflection.blockName];

    const void* valuePtr = nullptr;
    std::visit([&valuePtr](auto& value) {
      using T = std::decay_t<decltype(value)>;
      if constexpr (std::is_same_v<T, glm::vec3>)
        valuePtr = reinterpret_cast<const void*>(glm::value_ptr(value));
      else if constexpr (std::is_same_v<T, glm::mat4>)
        valuePtr = reinterpret_cast<const void*>(glm::value_ptr(value));
    }, variable);

    std::memcpy(uniformBlock.data() + reflection.offset, valuePtr, reflection.size);
  }

  for (auto& [uniformBlockName, uniformBlock] : uniformBlockBuffers) {
    auto buffer = makeMetalRef(m_device->getMTLDevice()->newBuffer(uniformBlock.size(), MTL::ResourceStorageModeShared));
    std::memcpy(buffer->contents(), uniformBlock.data(), uniformBlock.size());
    encoder->setBufferTemp(buffer, 0, m_uniformBlockIdx.at(uniformBlockName));
  }
}

void MetalPipeline::_initializeDepthStencilState() {
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

void MetalPipeline::_initializeReflection(MTL::RenderPipelineReflection* reflection) {
  int count = static_cast<int>(reflection->vertexBindings()->count());
  for (int i = 0; i < count; ++i) {
    const auto* arg = reinterpret_cast<MTL::Argument*>(reflection->vertexBindings()->object(i));
    std::string uniformBlockName = arg->name()->utf8String();
    const auto size = static_cast<int>(arg->bufferDataSize());
    const auto index = static_cast<int>(arg->index());

    const auto members = arg->bufferStructType()->members();
    if (!members)
      continue;

    m_uniformBlockBuffers[uniformBlockName].resize(size);
    m_uniformBlockIdx[uniformBlockName] = index;
    m_uniformReflectionMap.insert({uniformBlockName, {}});

    for (int j = 0; j < members->count(); ++j) {
      const auto* member = reinterpret_cast<MTL::StructMember*>(members->object(j));
      m_uniformReflectionMap[uniformBlockName].push_back(
        UniformReflection{
          .blockName = uniformBlockName,
          .name = member->name()->utf8String(),
          .type = static_cast<uint32_t>(member->dataType()),
          .size = getTypeSize(member->dataType()),
          .offset = static_cast<uint32_t>(member->offset()),
          .index = static_cast<uint32_t>(member->argumentIndex())
        }
      );
    }
  }
}

void MetalPipeline::_initializePipeline(MTL::RenderPipelineDescriptor* pipelineDesc) {
  NS::Error* err = nil;
  MTL::RenderPipelineReflection* reflection = nil;
  auto option = MTL::PipelineOptionArgumentInfo | MTL::PipelineOptionBufferTypeInfo;
  m_device->getMTLDevice()->newRenderPipelineState(pipelineDesc, option, &reflection, &err);
  m_pipeline = makeMetalRef(m_device->getMTLDevice()->newRenderPipelineState(pipelineDesc, &err));
  assert(m_pipeline && "Failed to create pipeline");

  _initializeReflection(reflection);
}
} // namespace goala
