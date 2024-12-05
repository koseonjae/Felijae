#pragma once

#include <Graphics/Model/Buffer.h>
#include <Graphics/Model/OutputMerger.h>
#include <Graphics/Model/Rasterizer.h>
#include <Graphics/Model/Uniforms.h>
#include <Graphics/Model/RenderPass.h>
#include <Graphics/Model/Shader.h>

#include <vector>

namespace goala {

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

public:
 virtual void update() = 0;
 virtual void render() = 0;

public:
 //  void setBuffer(std::shared_ptr<Buffer> buffer) { m_buffer = std::move(buffer); }
 //  void setProgram(std::shared_ptr<Program> program) { m_program = std::move(program); }
 //  void setRasterizer(std::shared_ptr<Rasterizer> rasterizer) { m_rasterizer = std::move(rasterizer); }
 //  void setOutputMerger(std::shared_ptr<OutputMerger> outputMerger) { m_outputMerger = std::move(outputMerger); }
 //  void setUniforms(std::shared_ptr<Uniforms> uniforms) { m_uniforms = std::move(uniforms); }

 const Buffer* getVertexBuffer() const { return m_vertexBuffer.get(); }
 Buffer* getVertexBuffer() { return m_vertexBuffer.get(); }

 const Uniforms* getUniforms() const { return m_desc.uniforms.get(); }
 Uniforms* getUniforms() { return m_desc.uniforms.get(); }

protected:
 PipelineDescription m_desc{};
 std::shared_ptr<Buffer> m_vertexBuffer;
};
} // namespace goala
