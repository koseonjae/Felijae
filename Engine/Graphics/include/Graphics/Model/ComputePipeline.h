#pragma once

#include <Graphics/Model/Shader.h>
#include <Graphics/Model/Uniforms.h>
#include <Graphics/Utility/MetalRef.h>

#include <vector>

namespace MTL {
class Buffer;
}

namespace goala {
struct ComputeBufferDescription {
  std::vector<float> data;
};

struct ComputePipelineDescription {
  ShaderDescription shader;
  std::vector<MetalRef<MTL::Buffer>> buffers;
  std::vector<UniformType> uniforms;
  std::vector<std::shared_ptr<Texture>> textures;
  std::vector<int> threadSize;
};

class ComputePipeline {
public:
  explicit ComputePipeline(ComputePipelineDescription desc) : m_desc(std::move(desc)) {}
  virtual ~ComputePipeline() = default;

protected:
  ComputePipelineDescription m_desc{};
};
} // namespace goala
