#pragma once

#include <Model/Program.h>

#include <memory>

class Object;
class Program;
class Pipeline;
class Buffer;

class Model {
 public:
  Model() = default;

  virtual ~Model() = default;

  virtual void update() = 0;

  virtual void render() = 0;

  void setPipeline(std::shared_ptr<Pipeline> pipeline) { m_pipeline = std::move(pipeline); }

  const Pipeline* getPipeline() const { return m_pipeline.get(); }
  Pipeline* getPipeline() { return m_pipeline.get(); }

  void setBuffer(std::shared_ptr<Buffer> buffer) {
    m_buffer = std::move(buffer);
  }

 protected:
  std::shared_ptr<Pipeline> m_pipeline;
  std::shared_ptr<Buffer> m_buffer;
};
