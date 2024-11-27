#include <Graphics/Metal/MetalBuffer.h>
#include <Graphics/Metal/MetalPipeline.h>
#include <Graphics/Metal/MetalShader.h>
#include <Graphics/Utility/ImageFormatUtil.h>

#include <Metal/Metal.hpp>

namespace goala {

MetalPipeline::MetalPipeline(MetalDevice* device, PipelineDescription desc)
  : Pipeline(std::move(desc)) {
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

  m_desc.rasterizer->bind(pipelineDesc);
  m_desc.outputMerger->bind(pipelineDesc);

  NS::Error* err = nil;
  m_pipeline = makeMetalRef(device->getMTLDevice()->newRenderPipelineState(pipelineDesc, &err));
  assert(m_pipeline && "Failed to create pipeline");
}

void MetalPipeline::update() {
}

void MetalPipeline::render() {
}

MTL::RenderPipelineState* MetalPipeline::getPipeline() {
  return m_pipeline.get();
}

const MTL::RenderPipelineState* MetalPipeline::getPipeline() const {
  return m_pipeline.get();
}

} // namespace goala
