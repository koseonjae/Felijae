#pragma once

#include <Graphics/Model/Shader.h>
#include <Graphics/Model/Uniforms.h>

#include <vector>

namespace goala {
struct ComputeBufferDescription {
  std::vector<float> data;
  int width; // cols
  int height; // rows
};

struct ComputePipelineDescription {
  ShaderDescription shader;
  ComputeBufferDescription buffer;
  std::vector<UniformType> uniforms;
  std::vector<std::shared_ptr<Texture>> textures;
};

class ComputePipeline {
public:
  explicit ComputePipeline(ComputePipelineDescription desc) : m_desc(std::move(desc)) {}
  virtual ~ComputePipeline() = default;

protected:
  ComputePipelineDescription m_desc{};
};
} // namespace goala
