#pragma once

#include <Model/Program.h>

#include <memory>

class Object;
class Program;
class Pipeline;

class Model {
 public:
  Model() = default;

  virtual ~Model() = default;

  virtual void initialize(const Object &) = 0;

  virtual void release() = 0;

  virtual void update() = 0;

  virtual void render() = 0;

  void setProgram(std::unique_ptr<Program> program) {
    m_program = std::move(program);
  }

  void setPipeline(std::shared_ptr<Pipeline> pipeline) {
    m_pipeline = std::move(pipeline);
  }

 protected:
  std::unique_ptr<Program> m_program;
  std::shared_ptr<Pipeline> m_pipeline;
};