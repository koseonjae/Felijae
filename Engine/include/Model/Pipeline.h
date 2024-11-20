#pragma once

#include <Model/Program.h>
#include <Model/Rasterizer.h>
#include <Model/OutputMerger.h>

#include <memory>

class Pipeline {
 public:
  virtual ~Pipeline() = default;

 public:
  virtual void bind() = 0;

 public:
  void setRasterizer(const Rasterizer& rasterizer) { m_rasterizer = rasterizer; }
  void setOutputMerger(const OutputMerger& output_merger) { m_outputMerger = output_merger; }
  void setProgram(std::shared_ptr<Program> program) { m_program = std::move(program); }

  const Rasterizer& getRasterizer() const { return m_rasterizer; }
  Rasterizer& getRasterizer() { return m_rasterizer; }
  
  const OutputMerger& getOutputMerger() const { return m_outputMerger; }
  OutputMerger& getOutputMerger() { return m_outputMerger; }

  const Program* getProgram() const { return m_program.get(); };
  Program* getProgram() { return m_program.get(); };

 protected:
  std::shared_ptr<Program> m_program;
  Rasterizer m_rasterizer;
  OutputMerger m_outputMerger;
};
