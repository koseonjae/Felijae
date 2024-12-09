#pragma once

#include <Graphics/Model/ComputePipeline.h>
#include <Graphics/Utility/MetalRef.h>

namespace MTL {
class ComputePipelineState;
class ComputePipelineDescriptor;
class ComputeCommandEncoder;
class Buffer;
}

namespace goala {
class MetalDevice;
class MetalCommandEncoder;
class MetalShader;
class Texture;

class MetalComputePipeline : public ComputePipeline {
public:
  MetalComputePipeline(MetalDevice* device, ComputePipelineDescription pipeline);

  void encode(MTL::ComputeCommandEncoder* encoder);

private:
  void _initializeBuffer(MTL::ComputePipelineDescriptor* pipelineDesc);
  void _initializeShader(MTL::ComputePipelineDescriptor* pipelineDesc);
  void _initializePipeline(MTL::ComputePipelineDescriptor* pipelineDesc);

private:
  MetalDevice* m_device = nullptr;
  std::shared_ptr<Shader> m_shader;
  MetalRef<MTL::ComputePipelineState> m_pipelineState;
  MetalRef<MTL::Buffer> m_buffer; // todo: MetalBuffer로 wrap
};

}
