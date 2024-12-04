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

struct UniformReflection {
  // UniformReflection(const UniformReflection&) = delete;
  // UniformReflection(UniformReflection&&) = delete;
  // UniformReflection& operator=(const UniformReflection&) = delete;
  // UniformReflection& operator=(UniformReflection&&) = delete;
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

  void update() override;

  void render() override;

  MTL::RenderPipelineState* getPipeline();

  const MTL::RenderPipelineState* getPipeline() const;

  void encode(MTL::RenderCommandEncoder* encoder);

private:
  void _initializeDepthTest();
  void _initializeAlphaBlend(MTL::RenderPipelineDescriptor* descriptor);
  void _initializeReflection(MTL::RenderPipelineReflection* reflection);
  void _encodeViewport(MTL::RenderCommandEncoder* encoder);
  void _encodeCulling(MTL::RenderCommandEncoder* encoder);
  void _encodeUniformTextures(MTL::RenderCommandEncoder* encoder);
  void _encodeUniformVariables(MTL::RenderCommandEncoder* encoder);

private:
  MetalDevice* m_device = nullptr;
  MetalRef<MTL::RenderPipelineState> m_pipeline;
  MetalRef<MTL::DepthStencilState> m_depthStencilState;

  std::unordered_map<std::string, std::vector<uint8_t>> m_uniformBlockBuffers;
  std::unordered_map<std::string, int> m_uniformBlockIdx;
  std::unordered_map<std::string, std::vector<UniformReflection>> m_uniformReflectionMap;
  std::unordered_map<std::string, MetalRef<MTL::Buffer>> m_mtlUniformBlocks;
};
} // namespace goala
