#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <glm/ext/matrix_transform.hpp>

namespace goala {
class Object;
class Pipeline;
class Buffer;

class Model {
public:
  void update(); // todo: model이 update만 있다 render는 없다. render는 pipeline 객체를 가져와서 하는중. 구조 정리하자

  void setPipeline(std::shared_ptr<Pipeline> pipeline) { m_pipeline = std::move(pipeline); }

  const Pipeline* getPipeline() const { return m_pipeline.get(); }
  Pipeline* getPipeline() { return m_pipeline.get(); }

public:
  void translate(glm::vec3 translate) {
    m_translate += translate;
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

    m_worldMat = glm::translate(m_worldMat, m_translate);

    m_dirty = false;
    return m_worldMat;
  }

protected:
  std::shared_ptr<Pipeline> m_pipeline;

private:
  glm::vec3 m_translate{};
  glm::vec3 m_scale{1.0f, 1.0f, 1.0f};
  glm::vec4 m_rotate{};
  glm::mat4 m_worldMat = glm::mat4(1.0f);
  bool m_dirty = false;
};
} // namespace goala
