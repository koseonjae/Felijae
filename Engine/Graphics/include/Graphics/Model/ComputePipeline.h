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
  // VertexBufferDescription vertexBuffer;
  // std::vector<ShaderDescription> shaders;
  // Rasterizer rasterizer{};
  // OutputMerger outputMerger{};
  // std::shared_ptr<Uniforms> uniforms;
  // ImageFormat format;
};

class ComputePipeline {
public:
  explicit ComputePipeline(ComputePipelineDescription desc) : m_desc(std::move(desc)) {}
  virtual ~ComputePipeline() = default;

  // const Buffer* getVertexBuffer() const { return m_vertexBuffer.get(); }
  // Buffer* getVertexBuffer() { return m_vertexBuffer.get(); }
  //
  // const Uniforms* getUniforms() const { return m_desc.uniforms.get(); }
  // Uniforms* getUniforms() { return m_desc.uniforms.get(); }

protected:
  ComputePipelineDescription m_desc{};
  // std::shared_ptr<Buffer> m_vertexBuffer;
};
} // namespace goala
