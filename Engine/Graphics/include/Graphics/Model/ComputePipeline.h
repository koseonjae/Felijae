#pragma once

#include <Graphics/Model/Shader.h>

#include <vector>

namespace goala {
struct ComputeBufferDescription {
  std::vector<float> data;
  int width;
  int height;
};

struct ComputePipelineDescription {
  ShaderDescription shader;
  ComputeBufferDescription buffer;
};

class ComputePipeline {
public:
  explicit ComputePipeline(ComputePipelineDescription desc) : m_desc(std::move(desc)) {}
  virtual ~ComputePipeline() = default;

protected:
  ComputePipelineDescription m_desc{};
};
} // namespace goala
