#pragma once

#include <Model/Buffer.h>
#include <Model/Program.h>
#include <Model/Rasterizer.h>
#include <Model/OutputMerger.h>

#include <memory>

class Pipeline {
 public:
  virtual ~Pipeline() = default;

 public:
  virtual void update() = 0;
  virtual void render() = 0;

 public:
  void setBuffer(std::shared_ptr<Buffer> buffer) { m_buffer = std::move(buffer); }
  void setProgram(std::shared_ptr<Program> program) { m_program = std::move(program); }
  void setRasterizer(const Rasterizer& rasterizer) { m_rasterizer = rasterizer; }
  void setOutputMerger(const OutputMerger& output_merger) { m_outputMerger = output_merger; }

  const Buffer* getBuffer() const { return m_buffer.get(); }
  Buffer* getBuffer() { return m_buffer.get(); }

  const Program* getProgram() const { return m_program.get(); };
  Program* getProgram() { return m_program.get(); };

  const Rasterizer& getRasterizer() const { return m_rasterizer; }
  Rasterizer& getRasterizer() { return m_rasterizer; }

  const OutputMerger& getOutputMerger() const { return m_outputMerger; }
  OutputMerger& getOutputMerger() { return m_outputMerger; }

 protected:
  std::shared_ptr<Buffer> m_buffer;
  std::shared_ptr<Program> m_program;
  Rasterizer m_rasterizer;
  OutputMerger m_outputMerger;
};
