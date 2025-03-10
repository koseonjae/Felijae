#pragma once

#include <Graphics/Model/VertexBuffer.h>
#include <Graphics/Model/OutputMerger.h>
#include <Graphics/Model/Rasterizer.h>
#include <Graphics/Model/Uniforms.h>
#include <Graphics/Model/RenderPass.h>
#include <Graphics/Model/Shader.h>

#include <vector>

namespace larco {

struct PipelineDescription {
 VertexBufferDescription vertexBuffer;
 std::vector<ShaderDescription> shaders;
 Rasterizer rasterizer{};
 OutputMerger outputMerger{};
 std::shared_ptr<Uniforms> uniforms;
 ImageFormat format;
};

class Pipeline {
public:
 explicit Pipeline(PipelineDescription desc) : m_desc(std::move(desc)) {}
 virtual ~Pipeline() = default;

 const Buffer* getVertexBuffer() const { return m_vertexBuffer.get(); }
 Buffer* getVertexBuffer() { return m_vertexBuffer.get(); }

 const Uniforms* getUniforms() const { return m_desc.uniforms.get(); }
 Uniforms* getUniforms() { return m_desc.uniforms.get(); }

protected:
 PipelineDescription m_desc{};
 std::shared_ptr<Buffer> m_vertexBuffer;
};
} // namespace larco
