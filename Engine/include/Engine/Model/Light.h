#pragma once

#include <Engine/Model/Node.h>

#include <glm/glm.hpp>

namespace goala {

class Light : public Node {
 public:
  void setLightColor(const glm::vec3& color) { m_lightColor = color; }
  void setLightDirection(const glm::vec3& dir) { m_lightDir = dir; }
  glm::vec3 getLightColor() const { return m_lightColor; };
  glm::vec3 getLightDirection() const { return m_lightDir; };

 private:
  glm::vec3 m_lightColor{};
  glm::vec3 m_lightDir{};
};

} // namespace goala
