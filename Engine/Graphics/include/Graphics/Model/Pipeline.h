#pragma once

#include <Graphics/Model/Buffer.h>
#include <Graphics/Model/OutputMerger.h>
#include <Graphics/Model/Program.h>
#include <Graphics/Model/Rasterizer.h>
#include <Graphics/Model/RenderPass.h>

#include <memory>

namespace goala {

class Pipeline {
 public:
  virtual ~Pipeline() = default;

 public:
  virtual void update() = 0;
  virtual void render() = 0;

 public:
  void setBuffer(std::shared_ptr<Buffer> buffer) { m_buffer = std::move(buffer); }
  void setProgram(std::shared_ptr<Program> program) { m_program = std::move(program); }
  void setRenderPass(std::shared_ptr<RenderPass> renderPass) { m_renderPass = std::move(renderPass); }
  void setRasterizer(std::shared_ptr<Rasterizer> rasterizer) { m_rasterizer = std::move(rasterizer); }
  void setOutputMerger(std::shared_ptr<OutputMerger> outputMerger) { m_outputMerger = std::move(outputMerger); }

  const Buffer* getBuffer() const { return m_buffer.get(); }
  Buffer* getBuffer() { return m_buffer.get(); }

  const Program* getProgram() const { return m_program.get(); };
  Program* getProgram() { return m_program.get(); };

  const Rasterizer* getRasterizer() const { return m_rasterizer.get(); }
  Rasterizer* getRasterizer() { return m_rasterizer.get(); }

  const OutputMerger* getOutputMerger() const { return m_outputMerger.get(); }
  OutputMerger* getOutputMerger() { return m_outputMerger.get(); }

  const RenderPass* getRenderPass() const { return m_renderPass.get(); }
  RenderPass* getRenderPass() { return m_renderPass.get(); }

 protected:
  std::shared_ptr<Buffer> m_buffer;
  std::shared_ptr<Program> m_program;
  std::shared_ptr<RenderPass> m_renderPass;
  std::shared_ptr<Rasterizer> m_rasterizer;
  std::shared_ptr<OutputMerger> m_outputMerger;
};

} // namespace goala
