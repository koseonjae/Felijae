#pragma once

#include <Graphics/Model/Pipeline.h>
#include <Graphics/Utility/MetalRef.h>
#include <Metal/MTLDepthStencil.hpp>

#include <unordered_map>
#include <vector>
#include <Metal/MTLRenderPipeline.hpp>

namespace MTL {
class RenderPipelineState;
class RenderPipelineDescriptor;
class RenderCommandEncoder;
class RenderPipelineReflection;
class Buffer;
}

namespace goala {
class MetalDevice;
class MetalCommandEncoder;

struct UniformReflection {
  std::string blockName;
  std::string name;
  uint32_t type = 0; // todo: remove
  uint32_t size = 0;
  uint32_t offset = 0;
  uint32_t index = 0; // todo: remove
};

class MetalPipeline : public Pipeline {
public:
  MetalPipeline(MetalDevice* device, PipelineDescription desc);

  MTL::RenderPipelineState* getPipeline();

  const MTL::RenderPipelineState* getPipeline() const;

  void encode(MetalCommandEncoder* encoder);

private:
  void _initializeShaders(MTL::RenderPipelineDescriptor* pipelineDesc);
  void _initializeVertexBuffer(MTL::RenderPipelineDescriptor* pipelineDesc);
  void _initializeAttachments(MTL::RenderPipelineDescriptor* pipelineDesc);
  void _initializeDepthStencilState();
  void _initializeAlphaBlend(MTL::RenderPipelineDescriptor* descriptor);
  void _initializeReflection(MTL::RenderPipelineReflection* reflection);
  void _initializePipeline(MTL::RenderPipelineDescriptor* descriptor);
  void _encodeViewport(MTL::RenderCommandEncoder* encoder);
  void _encodeCulling(MTL::RenderCommandEncoder* encoder);
  void _encodeUniformTextures(MTL::RenderCommandEncoder* encoder);
  void _encodeUniformVariables(MetalCommandEncoder* encoder);

private:
  MetalDevice* m_device = nullptr;
  std::vector<std::shared_ptr<Shader>> m_shaders;
  MetalRef<MTL::RenderPipelineState> m_pipeline;
  MetalRef<MTL::DepthStencilState> m_depthStencilState;

  std::unordered_map<std::string, std::vector<uint8_t>> m_uniformBlockBuffers;
  std::unordered_map<std::string, int> m_uniformBlockIdx;
  std::unordered_map<std::string, std::vector<UniformReflection>> m_uniformReflectionMap;
};
} // namespace goala
