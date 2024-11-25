#include <Graphics/Metal/MetalBuffer.h>
#include <Graphics/Metal/MetalPipeline.h>
#include <Graphics/Metal/MetalShader.h>
#include <Graphics/Utility/ImageFormatUtil.h>

#include <Metal/Metal.hpp>

namespace goala {

MetalPipeline::MetalPipeline(MetalDevice* device) : m_device(device) {
  assert(device && "Device must not be null");
}

void MetalPipeline::initialize(const PipelineDescription& description) {
  auto pipelineDesc = MetalRef(MTL::RenderPipelineDescriptor::alloc()->init());

  for (const auto& shader : description.shaders) {
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

  auto metalBuffer = std::static_pointer_cast<MetalBuffer>(description.buffer);
  pipelineDesc->setVertexDescriptor(metalBuffer->getVertexDescriptor());

  auto colorAttachmentDesc = pipelineDesc->colorAttachments()->object(0);
  colorAttachmentDesc->setPixelFormat(getMetalImageFormat(description.format));

  NS::Error* err = nil;
  m_pipeline = MetalRef(m_device->getMTLDevice()->newRenderPipelineState(pipelineDesc.get(), &err));
  assert(m_pipeline && "Failed to create pipeline");

  m_buffer = description.buffer; // pipeline에서 descripion을 통째로 저장하고 있도록 하자
  m_shaders = description.shaders;
  m_rasterizer = description.rasterizer;
  m_outputMerger = description.outputMerger;
}

void MetalPipeline::update() {
}

void MetalPipeline::render() {
}

MTL::RenderPipelineState* MetalPipeline::getPipeline() const {
  return m_pipeline.get();
}

} // namespace goala
