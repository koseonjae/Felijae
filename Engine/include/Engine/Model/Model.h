#pragma once

#include <Graphics/Model/Program.h>

#include <glm/glm.hpp>
#include <memory>

namespace goala {

class Object;
class Program;
class Pipeline;
class Buffer;

class Model final {
 public:
  void update();

  void render();

  void setPipeline(std::shared_ptr<Pipeline> pipeline) {m_pipeline = std::move(pipeline);}

  const Pipeline* getPipeline() const { return m_pipeline.get(); }
  Pipeline* getPipeline() { return m_pipeline.get(); }

 public:
  void translate(glm::vec3 translate) {
    // todo: implement
    m_dirty = true;
  }

  void scale(glm::vec3 scale) {
    // todo: implement
    m_dirty = true;
  }

  void rotate(float degree, glm::vec3 axis) {
    // todo: implement
    m_dirty = true;
  }

  glm::mat4 calculateWorldMat() {
    if (!m_dirty) return m_worldMat;

    // todo: implement

    m_dirty = false;
    return m_worldMat;
  }

 protected:
  std::shared_ptr<Pipeline> m_pipeline;

 private:
  glm::vec3 m_translate{};
  glm::vec3 m_scale{};
  glm::vec4 m_rotate{};
  glm::mat4 m_worldMat = glm::mat4(1.0f);
  bool m_dirty = false;
};

} // namespace goala
